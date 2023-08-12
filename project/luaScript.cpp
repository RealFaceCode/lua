#include "luaScript.h"

LuaScript::LuaScript()
: sUserPtr(), L(luaL_newstate()), mFuncDesc(), mPath(""), mRetValCount(0)
{}

LuaScript::LuaScript(const std::filesystem::path& path)
: sUserPtr(), L(luaL_newstate()), mFuncDesc(), mPath(path), mRetValCount(0)
{
    luaL_openlibs(L);
}

LuaScript::~LuaScript()
{
    lua_close(L);
}

void LuaScript::regFunc(const std::string& funcName, const FuncDescription& funcDesc)
{
    if(!mFuncDesc.contains(funcName))
    mFuncDesc[funcName] = funcDesc;
}

void LuaScript::regFunc(std::function<int(LuaScript&)> func, const std::string& funcName, const FuncDescription& funcDesc)
{
    auto cFunPtr = func.target<int(*)(LuaScript&)>();
    static auto luaCFunc = [this, cFunPtr](lua_State* L) -> int {
        return (*cFunPtr)(*this);
    };

    auto luaBaseFunc = Lambda::ptr<int, lua_CFunction>(luaCFunc);

    lua_register(L, funcName.c_str(), luaBaseFunc);
    regFunc(funcName, funcDesc);
}

void LuaScript::compile()
{
    if(!std::filesystem::exists(mPath))
    {
        std::cerr << "Failed to load lua script with path[" << mPath << "] - " << std::strerror(errno) << std::endl;
    }

    if(luaL_dofile(L, mPath.string().c_str()))
    {
        std::cerr << "Failed to load lua script with path[" << mPath << "] - " << lua_tostring(L, -1) << std::endl;
    }
}

void LuaScript::compileString(const std::string& luaCode)
{
    if(luaL_dostring(L, luaCode.c_str()))
    {
        std::cerr << "Failed to load lua script with path[" << mPath << "] - " << lua_tostring(L, -1) << std::endl;
    }
}

void LuaScript::doFunc(const std::string& funcName)
{
    lua_getglobal(L, funcName.c_str());

    if(!mFuncDesc.contains(funcName))
    {
        std::cerr << "Failed to run function[" << funcName << "] - no function with this name was registred" << std::endl;
        return;
    }

    std::vector<FuncDescriptionValue>& args = mFuncDesc[funcName].mArgs;
    std::vector<FuncDescriptionValue>& retVals = mFuncDesc[funcName].mRetVals;

    for(auto& arg : args)
    {
        switch (arg.mValueType)
        {
        case ValueType::none:
            break;
        case ValueType::integer:
            lua_pushinteger(L, *static_cast<int*>(arg.mValuePtr));
            break;
        case ValueType::boolean:
            lua_pushboolean(L, *static_cast<bool*>(arg.mValuePtr));
            break;
        case ValueType::float_number:
            lua_pushnumber(L, *static_cast<lua_Number*>(arg.mValuePtr));
            break;
            case ValueType::double_number:
            lua_pushnumber(L, *static_cast<lua_Number*>(arg.mValuePtr));
            break;
        case ValueType::string:
            lua_pushstring(L, static_cast<std::string*>(arg.mValuePtr)->c_str());
            break;
        default:
            break;
        }
    }

    if(lua_pcall(L, args.size(), retVals.size(), 0))
    {
        std::cerr << "Failed to run function[" << funcName << "] - " << lua_tostring(L, -1)<< std::endl;
        return;
    }

    int index = retVals.size();
    for(auto& retval : retVals)
    {
        switch (retval.mValueType)
        {
        case ValueType::none:
            break;
        case ValueType::integer:
        {
            int value = lua_tointeger(L, -index);
            int& valueRef = *static_cast<int*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::boolean:
        {
            bool value = lua_toboolean(L, -index);
            bool& valueRef = *static_cast<bool*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::float_number:
        {
            float value = lua_tonumber(L, -index);
            float& valueRef = *static_cast<float*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::double_number:
        {
            double value = lua_tonumber(L, -index);
            double& valueRef = *static_cast<double*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::string:
        {
            std::string value = lua_tostring(L, -index);
            std::string& valueRef = *static_cast<std::string*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        default:
            break;
        }
        index--;
    }
    lua_pop(L, retVals.size());
}

std::string LuaScript::toString(const int& index)
{
    if(!lua_isstring(L, index))
    {
        std::cerr << "Failed to get string: " << lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return lua_tostring(L, index);
}

int LuaScript::toInteger(const int& index)
{
    if(!lua_isinteger(L, index))
    {
        std::cerr << "Failed to get integer: " << lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return lua_tointeger(L, index);
}

int LuaScript::toBooleam(const int& index)
{
    if(!lua_isboolean(L, index))
    {
        std::cerr << "Failed to get boolean: " << lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return lua_toboolean(L, index);
}

double LuaScript::toNumber(const int& index)
{
    if(!lua_isnumber(L, index))
    {
        std::cerr << "Failed to get number: " << lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return lua_tonumber(L, index);
}

void LuaScript::pushString(const std::string& string)
{
    lua_pushstring(L, string.c_str());
    mRetValCount++;
}

void LuaScript::pushInteger(const int& integer)
{
    lua_pushinteger(L, integer);
    mRetValCount++;
}

void LuaScript::pushBoolean(const bool& boolean)
{
    lua_pushinteger(L, boolean);
    mRetValCount++;
}

void LuaScript::pushNumber(const double& number)
{
    lua_pushinteger(L, number);
    mRetValCount++;
}

int LuaScript::getRetValCount()
{
    int r = mRetValCount;
    mRetValCount = 0;
    return r;
}

lua_State* LuaScript::getLuaState()
{
    return L;
}