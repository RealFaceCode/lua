#ifndef UTIL_H
#define UTIL_H

#include <string_view>

struct StringHash
{
    std::size_t operator()(std::string_view str) const;
};

struct StringEqual
{
    bool operator()(std::string_view lhs, std::string_view rhs) const;
};

struct TransparentHash
{
    using is_transparent = void;
    std::size_t operator()(std::string_view str) const;
};

struct TransparentEqual
{
    using is_transparent = void;
    bool operator()(std::string_view lhs, std::string_view rhs) const;
};

#endif // UTIL_H