#ifndef FUNC_DESC_H
#define FUNC_DESC_H

#include <vector>
#include "funcDiscValue.h"

/**
 * @brief A class that holds descriptions of function arguments and return values.
 */
class FuncDescription
{
private:
    /**
     * @brief A vector containing descriptions of function arguments.
     */
    std::vector<LuaValue> mArgs;

    /**
     * @brief A vector containing descriptions of function return values.
     */
    std::vector<LuaValue> mRetVals;

public:
    /**
     * @brief Default constructor for the FuncDescription class.
     */
    FuncDescription() = default;

    /**
     * @brief Default destructor for the FuncDescription class.
     */
    ~FuncDescription() = default;

    //TODO: add comment
    const std::vector<LuaValue>& getArgs() const;
    const std::vector<LuaValue>& getRetVals() const;
    
    /**
     * @brief Adds a description for a function argument.
     * 
     * @tparam TYPE The type of the argument value.
     * @param value The value of the argument.
     * @param type The ValueType representing the type of the argument.
     */
    template<typename TYPE>
    void addArg(const TYPE& value, const LuaValueType& type)
    {
        mArgs.emplace_back(LuaValue(value, type));
    }

    /**
     * @brief Adds a description for a function return value.
     * 
     * @tparam TYPE The type of the return value.
     * @param value The value of the return value.
     * @param type The ValueType representing the type of the return value.
     */
    template<typename TYPE>
    void addRetVal(const TYPE& value, const LuaValueType& type)
    {
        mRetVals.emplace_back(LuaValue(value, type));
    }
};

#endif // FUNC_DESC_H