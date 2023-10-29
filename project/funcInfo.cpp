#include "funcInfo.h"

FuncInfo::FuncInfo() = default;

FuncInfo::FuncInfo(std::string_view desc)
: mDesc(desc)
{}

FuncInfo::FuncInfo(FuncInfoType type)
: mType(type)
{}

FuncInfo::FuncInfo(std::string_view desc, FuncInfoType type)
: mDesc(desc), mType(type)
{}

FuncInfo::~FuncInfo() = default;

std::string_view FuncInfo::getDesc() const
{
    return mDesc;
}

FuncInfoType FuncInfo::getType() const
{
    return mType;
}

bool FuncInfo::operator==(FuncInfoType type) const
{
    return mType == type;
}

bool FuncInfo::operator!=(FuncInfoType type) const
{
    return mType != type;
}

FuncInfo::operator bool() const
{
    return mType == FuncInfoType::OK;
}
