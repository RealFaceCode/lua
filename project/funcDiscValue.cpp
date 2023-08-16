#include "funcDiscValue.h"

void LuaValue::setType(LuaValueType type)
{
    mValueType = type;
}

LuaValueType LuaValue::getValueType() const
{
    return mValueType;
}
