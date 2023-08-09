#include <lua.hpp>

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    return 0;
}