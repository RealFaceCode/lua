#ifndef LUA_VALUE_TYPE_H
#define LUA_VALUE_TYPE_H

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

#endif // LUA_VALUE_TYPE_H