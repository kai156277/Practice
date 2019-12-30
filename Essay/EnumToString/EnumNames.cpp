#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "EnumNames.h"

#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#pragma execution_character_set("utf-8")

#ifdef ValuePair
#    undef ValuePair
#endif

#define ValuePair(token, describe) {#token, {token, #describe}},

struct EnumValueInfo
{
    int64_t     value;
    std::string describe;
};

#define END_LIST       \
    {                  \
        "", { -1, "" } \
    }

keyInfo findByKey(One::Two::Test key)
{
    static const std::map<std::string, EnumValueInfo> _keyInfo = {
#include "EnumKey.inc"
        END_LIST,
    };
    for (auto begin = _keyInfo.begin(); begin != _keyInfo.end(); ++begin)
    {
        if (begin->second.value == key)
            return {begin->first, begin->second.value, begin->second.describe};
    }
    return {"", -1, ""};
}

keyInfo findByName(const std::string &name)
{
    static const std::map<std::string, EnumValueInfo> _keyInfo = {
#include "EnumKey.inc"
        END_LIST,
    };
    auto search = _keyInfo.find(name);
    if (search != _keyInfo.end())
        return {search->first, search->second.value, search->second.describe};
    else
        return {"", -1, ""};
}
