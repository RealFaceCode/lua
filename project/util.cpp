#include "util.h"


std::size_t StringHash::operator()(std::string_view str) const
{
    return std::hash<std::string_view>{}(str);
}

bool StringEqual::operator()(std::string_view lhs, std::string_view rhs) const
{
    return lhs == rhs;
}

std::size_t TransparentHash::operator()(std::string_view str) const
{
    return std::hash<std::string_view>{}(str);
}

bool TransparentEqual::operator()(std::string_view lhs, std::string_view rhs) const
{
    return lhs == rhs;
}