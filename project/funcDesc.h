#ifndef FUNC_DESC_H
#define FUNC_DESC_H

#include <vector>
#include "funcDiscValue.h"

class FuncDescription
{
public:
    std::vector<FuncDescriptionValue> mArgs;
    std::vector<FuncDescriptionValue> mRetVals;

public:
    FuncDescription() = default;
    ~FuncDescription() = default;

    template<typename TYPE>
    void addArg(const TYPE& value, const ValueType& type)
    {
        mArgs.emplace_back(FuncDescriptionValue{.mValuePtr = (void*)&value, .mValueType = type});
    }

    template<typename TYPE>
    void addRetVal(const TYPE& value, const ValueType& type)
    {
        mRetVals.emplace_back(FuncDescriptionValue{.mValuePtr = (void*)&value, .mValueType = type});
    }
};

#endif // FUNC_DESC_H