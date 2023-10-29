#ifndef FUNC_INFO_H
#define FUNC_INFO_H

#include <string>
#include <string_view>

enum class FuncInfoType : int
{
    OK  = 1,
    REGISTER = 0,
    LOAD = -1,
    COMPILE = -2,
    RUN = -3,
};

class FuncInfo
{
private:
    std::string mDesc = "";
    FuncInfoType mType = FuncInfoType::OK;

public:
    FuncInfo();
    explicit FuncInfo(std::string_view desc);
    explicit FuncInfo(FuncInfoType type);
    FuncInfo(std::string_view desc, FuncInfoType type);
    ~FuncInfo();

    std::string_view getDesc() const;
    FuncInfoType getType() const;
    
    bool operator==(FuncInfoType type) const;
    bool operator!=(FuncInfoType type) const;
    explicit operator bool() const;
};

#endif // FUNC_INFO_H