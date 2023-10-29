#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <cstring>

#include "luaValueType.h"
#include "luaValue.h"
#include "funcDesc.h"
#include "lambda.h"
#include "luaTable.h"
#include "util.h"

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

/**
 * @class LuaScript
 * @brief A class for managing Lua scripts and function interactions.
 */
class LuaScript 
{
private:
    std::unordered_map<std::string, void*, TransparentHash, TransparentEqual> mUserPtr = {}; /**< Map of user data pointers. */
    std::unordered_map<std::string, FuncDescription, TransparentHash, TransparentEqual> mFuncDesc = {}; /**< Map of function descriptions. */
    lua_State* L = nullptr; /**< Lua state instance. */
    std::filesystem::path mPath = ""; /**< Path to the Lua script. */
    int mRetValCount = 0; /**< Return value count for Lua function calls. */

public:
    /**
     * @brief Default constructor. Initializes Lua state.
     */
    LuaScript();

    /**
     * @brief Constructor with script path. Initializes Lua state and loads standard libraries.
     * @param path Path to the Lua script file.
     */
    explicit LuaScript(const std::filesystem::path& path);

/**
     * @brief Constructor with bitmask libraries to open. Initializes Lua state and loads the libraries.
     * @param libs Bitmask to open the lua libraries.
     */
    explicit LuaScript(std::size_t libs);

    /**
     * @brief Constructor with script path and bitmask libraries to open. Initializes Lua state and loads the libraries.
     * @param path Path to the Lua script file.
     * @param libs Bitmask to open the lua libraries.
     */
    explicit LuaScript(const std::filesystem::path& path, std::size_t libs);

    /**
     * @brief Destructor. Closes the Lua state.
     */
    ~LuaScript();

    /**
     * @brief Registers a Lua function with the given name and optional function description.
     * @param funcName Name of the Lua function.
     * @param funcDesc Function description (optional).
     */
    void regFunc(std::string_view funcName, const FuncDescription& funcDesc = FuncDescription());

    /**
     * @brief Registers a C++ function as a Lua function with the given name and optional function description.
     * @param func C++ function to register.
     * @param funcName Name of the Lua function.
     * @param funcDesc Function description (optional).
     */
    void regFunc(std::function<int(LuaScript&)> func, std::string_view funcName, const FuncDescription& funcDesc = FuncDescription());

    /**
     * @brief Compiles and executes the Lua script loaded from the specified file path.
     */
    void compile();

    /**
     * @brief Compiles and executes the given Lua code string.
     * @param luaCode Lua code string to compile and execute.
     */
    void compileString(std::string_view luaCode);

    /**
     * @brief Calls a Lua function with the given name.
     * @param funcName Name of the Lua function to call.
     */
    void doFunc(std::string_view funcName);

    /**
     * @brief Converts a Lua value at the specified index to a string.
     * @param index Index of the Lua value on the stack.
     * @return Converted string value.
     */
    std::string_view toString(int index);

    /**
     * @brief Converts a Lua value at the specified index to an integer.
     * @param index Index of the Lua value on the stack.
     * @return Converted integer value.
     */
    long long toInteger(int index);

    /**
     * @brief Converts a Lua value at the specified index to a boolean.
     * @param index Index of the Lua value on the stack.
     * @return Converted boolean value.
     */
    int toBooleam(int index);

    /**
     * @brief Converts a Lua value at the specified index to a double.
     * @param index Index of the Lua value on the stack.
     * @return Converted double value.
     */
    double toNumber(int index);

    /**
     * @brief Pushes a string onto the Lua stack.
     * @param string String value to push onto the stack.
     */
    void pushString(std::string_view string);

    /**
     * @brief Pushes an integer onto the Lua stack.
     * @param integer Integer value to push onto the stack.
     */
    void pushInteger(int integer);

    /**
     * @brief Pushes a boolean onto the Lua stack.
     * @param boolean Boolean value to push onto the stack.
     */
    void pushBoolean(bool boolean);

    /**
     * @brief Pushes a double onto the Lua stack.
     * @param number Double value to push onto the stack.
     */
    void pushNumber(double number);

    /**
     * @brief Retrieves the number of return values and resets the count.
     * @return Number of return values.
     */
    int getRetValCount();

    void pushTable(LuaTable& table, long long idx = 1);
    
    LuaTable getTable(std::string_view name);

    /**
     * @brief Retrieves the Lua state associated with the LuaScript instance.
     * @return Pointer to the Lua state.
     */
    lua_State* getLuaState();

    /**
     * @brief Adds a user-defined data pointer.
     * @tparam TYPE Type of the user data.
     * @param name Name of the user data.
     * @param value Reference to the user data value.
     */
    template<typename TYPE>
    void addUserPtr(std::string_view name, TYPE& value)
    {
        if(!mUserPtr.contains(name))
            mUserPtr[name.data()] = value;
    }

    /**
     * @brief Retrieves a user-defined data pointer.
     * @tparam TYPE Type of the user data.
     * @param name Name of the user data.
     * @return Reference to the user data value.
     * @throws std::runtime_error if the user data with the given name is not found.
     */
    template<typename TYPE>
    TYPE& getUserPtr(std::string_view name)
    {
        for (auto iter = mUserPtr.begin; iter != mUserPtr.end(); iter++)
        {
            if(iter->first == name)
            {
                return iter->second;
            }
        }
        throw std::invalid_argument("Failed to get user data");
    }

private:
    void resolveTable(LuaTable& table, int idx);
    void resolvePushTable(LuaTable &table, long long idx);
    void keyValueTable(LuaTable& table, int idx);
    void indexedTable(LuaTable& table, int idx, unsigned long long tableLen);
    void openLibs(std::size_t libs);
};

#endif // LUA_SCRIPT_H