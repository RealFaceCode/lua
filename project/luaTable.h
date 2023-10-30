#ifndef LUA_TABLE_H
#define LUA_TABLE_H

#include <string_view>
#include <any>
#include <vector>
#include "DynamicVar.h"
#include <iostream>
#include <type_traits>

class LuaTable;

using LuaTableType = DynamicVar<long long, double, bool, std::string, LuaTable>;
using LuaTableVar = std::pair<std::string, LuaTableType>;

class LuaTable
{
private:
    std::string_view mTableName = "";
    std::vector<LuaTableVar> mValues = {};
    std::size_t mValueIndex = 0;
    bool mIndexed = false;

public:
    LuaTable() = default;
    explicit LuaTable(std::string_view name);
    
    void setName(std::string_view name);
    void setIndexed(bool indexed = true);

    template<typename T>
    LuaTableType& addValue(T value)
    {
        return mValues.emplace_back(std::string(""), LuaTableType(value)).second;
    }

    template<typename T>
    LuaTableType& addValue(std::string_view name, T value)
    {
        return mValues.emplace_back(std::string(name), LuaTableType(value)).second;
    }
    
    bool isEnd() const;
    bool isIndexed() const;

    void print();

    std::pair<std::string_view, LuaTableType> getNextValue();
    std::string_view getValueName();
    std::string_view getName() const;
    std::size_t size() const;

private:
    void resetIndex();
    static void resolveTable(LuaTable& table, int& depth, int& rDepth);
    template<typename T>
    static void printValue(LuaTableType& value, std::string_view vName)
    {
        if(vName == "")
            {
            if(value.hasType<bool>())
                std::cout << ((value.retrieve<bool>()) ? "true" : "false") << "," << std::endl;
            else
                std::cout << value.retrieve<T>() << "," << std::endl;
        }
        else
        {
            if(value.hasType<bool>())
                std::cout << vName << " = " << ((value.retrieve<bool>()) ? "true" : "false") << "," << std::endl;
            else
                std::cout << vName << " = " << value.retrieve<T>() << "," << std::endl;
        }
    }
};

#endif // LUA_TABLE_H