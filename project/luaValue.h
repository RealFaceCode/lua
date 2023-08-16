#ifndef FUNC_DESC_VALUE_H
#define FUNC_DESC_VALUE_H

#include <memory>

/**
 * @brief Enumerates the possible value types for `FuncDescriptionValue`.
 */
enum class LuaValueType
{
    none = 0,           ///< No specific value type.
    integer = 1,        ///< Represents an integer value.
    boolean = 2,        ///< Represents a boolean value.
    float_number = 3,   ///< Represents a float value.
    double_number = 4,  ///< Represents a double value.
    string = 5,         ///< Represents a string value.
    table = 6
};

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