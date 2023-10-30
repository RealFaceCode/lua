#ifndef FUNC_DESC_H
#define FUNC_DESC_H

#include <vector>
#include "DynamicVar.h"
#include "luaTable.h"
#include <memory>


using LuaDescValue = DynamicVar<long long, double, bool, std::string, LuaTable>;
using LuaDescValueR = DynamicVar<long long*, double*, bool*, std::string*, LuaTable*>;

/**
 * @brief A class that holds descriptions of function arguments and return values.
 */
class FuncDescription
{
private:
    /**
     * @brief A vector containing descriptions of function arguments.
     */
    std::vector<LuaDescValue> mArgs;

    /**
     * @brief A vector containing descriptions of function return values.
     */
    std::vector<LuaDescValueR> mRetVals;

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
    std::vector<LuaDescValue>& getArgs();
    std::vector<LuaDescValueR>& getRetVals();
    
    /**
     * @brief Adds a description for a function argument.
     * 
     * @tparam TYPE The type of the argument value.
     * @param value The value of the argument.
     */
    template<typename Type>
    void addArg(const Type& value);

    /**
     * @brief Adds a description for a function return value.
     * 
     * @tparam TYPE The type of the return value.
     * @param value The value of the return value.
     */
    template<typename Type>
    void addRetVal(Type& value);
};

#endif // FUNC_DESC_H