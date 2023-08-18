#ifndef LUA_TABLE_H
#define LUA_TABLE_H

#include <string_view>
#include <any>
#include <vector>
#include "DynamicVar.h"
#include "luaValueType.h"
#include <iostream>

class LuaTable;

using LuaTableType = DynamicVar<long long, double, bool, std::string_view, LuaTable>;
using LuaTableVar = std::pair<std::string_view, LuaTableType>;

class LuaTable
{
private:
    std::string_view mTableName = "";
    std::vector<LuaTableVar> mValues = {};
    std::size_t mValueIndex = 0;

public:
    LuaTable() = default;
    explicit LuaTable(std::string_view name);
    
    void setName(std::string_view name);

    template<typename T>
    LuaTableType& addValue(T value)
    {
        return mValues.emplace_back(std::string_view(""), LuaTableType(value)).second;
    }

    template<typename T>
    LuaTableType& addValue(std::string_view name, T value)
    {
        return mValues.emplace_back(name, LuaTableType(value)).second;
    }
    
    bool isEnd() const;

    void print();

    std::pair<std::string_view, LuaTableType> getNextValue();
    std::string_view getValueName();
    std::string_view getName() const;

private:
    void resetIndex();
    static void resolveTable(LuaTable& table, int& depth, int& rDepth);
    template<typename T>
    static void printValue(LuaTableType& value, std::string_view vName)
    {
        if(vName == "")
            std::cout << value.retrieve<T>() << "," << std::endl;
        else
            std::cout << vName << " = " << value.retrieve<T>() << "," << std::endl;
    }
};

#endif // LUA_TABLE_H