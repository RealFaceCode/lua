#include "luaScript.h"
#include <format>

int c_func(LuaScript& L)
{   
    auto str = L.toString(1);
    std::cout << "log from lua: " << str << std::endl;
    return L.getRetValCount();
}

int main() 
{
    LuaScript lua("../../data/scripts/script.lua");

    long long delta = 1;
    std::string_view str = "";
    FuncDescription descUpdate;
    descUpdate.addArg(delta, LuaValueType::integer);
    descUpdate.addRetVal(str, LuaValueType::string);

    lua.regFunc("update",  descUpdate);
    lua.regFunc(::c_func, "log");

    lua.compile();
    
	lua.doFunc("update");

    return 0;
}