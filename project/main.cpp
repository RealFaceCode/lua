#include "luaScript.h"
#include "luaTable.h"
#include "DynamicVar.h"

int c_func(LuaScript& L)
{   
    auto str = L.toString(1);
    std::cout << "log from lua: " << str << std::endl;
    return L.getRetValCount();
}

void printTableTest()
{
        LuaTable table("temp");
        table.addValue<long long>("x", 12);
        auto& nT = table.addValue<LuaTable>(LuaTable("second")).retrieve<LuaTable>();
        nT.addValue<std::string_view>("name", "dave");
        nT.addValue<LuaTable>(LuaTable("another nested"));
        auto& nT2 = table.addValue<LuaTable>(LuaTable("third")).retrieve<LuaTable>();
        nT2.addValue<double>(11.11);
        nT2.addValue<LuaTable>(LuaTable());

        table.print();
}

int main() 
{   
    ::printTableTest();

    LuaScript lua("../../data/scripts/script.lua");

    std::cout << std::endl;
    std::cout << std::endl;

    long long delta = 1;
    std::string_view str = "";
    FuncDescription descUpdate;
    descUpdate.addArg(delta, LuaValueType::integer);
    descUpdate.addRetVal(str, LuaValueType::string);

    lua.regFunc("update",  descUpdate);
    lua.regFunc(::c_func, "log");

    lua.compile();

    auto first = lua.getTable("first");
    first.print();

    std::cout << std::endl;

    first.setName("newFirst");
    lua.pushTable(first);

    auto newFirst = lua.getTable("newFirst");
    newFirst.print();

    std::cout << std::endl;
    std::cout << std::endl;

    auto indexed = lua.getTable("indexedTable");
    indexed.print();

    std::cout << std::endl;

    indexed.setName("newIndexedTable");
    lua.pushTable(indexed);

    auto newIndexed = lua.getTable("newIndexedTable");
    newIndexed.print();

	lua.doFunc("update");

    return 0;
}