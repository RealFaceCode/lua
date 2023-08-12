#ifndef FUNC_DESC_VALUE_H
#define FUNC_DESC_VALUE_H

enum class ValueType
{
    none = 0,
    integer = 1,
    boolean = 2,
    float_number = 3,
    double_number = 4,
    string = 5
};

class FuncDescriptionValue
{
public:
    void* mValuePtr;
    ValueType mValueType;
};

#endif //FUNC_DESC_VALUE_H