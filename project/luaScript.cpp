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

void LuaScript::regFunc(std::string_view funcName, const FuncDescription& funcDesc)
{
    ::lua_getglobal(L, funcName.data());
    if(!mFuncDesc.contains(funcName) && !lua_isfunction(L, -1))
        mFuncDesc[funcName.data()] = funcDesc;
}

template<typename LuaCFunc>
void LuaScript::regFunc(LuaCFunc func, std::string_view funcName, const FuncDescription& funcDesc)
{
    auto cFunPtr = func;
    static auto luaCFunc = [this, cFunPtr](lua_State const*)
    {
        return (*cFunPtr)(*this);
    };

    auto luaBaseFunc = Lambda::ptr<int, lua_CFunction>(luaCFunc);

    lua_register(L, funcName.data(), luaBaseFunc);
    regFunc(funcName, funcDesc);
}

template void LuaScript::regFunc<int(*)(LuaScript&)>(int(*)(LuaScript&), std::string_view, const FuncDescription&);

void LuaScript::compile()
{
    if(!std::filesystem::exists(mPath))
    {
        std::cout << "Failed to load lua script with path[" << mPath << "] - " << std::strerror(errno) << std::endl;
        return; //TODO: add error handling
    }

    if(luaL_dofile(L, mPath.string().c_str()))
    {
        std::cout << "Failed to load lua script with path[" << mPath << "] - " << lua_tostring(L, -1) << std::endl;
        return; //TODO: add error handling
    }
    return; //TODO: add error handling
}

void LuaScript::compileString(std::string_view luaCode)
{
    if(luaL_dostring(L, luaCode.data()))
    {
        std::cout << "Failed to load lua script with path[" << mPath << "] - " << lua_tostring(L, -1) << std::endl;
        return; //TODO: add error handling
    }
    return; //TODO: add error handling
}

void LuaScript::doFunc(std::string_view funcName)
{
    ::lua_getglobal(L, funcName.data());

    auto iter = mFuncDesc.begin(); 
    const FuncDescription* disc = nullptr;

    for (; iter != mFuncDesc.end(); iter++)
    {
        if(iter->first == funcName)
        {
            disc = &iter->second;
            break;
        }
    }

    if(!disc)
    {
        std::cout << "Failed to run function[" << funcName << "] - no function with this name was registred" << std::endl;
        return; //TODO: add error handling
    }

    std::vector<LuaValue> const& args = disc->getArgs();
    std::vector<LuaValue> const& retVals = disc->getRetVals();

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
        std::cout << "Failed to run function[" << funcName << "] - " << lua_tostring(L, -1)<< std::endl;
        return; //TODO: add error handling
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

    return; //TODO: add error handling
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
            else if(value.hasType<std::string_view>())
            {
                ::lua_pushstring(L, name.data());
                ::lua_pushstring(L, value.retrieve<std::string_view>().data());
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
            else if(value.hasType<std::string_view>())
            {
                ::lua_pushstring(L,  value.retrieve<std::string_view>().data());
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
                table.addValue<std::string_view>(key, ::lua_tostring(L, idx));
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
            table.addValue<std::string_view>(::lua_tostring(L, idx));
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
