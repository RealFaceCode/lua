#include "funcDesc.h"

const std::vector<LuaValue> &FuncDescription::getArgs() const
{
    return mArgs;
}

const std::vector<LuaValue> &FuncDescription::getRetVals() const
{
    return mRetVals;
}
