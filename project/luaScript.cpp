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

LuaScript::LuaScript(std::size_t libs)
{  
    L = ::luaL_newstate();
    openLibs(libs);
}

LuaScript::LuaScript(const std::filesystem::path &path, std::size_t libs)
: mPath(path)
{
    L = ::luaL_newstate();
    openLibs(libs);
}

LuaScript::~LuaScript()
{
    ::lua_close(L);
}

FuncInfo LuaScript::regFunc(std::string_view funcName, FuncDescription& funcDesc)
{
    ::lua_getglobal(L, funcName.data());
    if(!mFuncDesc.contains(funcName) && !lua_isfunction(L, -1))
    {
        mFuncDesc.try_emplace(funcName.data(), &funcDesc);
        return FuncInfo(FuncInfoType::OK);
    }

    std::string errmsg;
    errmsg.append("Failed to register function[").append(funcName).append("] - Function name is already registred!");
    return FuncInfo(errmsg, FuncInfoType::REGISTER);
}

FuncInfo LuaScript::regFunc(std::string_view funcName, const FuncDescription& funcDesc)
{
::lua_getglobal(L, funcName.data());
    if(!mFuncDesc.contains(funcName) && !lua_isfunction(L, -1))
    {
        mFuncDesc.try_emplace(funcName.data(), const_cast<FuncDescription*>(&funcDesc));
        return FuncInfo(FuncInfoType::OK);
    }

    std::string errmsg;
    errmsg.append("Failed to register function[").append(funcName).append("] - Function name is already registred!");
    return FuncInfo(errmsg, FuncInfoType::REGISTER);
}

template<typename LuaCFunc>
FuncInfo LuaScript::regFunc(LuaCFunc func, std::string_view funcName, const FuncDescription& funcDesc)
{
    auto cFunPtr = func;
    static auto luaCFunc = [this, cFunPtr](lua_State const*)
    {
        return (*cFunPtr)(*this);
    };

    auto luaBaseFunc = Lambda::ptr<int, lua_CFunction>(luaCFunc);
    auto info = regFunc(funcName, funcDesc);

    if(info)
        lua_register(L, funcName.data(), luaBaseFunc);
    return info;
}

template FuncInfo LuaScript::regFunc<int(*)(LuaScript&)>(int(*)(LuaScript&), std::string_view, const FuncDescription&);

FuncInfo LuaScript::compile()
{
    using enum FuncInfoType;
    if(!std::filesystem::exists(mPath))
    {   
        std::string errmsg;
        errmsg.append("Failed to load lua script with path[").append(mPath.string()).append("] - ").append(std::strerror(errno));
        return FuncInfo(errmsg, COMPILE);
    }

    if(luaL_dofile(L, mPath.string().c_str()))
    {
        std::string errmsg;
        errmsg.append("Failed to compile lua script with path[").append(mPath.string()).append("] - ").append(lua_tostring(L, -1));
        return FuncInfo(errmsg, COMPILE);
    }
    return FuncInfo(OK);
}

FuncInfo LuaScript::compileString(std::string_view luaCode)
{
    using enum FuncInfoType;
    if(luaL_dostring(L, luaCode.data()))
    {
        std::string errmsg;
        errmsg.append("Failed to compile lua script with path[").append(mPath.string()).append("] - ").append(lua_tostring(L, -1));
        return FuncInfo(errmsg, COMPILE);
    }
    return FuncInfo(OK);
}

FuncInfo LuaScript::doFunc(std::string_view funcName)
{
    using enum FuncInfoType;
    ::lua_getglobal(L, funcName.data());

    FuncDescription* disc = nullptr;

    for(auto const& [key, value] : mFuncDesc)
    {
        if(key == funcName)
        {
            disc = value;
            break;
        }
    }

    if(!disc)
    {
        std::string errmsg;
        errmsg.append("Failed to run function[").append(funcName).append("] - no function with this name was registred");
        return FuncInfo(errmsg, RUN);
    }

    std::vector<LuaDescValue>& args = disc->getArgs();
    std::vector<LuaDescValueR>& retVals = disc->getRetVals();

    resolveArgs(args);

    if(lua_pcall(L, args.size(), retVals.size(), 0))
    {
        std::string errmsg;
        errmsg.append("Failed to run function[").append(funcName).append("] - ").append(lua_tostring(L, -1));
        return FuncInfo(errmsg, RUN);
    }

    resolveRets(retVals);
    
    lua_pop(L, retVals.size());

    return FuncInfo(OK);
}

std::string_view LuaScript::toString(int index)
{
    if(!::lua_isstring(L, index))
    {
        std::cout << "Failed to get string: " << ::lua_tostring(L, -1) << std::endl;
        return "";
    }
    return ::lua_tostring(L, index);
}

