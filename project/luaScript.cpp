#include "luaScript.h"

LuaScript::LuaScript()
{
    L = ::luaL_newstate();
    ::luaL_openlibs(L);
}

LuaScript::LuaScript(const std::filesystem::path& path)
: mPath(path)
{
    L = ::luaL_newstate();
    ::luaL_openlibs(L);
}

LuaScript::~LuaScript()
{
    ::lua_close(L);
}

void LuaScript::regFunc(std::string_view funcName, const FuncDescription& funcDesc)
{
    ::lua_getglobal(L, funcName.data());
    if(!mFuncDesc.contains(funcName) && !lua_isfunction(L, -1))
        mFuncDesc[funcName] = funcDesc;
}

void LuaScript::regFunc(std::function<int(LuaScript&)> func, std::string_view funcName, const FuncDescription& funcDesc)
{
    auto cFunPtr = func.target<int(*)(LuaScript&)>();
    static auto luaCFunc = [this, cFunPtr](lua_State const*)
    {
        return (*cFunPtr)(*this);
    };

    auto luaBaseFunc = Lambda::ptr<int, lua_CFunction>(luaCFunc);

    lua_register(L, funcName.data(), luaBaseFunc);
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

void LuaScript::compileString(std::string_view luaCode)
{
    if(luaL_dostring(L, luaCode.data()))
    {
        std::cerr << "Failed to load lua script with path[" << mPath << "] - " << lua_tostring(L, -1) << std::endl;
    }
}

void LuaScript::doFunc(std::string_view funcName)
{
    ::lua_getglobal(L, funcName.data());

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
        using enum ValueType;
        case none:
            break;
        case integer:
            ::lua_pushinteger(L, *static_cast<int*>(arg.mValuePtr));
            break;
        case boolean:
            ::lua_pushboolean(L, *static_cast<bool*>(arg.mValuePtr));
            break;
        case float_number:
            ::lua_pushnumber(L, *static_cast<lua_Number*>(arg.mValuePtr));
            break;
            case double_number:
            ::lua_pushnumber(L, *static_cast<lua_Number*>(arg.mValuePtr));
            break;
        case string:
            ::lua_pushstring(L, static_cast<std::string*>(arg.mValuePtr)->c_str());
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

    auto index = retVals.size();
    for(auto& retval : retVals)
    {
        switch (retval.mValueType)
        {
        case ValueType::none:
            break;
        case ValueType::integer:
        {
            auto value = ::lua_tointeger(L, -static_cast<int>(index));
            long long& valueRef = *static_cast<long long*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::boolean:
        {
            bool value = ::lua_toboolean(L, -static_cast<int>(index));
            bool& valueRef = *static_cast<bool*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::float_number:
        {
            double value = ::lua_tonumber(L, -static_cast<int>(index));
            float& valueRef = *static_cast<float*>(retval.mValuePtr);
            valueRef = static_cast<float>(value);
            break;
        }
        case ValueType::double_number:
        {
            double value = lua_tonumber(L, -static_cast<int>(index));
            double& valueRef = *static_cast<double*>(retval.mValuePtr);
            valueRef = value;
            break;
        }
        case ValueType::string:
        {
            std::string_view value = lua_tostring(L, -index);
            std::string_view& valueRef = *static_cast<std::string_view*>(retval.mValuePtr);
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

std::string_view LuaScript::toString(int index)
{
    if(!::lua_isstring(L, index))
    {
        std::cerr << "Failed to get string: " << ::lua_tostring(L, -1) << std::endl;
        return "";
    }
    return ::lua_tostring(L, index);
}

long long LuaScript::toInteger(int index)
{
    if(!::lua_isinteger(L, index))
    {
        std::cerr << "Failed to get integer: " << ::lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return ::lua_tointeger(L, index);
}

int LuaScript::toBooleam(int index)
{
    if(!lua_isboolean(L, index))
    {
        std::cerr << "Failed to get boolean: " << ::lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return ::lua_toboolean(L, index);
}

double LuaScript::toNumber(int index)
{
    if(!::lua_isnumber(L, index))
    {
        std::cerr << "Failed to get number: " << ::lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return ::lua_tonumber(L, index);
}

void LuaScript::pushString(std::string_view string)
{
    ::lua_pushstring(L, string.data());
    mRetValCount++;
}

void LuaScript::pushInteger(int integer)
{
    ::lua_pushinteger(L, integer);
    mRetValCount++;
}

void LuaScript::pushBoolean(bool boolean)
{
    ::lua_pushinteger(L, boolean);
    mRetValCount++;
}

void LuaScript::pushNumber(double number)
{
    ::lua_pushnumber(L, number);
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
