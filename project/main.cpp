#include "luaScript.h"
#include "luaTable.h"

#include "DynamicVar.h"

int c_func(LuaScript& L)
{   
    auto str = L.toString(1);
    std::cout << "log from lua: " << str << std::endl;
    return L.getRetValCount();
}

int main() 
{
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
    

    LuaScript lua("../../data/scripts/script.lua");

    long long delta = 1;
    std::string_view str = "";
    FuncDescription descUpdate;
    descUpdate.addArg(delta, LuaValueType::integer);
    descUpdate.addRetVal(str, LuaValueType::string);

    lua.regFunc("update",  descUpdate);
    lua.regFunc(::c_func, "log");

    lua.compile();

    auto t = lua.getTable("first");
    t.print();

    auto tt = lua.getTable("indexedTable");
    tt.print();

	lua.doFunc("update");

    return 0;
}