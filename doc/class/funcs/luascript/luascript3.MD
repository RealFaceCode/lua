# LuaScript - LuaScript(const std::filesystem::path& path, std::size_t libs)

Constructor for the `LuaScript` object. The path to the lua script is saved in the object. The second passed argument will load the given lua libraries. A lua state will be created and all lua libraries will be loaded.

## Example

```cpp
LuaScript lua("path/to/the/lua/script.lua", Lua_lib_all);
```

## Defines / constexpr

```cpp
constexpr std::size_t Lua_lib_package     = 0x000000000000000F;
constexpr std::size_t Lua_lib_table       = 0x00000000000000F0;
constexpr std::size_t Lua_lib_string      = 0x0000000000000F00;
constexpr std::size_t Lua_lib_math        = 0x000000000000F000;
constexpr std::size_t Lua_lib_debug       = 0x00000000000F0000;
constexpr std::size_t Lua_lib_io          = 0x0000000000F00000;
constexpr std::size_t Lua_lib_coroutine   = 0x000000000F000000;
constexpr std::size_t Lua_lib_os          = 0x00000000F0000000;
constexpr std::size_t Lua_lib_utf8        = 0x0000000F00000000;
constexpr std::size_t Lua_lib_all         = ::Lua_lib_package | ::Lua_lib_table | ::Lua_lib_string | ::Lua_lib_math |
                                                ::Lua_lib_debug | ::Lua_lib_io | ::Lua_lib_coroutine | ::Lua_lib_os |
                                                ::Lua_lib_utf8;
```

## Other links

- [Usage](../../../usage.MD)
- [LuaScript()](luascript0.MD)
- [LuaScript(const std::filesystem::path&)](luascript1.MD)
- [LuaScript(LuaScript(std::size_t libs)](luascript2.MD)
- [~LuaScript()](luascript4.MD)
