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

/**
 * @class LuaScript
 * @brief A class for managing Lua scripts and function interactions.
 */
class LuaScript 
{
private:
    std::unordered_map<std::string_view, void*> mUserPtr = {}; /**< Map of user data pointers. */
    std::unordered_map<std::string_view, FuncDescription> mFuncDesc = {}; /**< Map of function descriptions. */
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

    void pushTable(const LuaTable& table);
    
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
            mUserPtr[name] = value;
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
        if(mUserPtr.contains(name))
            return mUserPtr[name];
        throw std::invalid_argument("Failed to get user data");
    }

private:
    void resolveTable(LuaTable& table, int idx);
};

#endif // LUA_SCRIPT_H