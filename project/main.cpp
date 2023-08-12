#include "luaScript.h"

int c_func(LuaScript& L)
{   
    auto str = L.toString(1);
    std::cout << "log from lua: " << str << std::endl;
    return L.getRetValCount();
}

int main() 
{
    LuaScript lua("../../data/scripts/script.lua");

    lua.regFunc("update");
    lua.regFunc(c_func, "log");

    lua.compile();
    
    lua.doFunc("update");

    return 0;
}