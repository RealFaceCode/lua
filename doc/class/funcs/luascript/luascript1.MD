# LuaScript - LuaScript(const std::filesystem::path&)

Constructor for the `LuaScript` object. The path to the lua script is saved in the object. A lua state will be created and all lua libraries will be loaded.

## Example

```cpp
LuaScript lua("path/to/the/lua/script.lua");
```

## Other links

- [Usage](../../../usage.MD)
- [LuaScript()](luascript0.MD)
- [LuaScript(LuaScript(std::size_t libs)](luascript2.MD)
- [LuaScript(const std::filesystem::path& path, std::size_t libs)](luascript3.MD)
- [~LuaScript()](luascript4.MD)
