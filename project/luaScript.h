#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <filesystem>
#include <cstring>

#include "funcDiscValue.h"
#include "funcDesc.h"
#include "lambda.h"


/**
 * @class LuaScript
 * @brief A class for managing Lua scripts and function interactions.
 */
class LuaScript 
{
private:
    std::map<std::string, void*> sUserPtr; /**< Map of user data pointers. */
    std::map<std::string, FuncDescription> mFuncDesc; /**< Map of function descriptions. */
    lua_State* L; /**< Lua state instance. */
    std::filesystem::path mPath; /**< Path to the Lua script. */
    int mRetValCount; /**< Return value count for Lua function calls. */

public:
    /**
     * @brief Default constructor. Initializes Lua state.
     */
    LuaScript();

    /**
     * @brief Constructor with script path. Initializes Lua state and loads standard libraries.
     * @param path Path to the Lua script file.
     */
    LuaScript(const std::filesystem::path& path);

    /**
     * @brief Destructor. Closes the Lua state.
     */
    ~LuaScript();

    /**
     * @brief Registers a Lua function with the given name and optional function description.
     * @param funcName Name of the Lua function.
     * @param funcDesc Function description (optional).
     */
    void regFunc(const std::string& funcName, const FuncDescription& funcDesc = FuncDescription());

    /**
     * @brief Registers a C++ function as a Lua function with the given name and optional function description.
     * @param func C++ function to register.
     * @param funcName Name of the Lua function.
     * @param funcDesc Function description (optional).
     */
    void regFunc(std::function<int(LuaScript&)> func, const std::string& funcName, const FuncDescription& funcDesc = FuncDescription());

    /**
     * @brief Compiles and executes the Lua script loaded from the specified file path.
     */
    void compile();

    /**
     * @brief Compiles and executes the given Lua code string.
     * @param luaCode Lua code string to compile and execute.
     */
    void compileString(const std::string& luaCode);

    /**
     * @brief Calls a Lua function with the given name.
     * @param funcName Name of the Lua function to call.
     */
    void doFunc(const std::string& funcName);

    /**
     * @brief Converts a Lua value at the specified index to a string.
     * @param index Index of the Lua value on the stack.
     * @return Converted string value.
     */
    std::string toString(const int& index);

    /**
     * @brief Converts a Lua value at the specified index to an integer.
     * @param index Index of the Lua value on the stack.
     * @return Converted integer value.
     */
    int toInteger(const int& index);

    /**
     * @brief Converts a Lua value at the specified index to a boolean.
     * @param index Index of the Lua value on the stack.
     * @return Converted boolean value.
     */
    int toBooleam(const int& index);

    /**
     * @brief Converts a Lua value at the specified index to a double.
     * @param index Index of the Lua value on the stack.
     * @return Converted double value.
     */
    double toNumber(const int& index);

    /**
     * @brief Pushes a string onto the Lua stack.
     * @param string String value to push onto the stack.
     */
    void pushString(const std::string& string);

    /**
     * @brief Pushes an integer onto the Lua stack.
     * @param integer Integer value to push onto the stack.
     */
    void pushInteger(const int& integer);

    /**
     * @brief Pushes a boolean onto the Lua stack.
     * @param boolean Boolean value to push onto the stack.
     */
    void pushBoolean(const bool& boolean);

    /**
     * @brief Pushes a double onto the Lua stack.
     * @param number Double value to push onto the stack.
     */
    void pushNumber(const double& number);

    /**
     * @brief Retrieves the number of return values and resets the count.
     * @return Number of return values.
     */
    int getRetValCount();

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
    void addUserPtr(const std::string& name, TYPE& value);

    /**
     * @brief Retrieves a user-defined data pointer.
     * @tparam TYPE Type of the user data.
     * @param name Name of the user data.
     * @return Reference to the user data value.
     * @throws std::runtime_error if the user data with the given name is not found.
     */
    template<typename TYPE>
    TYPE& getUserPtr(const std::string& name);
};

#endif // LUA_SCRIPT_H