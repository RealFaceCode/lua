#include "funcDesc.h"

std::vector<FuncDescriptionValue> &FuncDescription::getArgs()
{
    return mArgs;
}

std::vector<FuncDescriptionValue> &FuncDescription::getRetVals()
{
    return mRetVals;
}
