# LuaScript

Main class for handling with lua scripts.

## Example

### Compile and run script

Include the `luaScript.h` header.

```cpp
#include "luaScript.h"
```

Create a new `LuaScript` class with the path of your .lua script.

```cpp
LuaScript lua("your/path/to/the/lua/script.lua");
```

Compile your lua script.

```cpp
lua.compile();
```

### Run functions from script

#### Lua defined

Define the function in your lua script file.

```lua
function hello(delta)
    print("hello")
end
```

Include the `luaScript.h` header.

```cpp
#include "luaScript.h"
```

Create a new `LuaScript` class with the path of your .lua script.

```cpp
LuaScript lua("your/path/to/the/lua/script.lua");
```

Register the lua defined function.

```cpp
lua.regFunc("hello");
```

Compile your lua script.

```cpp
lua.compile();
```

Call the lua function from C++.

```cpp
lua.doFunc("hello");
```

#### C++ defined

Define the C++ function.

```cpp
int log(LuaScript& L)
{   
    auto str = L.toString(1);
    std::cout << "log from lua: " << str << std::endl;
    return L.getRetValCount();
}
```

Include the `luaScript.h` header.

```cpp
#include "luaScript.h"
```

Create a new `LuaScript` class with the path of your .lua script.

```cpp
LuaScript lua("your/path/to/the/lua/script.lua");
```

Register the lua defined function.

```cpp
lua.regFunc(::log, "log");
```

Compile your lua script.

```cpp
lua.compile();
```

Call the c++ function from your lua script.

```lua
log("this is called in lua")
```

## Functions

### public

| Function                                  | Link                               |
| ----------------------------------------  | ---------------------------------- |
| `LuaScript();` | [Link to functions doc](funcs/luascript/luascript0.MD) |
| `explicit LuaScript(const std::filesystem::path& path);` | [Link to functions doc](funcs/luascript/luascript1.MD) |
| `explicit LuaScript(std::size_t libs);` | [Link to functions doc](funcs/luascript/luascript2.MD) |
| `explicit LuaScript(const std::filesystem::path& path, std::size_t libs);` | [Link to functions doc](funcs/luascript/luascript3.MD) |
| `~LuaScript();` | [Link to functions doc](funcs/luascript/luascript4.MD) |
| `void regFunc(std::string_view funcName, const FuncDescription& funcDesc);` | [Link to functions doc](funcs/luascript/regfunc0.MD) |
| `void regFunc(std::function<int(LuaScript&)> func, std::string_view funcName, const FuncDescription& funcDesc);` | [Link to functions doc](funcs/luascript/regfunc1.MD) |
| `void compile();` | [Link to functions doc](funcs/luascript/compile.MD) |
| `void compileString(std::string_view luaCode);` | [Link to functions doc](funcs/luascript/Compilestring.MD) |
| `void doFunc(std::string_view funcName);` | [Link to functions doc](funcs/luascript/dofunc.MD) |
| `std::string_view toString(int index);` | [Link to functions doc](funcs/luascript/tostring.MD) |
| `long long toInteger(int index);` | [Link to functions doc](funcs/luascript/tointeger.MD) |
| `int toBooleam(int index);` | [Link to functions doc](funcs/luascript/toboolean.MD) |
| `double toNumber(int index);` | [Link to functions doc](funcs/luascript/tonumber.MD) |
| `void pushString(std::string_view string);` | [Link to functions doc](funcs/luascript/pushstring.MD) |
| `void pushInteger(int integer);` | [Link to functions doc](funcs/luascript/pushInteger.MD) |
| `void pushBoolean(bool boolean);` | [Link to functions doc](funcs/luascript/pushboolean.MD) |
| `void pushNumber(double number);` | [Link to functions doc](funcs/luascript/pushnumber.MD) |
| `int getRetValCount();` | [Link to functions doc](funcs/luascript/getretvalcount.MD) |
| `void pushTable(LuaTable& table, long long idx);` | [Link to functions doc](funcs/luascript/pushtable.MD) |
| `LuaTable getTable(std::string_view name);` | [Link to functions doc](funcs/luascript/gettable.MD) |
| `lua_State* getLuaState();` | [Link to functions doc](funcs/luascript/getluastate.MD) |
| `template<typename TYPE> void addUserPtr(std::string_view name, TYPE& value);` | [Link to functions doc](funcs/luascript/adduserptr.MD) |
| `template<typename TYPE> TYPE& getUserPtr(std::string_view name);` | [Link to functions doc](funcs/luascript/getuserptr.MD) |

### private

| Function                                  | Link                               |
| ----------------------------------------  | ---------------------------------- |
| `void resolveTable(LuaTable& table, int idx);` | [Link to functions doc](funcs/luascript/resolvetable.MD) |
| `void resolvePushTable(LuaTable &table, long long idx);;` | [Link to functions doc](funcs/luascript/resolvepushtable.MD) |
| `void keyValueTable(LuaTable& table, int idx);` | [Link to functions doc](funcs/luascript/keyvaluetable.MD) |
| `void indexedTable(LuaTable& table, int idx, unsigned long long tableLen);` | [Link to functions doc](funcs/luascript/indexedvaluetable.MD) |
| `void openLibs(std::size_t libs);` | [Link to functions doc](funcs/luascript/openLibs.MD) |

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

## includes

### C++

```cpp
#include <iostream>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <cstring>
```

### Libs

```cpp
#include <lua.hpp>
```

### Lua script manager

```cpp
#include "luaValueType.h"
#include "luaValue.h"
#include "funcDesc.h"
#include "lambda.h"
#include "luaTable.h"
#include "util.h"
```

## Other links

- [Usage](../usage.MD)