long long LuaScript::toInteger(int index)
{
    if(!::lua_isinteger(L, index))
    {
        std::cout << "Failed to get integer: " << ::lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return ::lua_tointeger(L, index);
}

int LuaScript::toBooleam(int index)
{
    if(!lua_isboolean(L, index))
    {
        std::cout << "Failed to get boolean: " << ::lua_tostring(L, -1) << std::endl;
        return 0;
    }
    return ::lua_toboolean(L, index);
}

double LuaScript::toNumber(int index)
{
    if(!::lua_isnumber(L, index))
    {
        std::cout << "Failed to get number: " << ::lua_tostring(L, -1) << std::endl;
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

void LuaScript::resolvePushTable(LuaTable &table, long long idx)
{
    if(!table.isIndexed())
    {
        auto size = static_cast<int>(table.size());
        ::lua_createtable(L, 0, size);
        while (!table.isEnd())
        {
            auto [name, value] = table.getNextValue();
            if(value.hasType<long long>())
            {
                ::lua_pushstring(L, name.data());
                ::lua_pushinteger(L,  value.retrieve<long long>());
                ::lua_settable(L, -3);
            }
            else if(value.hasType<double>())
            {
                ::lua_pushstring(L, name.data());
                ::lua_pushnumber(L,  value.retrieve<double>());
                ::lua_settable(L, -3);
            }
            else if(value.hasType<bool>())
            {
                ::lua_pushstring(L, name.data());
                ::lua_pushboolean(L,  value.retrieve<bool>());
                ::lua_settable(L, -3);
            }
            else if(value.hasType<std::string>())
            {
                ::lua_pushstring(L, name.data());
                ::lua_pushstring(L, value.retrieve<std::string>().data());
                ::lua_settable(L, -3);
            }
            else if(value.hasType<LuaTable>())
            {
                auto t = value.retrieve<LuaTable>();
                ::lua_pushstring(L, t.getName().data());
                resolvePushTable(t, 1);
                ::lua_settable(L, -3);
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        auto size = static_cast<int>(table.size());
        ::lua_createtable(L, size, 0);
        while (!table.isEnd())
        {
            auto [name, value] = table.getNextValue();
            if(value.hasType<long long>())
            {
                ::lua_pushinteger(L,  value.retrieve<long long>());
                ::lua_rawseti(L, -2, idx);
                idx++;
            }
            else if(value.hasType<double>())
            {
                ::lua_pushnumber(L,  value.retrieve<double>());
                ::lua_rawseti(L, -2, idx);
                idx++;
            }
            else if(value.hasType<bool>())
            {
                ::lua_pushboolean(L,  value.retrieve<bool>());
                ::lua_rawseti(L, -2, idx);
                idx++;
            }
            else if(value.hasType<std::string>())
            {
                ::lua_pushstring(L,  value.retrieve<std::string>().data());
                ::lua_rawseti(L, -2, idx);
                idx++;
            }
            else if(value.hasType<LuaTable>())
            {
                auto t = value.retrieve<LuaTable>();
                resolvePushTable(t, 1);
                ::lua_rawseti(L, -2, idx);
                idx++;
            }
            else
            {
                continue;
            }
        }
    }
}

void LuaScript::pushTable(LuaTable &table, long long idx)
{
    resolvePushTable(table, idx);
    ::lua_setglobal(L, table.getName().data());
}

LuaTable LuaScript::getTable(std::string_view name)
{
    int idx = -1;
    ::lua_getglobal(L, name.data());
    if (!lua_istable(L, idx))
    {
        return LuaTable();       
    }

    LuaTable table(name);

    resolveTable(table, idx);

    return table;
}

lua_State* LuaScript::getLuaState()
{
    return L;
}

void LuaScript::resolveTable(LuaTable &table, int idx)
{

#if LUA_VERSION_NUM > 501
    auto tableLen = ::lua_rawlen(L, idx);
#else
    auto tableLen = ::lua_objlen(L, idx);
#endif

    if(tableLen == 0)
        keyValueTable(table, idx);
    else
        indexedTable(table, idx, tableLen);
}

void LuaScript::keyValueTable(LuaTable &table, int idx)
{
    ::lua_pushnil(L);

    while(::lua_next(L, idx - 1) != 0)
    {
        std::string_view key;
        if(::lua_isstring(L, idx - 1))
        {
            key = lua_tostring(L, idx - 1);
            switch (const int type = ::lua_type(L, idx))
            {
            case LUA_TNUMBER:
            {
                ::lua_isinteger(L, idx) ? table.addValue<long long>(key, lua_tointeger(L, idx)) : table.addValue<double>(key, lua_tonumber(L, idx));
                break;
            }
            case LUA_TBOOLEAN:
            {
                table.addValue<bool>(key, ::lua_toboolean(L, idx));
                break;
            }
            case LUA_TSTRING:
            {
                table.addValue<std::string>(key, ::lua_tostring(L, idx));
                break;
            }
            case LUA_TTABLE:
            {
                LuaTable& newTable = table.addValue<LuaTable>(key, LuaTable(key)).retrieve<LuaTable>();
                resolveTable(newTable, idx);
                break;
            }
            default:
                break;
            }

            lua_pop(L, 1);
        }
    }
}

void LuaScript::indexedTable(LuaTable &table, int idx, unsigned long long tableLen)
{
    table.setIndexed();
    for(int i = 1; i <= tableLen; ++i)
    {
        ::lua_pushinteger(L, i);
        ::lua_gettable(L, idx - 1);

        switch (const int type = ::lua_type(L, idx))
        {
        case LUA_TNUMBER:
        {
            ::lua_isinteger(L, idx) ? table.addValue<long long>(lua_tointeger(L, idx)) : table.addValue<double>(lua_tonumber(L, idx));
            break;
        }
        case LUA_TBOOLEAN:
        {
            table.addValue<bool>(::lua_toboolean(L, idx));
            break;
        }
        case LUA_TSTRING:
        {
            table.addValue<std::string>(::lua_tostring(L, idx));
            break;
        }
        case LUA_TTABLE:
        {
            LuaTable& newTable = table.addValue<LuaTable>(LuaTable()).retrieve<LuaTable>();
            resolveTable(newTable, idx);
            break;
        }
        default:
            break;
        }

        lua_pop(L, 1);
    }
}

void LuaScript::openLibs(std::size_t libs)
{
    ::luaopen_base(L);
    if (libs & ::Lua_lib_package)
        ::luaL_requiref(L, "package", ::luaopen_package, 1);
    if (libs & ::Lua_lib_table)
        ::luaL_requiref(L, "table", ::luaopen_table, 1);
    if (libs & ::Lua_lib_string)
        ::luaL_requiref(L, "string", ::luaopen_string, 1);
    if (libs & ::Lua_lib_math)
        ::luaL_requiref(L, "math", ::luaopen_math, 1);
    if (libs & ::Lua_lib_debug)
        ::luaL_requiref(L, "debug", ::luaopen_debug, 1);
    if (libs & ::Lua_lib_io)
        ::luaL_requiref(L, "io", ::luaopen_io, 1);
    if (libs & ::Lua_lib_coroutine)
        ::luaL_requiref(L, "coroutine", ::luaopen_coroutine, 1);
    if (libs & ::Lua_lib_os)
        ::luaL_requiref(L, "os", ::luaopen_os, 1);
    if (libs & ::Lua_lib_utf8)
        ::luaL_requiref(L, "utf8", ::luaopen_utf8, 1);
}

void LuaScript::resolveArgs(std::vector<LuaDescValue>& args)
{
    for(auto& arg : args)
    {
        if(arg.hasType<long long>() && arg.hasValue())
        {
            auto t = arg.retrieve<long long>();
            ::lua_pushinteger(L,  t);
            continue;
        } 
        if(arg.hasType<bool>() && arg.hasValue())
        {
            ::lua_pushboolean(L,  arg.retrieve<bool>());
            continue;
        }
        if(arg.hasType<double>() && arg.hasValue())
        {
            ::lua_pushnumber(L,  arg.retrieve<double>());
            continue;
        }
        if(arg.hasType<std::string>() && arg.hasValue())
        {
            ::lua_pushstring(L,  arg.retrieve<std::string>().data());
            continue;
        }
        if(arg.hasType<LuaTable>() && arg.hasValue())
        {
            pushTable(arg.retrieve<LuaTable>());
            continue;
        }
    }
}

void LuaScript::resolveRets(std::vector<LuaDescValueR> &retVals)
{
    auto index = retVals.size();
    for(auto& retVal : retVals)
    {
        if(retVal.hasType<long long*>() && retVal.hasValue())
        {
            if(!::lua_isinteger(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was lua integer aka 'long long'");
            *retVal.retrieve<long long*>() = ::lua_tointeger(L, -static_cast<int>(index));
        } 
        else if(retVal.hasType<bool*>() && retVal.hasValue())
        {
            if(!lua_isboolean(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was boolean aka 'bool'");
            *retVal.retrieve<bool*>() = ::lua_toboolean(L, -static_cast<int>(index));
        }
        else if(retVal.hasType<double*>() && retVal.hasValue())
        {
            if(!::lua_isnumber(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was lua number aka 'double'");
            *retVal.retrieve<double*>() = lua_tonumber(L, -static_cast<int>(index));
        }
        else if(retVal.hasType<std::string*>() && retVal.hasValue())
        {
            if(!::lua_isstring(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was string'");
            *retVal.retrieve<std::string*>() = lua_tostring(L, -index);
        }
        else if(retVal.hasType<LuaTable*>() && retVal.hasValue())
        {
            if(!::lua_isstring(L, -static_cast<int>(index)))
                throw std::invalid_argument("Failed to get return value. Expected was string'");
            //TODO: implement
        }
        index--;
    }
}
