#ifndef FUNC_DESC_VALUE_H
#define FUNC_DESC_VALUE_H

/**
 * @brief Enumerates the possible value types for `FuncDescriptionValue`.
 */
enum class ValueType
{
    none = 0,           ///< No specific value type.
    integer = 1,        ///< Represents an integer value.
    boolean = 2,        ///< Represents a boolean value.
    float_number = 3,   ///< Represents a float value.
    double_number = 4,  ///< Represents a double value.
    string = 5          ///< Represents a string value.
};

/**
 * @brief A class that holds information about a value associated with a function description.
 */
class FuncDescriptionValue
{
public:
    void* mValuePtr;    ///< A pointer to the stored value.
    ValueType mValueType; ///< The type of the stored value.
};

#endif //FUNC_DESC_VALUE_H