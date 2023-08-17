#ifndef DYNAMIC_VAR_H
#define DYNAMIC_VAR_H

#include <variant>
#include <type_traits>
#include <stdexcept>

template <typename... AllowedTypes>
class DynamicVar
{
private:
    std::variant<AllowedTypes...> mData;
    bool mEmpty = true;
public:
    DynamicVar() = default;

    template <typename T>
    explicit DynamicVar(const T& value)
    : mData(value), mEmpty(false)
    {
        static_assert((std::is_same_v<T, AllowedTypes> || ...), "Type not allowed");
    }

    ~DynamicVar() = default;

    template <typename T>
    void store(const T& value)
    {
        static_assert((std::is_same_v<T, AllowedTypes> || ...), "Type not allowed");
        mData = value;
        mEmpty = false;
    }

    template<typename T>
    T& retrieve()
    {
        static_assert((std::is_same_v<T, AllowedTypes> || ...), "Type not allowed");
        if (hasValue()) 
        {
            try 
            {
                return std::get<T>(mData);
            }
            catch (const std::bad_variant_access&) 
            {
                throw std::invalid_argument("Incorrect stored type");
            }
        } 
        else 
        {
            throw std::empty("No value stored");
        }
    }

    template<typename T>
    const T& retrieve() const
    {
        static_assert((std::is_same_v<T, AllowedTypes> || ...), "Type not allowed");
        if (hasValue()) 
        {
            try 
            {
                return std::get<T>(mData);
            }
            catch (const std::bad_variant_access&) 
            {
                throw std::invalid_argument("Incorrect stored type");
            }
        } 
        else 
        {
            throw std::empty("No value stored");
        }
    }

    template <typename T>
    bool hasType() const
    {
        return (std::holds_alternative<T>(mData));
    }

    bool hasValue() const
    {
        return !mData.valueless_by_exception() && !mEmpty;
    }

    template <typename T>
    bool canStoreType() const {
        return (std::is_same_v<T, AllowedTypes> || ...);
    }
};

#endif // DYNAMIC_VAR_H