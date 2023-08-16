#ifndef FUNC_DESC_VALUE_H
#define FUNC_DESC_VALUE_H

#include <memory>
#include "luaValueType.h"

/**
 * @brief A class that holds information about a value associated with a function description.
 */
class LuaValue
{
private:
    void* mValuePtr = nullptr;                      ///< A pointer to the stored value.
    LuaValueType mValueType = LuaValueType::none;   ///< The type of the stored value.
public:
    LuaValue() = default;

    template<typename T>
    LuaValue(const T& value, LuaValueType type)
    {
        setValue(value);
        setType(type);
    }

    template<typename T>
    void setValue(const T& value)
    {
        mValuePtr = (void*)&value;
    }

    template<typename T>
    T& getValue() const
    {
        return *static_cast<T*>(mValuePtr);
    }

    void setType(LuaValueType type);
    LuaValueType getValueType() const;
};

#endif //FUNC_DESC_VALUE_H