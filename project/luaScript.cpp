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

    std::vector<LuaValue> const& args = mFuncDesc[funcName].getArgs();
    std::vector<LuaValue> const& retVals = mFuncDesc[funcName].getRetVals();

    for(auto const& arg : args)
    {
        switch (arg.getValueType())
        {
        using enum LuaValueType;
        case none:
            break;
        case integer:
            ::lua_pushinteger(L,  arg.getValue<long long>());
            break;
        case boolean:
            ::lua_pushboolean(L, arg.getValue<bool>());
            break;
        case number:
            ::lua_pushnumber(L, arg.getValue<double>());
            break;
        case string:
            ::lua_pushstring(L, arg.getValue<std::string>().c_str());
            break;
        case table:
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
        switch (retval.getValueType())
        {
        using enum LuaValueType;
        case none:
            break;
        case integer:
        {
            if(!::lua_isinteger(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was lua integer aka 'long long'");
            
            long long value = ::lua_tointeger(L, -static_cast<int>(index));
            long long& valueRef = retval.getValue<long long>();
            valueRef = value;
            break;
        }
        case boolean:
        {
            if(!lua_isboolean(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was boolean aka 'bool'");
            bool value = ::lua_toboolean(L, -static_cast<int>(index));
            bool& valueRef = retval.getValue<bool>();
            valueRef = value;
            break;
        }
        case number:
        {
            if(!::lua_isnumber(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was lua number aka 'double'");
            double value = lua_tonumber(L, -static_cast<int>(index));
            double& valueRef = retval.getValue<double>();
            valueRef = value;
            break;
        }
        case string:
        {
            if(!::lua_isstring(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was string'");
            std::string_view value = lua_tostring(L, -index);
            std::string_view& valueRef = retval.getValue<std::string_view>();
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