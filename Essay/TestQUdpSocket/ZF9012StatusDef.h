#pragma once   // ZF9012STATUSDEF_H

#include <stdint.h>

enum ZF9012StatusItemType : int16_t
{
    ZF9012Status_ScanningTime  = 1,
    ZF9012Status_NumOfLines    = 2,
    ZF9012Status_Progress      = 3,
    ZF9012Status_FileSize      = 4,
    ZF9012Status_PixelPerSec   = 5,
    ZF9012Status_LinesPerSec   = 6,
    ZF9012Status_MaxRange      = 8,
    ZF9012Status_Filter        = 10,
    ZF9012Status_HorzPosition  = 12,
    ZF9012Status_NumOfErrors   = 14,
    ZF9012Status_Step          = 15,
    ZF9012Status_ReferenzRange = 20,
    ZF9012Status_XLevel        = 21,
    ZF9012Status_YLevel        = 22
};

struct int32group
{
    int32_t first;
    int32_t second;
};

union value_t
{
    double     _doubleValue;
    int32_t    _int32Value;
    int32group _groupValue;
};

struct PackageItem
{
    int16_t index;
    int16_t type;
    int32_t dummy;
    value_t value;
};

struct head
{
    int32_t id;
    int32_t seq;
    int32_t packageNum;
    int32_t numOfPackage;
};
