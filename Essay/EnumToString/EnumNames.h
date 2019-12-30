#pragma once   // ENUMNAMES_H

#include "EnumType.h"

#include <string>

struct keyInfo
{
    std::string name;
    int64_t     value;
    std::string describe;
};

keyInfo findByKey(One::Two::Test key);

keyInfo findByName(const std::string &name);
