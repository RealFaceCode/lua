#ifndef UTIL_H
#define UTIL_H

#include <string_view>

struct StringHash
{
public:
    std::size_t operator()(std::string_view str) const
    {
        return std::hash<std::string_view>{}(str);
    }
};

struct StringEqual
{
public:
    bool operator()(std::string_view lhs, std::string_view rhs) const
    {
        return lhs == rhs;
    }
};

struct TransparentHash
{
public:
    using is_transparent = void;
    std::size_t operator()(std::string_view str) const
    {
        return std::hash<std::string_view>{}(str);
    }
};

struct TransparentEqual
{
public:
    using is_transparent = void;
    bool operator()(std::string_view lhs, std::string_view rhs) const
    {
        return lhs == rhs;
    }
};

#endif // UTIL_H