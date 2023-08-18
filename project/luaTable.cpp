#include "luaTable.h"
#include <iostream>

LuaTable::LuaTable(std::string_view name)
: mTableName(name)
{}

void LuaTable::setName(std::string_view name)
{
    mTableName = name;
}

bool LuaTable::isEnd() const
{
    return mValueIndex == mValues.size();
}

void LuaTable::print()
{
    int depth = 0;
    int rDepth = 1;
    resolveTable(*this, depth, rDepth);
}

std::pair<std::string_view, LuaTableType> LuaTable::getNextValue()
{
    if(!isEnd())
    {
        auto const& [first, last] = mValues.at(mValueIndex);
        mValueIndex++;
        return {first, last};
    }
    return std::pair<std::string_view, LuaTableType>();
}

std::string_view LuaTable::getValueName()
{
    if(!isEnd())
    {
        auto const& [first, last] = mValues.at(mValueIndex - 1);
        return first;
    }
    return std::string_view();
}

std::string_view LuaTable::getName() const
{
    return mTableName;
}

void LuaTable::resetIndex()
{
    mValueIndex = 0;
}

void LuaTable::resolveTable(LuaTable &table, int &depth, int& rDepth)
{
    for(int i = 0; i < depth; i++)
            std::cout << "\t";
    std::cout << table.getName() << " {" << std::endl;
    depth++;
    while(!table.isEnd())
    {
        auto [name, value] = table.getNextValue();
        for(int i = 0; i < (value.hasType<LuaTable>() ? depth - rDepth : depth); i++)
            std::cout << "\t";
        
        if(value.hasValue() && value.hasType<long long>())
            printValue<long long>(value, name);
        else if(value.hasValue() && value.hasType<double>())
            printValue<double>(value, name);
        else if(value.hasValue() && value.hasType<bool>())
            printValue<bool>(value, name);
        else if(value.hasValue() && value.hasType<std::string_view>())
            printValue<std::string_view>(value, name);
        else if(value.hasValue() && value.hasType<LuaTable>())
        {
            rDepth++;
            resolveTable(value.retrieve<LuaTable>(), depth, rDepth);
            rDepth--;
        }
        else 
        {
            table.resetIndex();
            return;
        }
    }
    depth--;
    for(int i = 0; i < depth; i++)
            std::cout << "\t";
    std::cout << "}" << std::endl;
}
