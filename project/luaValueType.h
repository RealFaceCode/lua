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
    number = 3,         ///< Represents a double value.
    string = 4,         ///< Represents a string value.
    table = 5
};

#endif // LUA_VALUE_TYPE_H