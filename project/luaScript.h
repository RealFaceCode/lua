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


class LuaScript
{
    private:
    std::map<std::string, void*> sUserPtr;
    std::map<std::string, FuncDescription> mFuncDesc;
    lua_State* L;
    std::filesystem::path mPath;
    int mRetValCount;

public:
    LuaScript();
    LuaScript(const std::filesystem::path& path);
    ~LuaScript();

    void regFunc(const std::string& funcName, const FuncDescription& funcDesc = FuncDescription());
    void regFunc(std::function<int(LuaScript&)> func, const std::string& funcName, const FuncDescription& funcDesc = FuncDescription());

    void compile();
    void compileString(const std::string& luaCode);

    void doFunc(const std::string& funcName);

    std::string toString(const int& index);
    int toInteger(const int& index);
    int toBooleam(const int& index);
    double toNumber(const int& index);

    void pushString(const std::string& string);
    void pushInteger(const int& integer);
    void pushBoolean(const bool& boolean);
    void pushNumber(const double& number);

    int getRetValCount();
    lua_State* getLuaState();

    template<typename TYPE>
    void addUserPtr(const std::string& name, TYPE& value)
    {
        if(!sUserPtr.contains(name))
        {
            sUserPtr[name] = &value;
        }
    }

    template<typename TYPE>
    TYPE& getUserPtr(const std::string& name)
    {
        if(sUserPtr.contains(name))
        {
            return *static_cast<TYPE*>(sUserPtr[name]);
        }
        throw std::runtime_error("No Userdata with name" + name);
    }
};

#endif // LUA_SCRIPT_H