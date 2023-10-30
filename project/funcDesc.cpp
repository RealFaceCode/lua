#include "funcDesc.h"

std::vector<LuaDescValue> &FuncDescription::getArgs()
{
    return mArgs;
}

std::vector<LuaDescValueR> &FuncDescription::getRetVals()
{
    return mRetVals;
}

template<typename Type>
void FuncDescription::addArg(const Type& value)
{
    mArgs.emplace_back(value);
}


template<typename Type>
void FuncDescription::addRetVal(Type& value)
{
    mRetVals.emplace_back(&value);
}

template void FuncDescription::addArg<long long>(const long long&);
template void FuncDescription::addArg<double>(const double&);
template void FuncDescription::addArg<bool>(const bool&);
template void FuncDescription::addArg<std::string>(const std::string&);
template void FuncDescription::addArg<LuaTable>(const LuaTable&);

template void FuncDescription::addRetVal<long long>(long long&);
template void FuncDescription::addRetVal<double>(double&);
template void FuncDescription::addRetVal<bool>(bool&);
template void FuncDescription::addRetVal<std::string>(std::string&);
template void FuncDescription::addRetVal<LuaTable>(LuaTable&);