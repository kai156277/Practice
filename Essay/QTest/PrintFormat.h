#pragma once
#include <QString>
#include <cstdint>
#include <type_traits>

enum Flag : char
{
    Default_Flag     = 0,   // 右对齐
    LeftAligned_Flag = '-',
    Zero_Flag        = '0',
    Blank_Flag       = ' ',
    Prefix_Flag      = '#'
};

enum TypeField : char
{
    Char_TypeField = 'c',   // 字符
    Sig_TypeField  = 'd',   // 有符号 10 进制
    Dec_TypeField  = 'u',   // 无符号 10 进制
    Oct_TypeField  = 'o',   // 无符号 8  进制
    hex_TypeField  = 'x',   // 无符号 16 进制 使用 "abcdef"
    Hex_TypeField  = 'X',   // 无符号 16 进制 使用 "ABCDEF"
    exp_TypeField  = 'e',   // 浮点数 指数 e 形式
    Exp_TypeField  = 'E',   // 浮点数 指数 E 形式
    flo_TypeField  = 'f',   // 浮点数 小数形式，infinity 和 nan 小写
    Flo_TypeField  = 'F',   // 浮点数 小数形式，infinity 和 nan 大写
};

enum ByteNum : uint8_t
{
    bit8_t  = 1,
    bit16_t = 2,
    bit32_t = 3,
    bit64_t = 4,
};

struct Format
{
    Format() = default;
    Format(uint32_t _pos, TypeField _field, Flag _flag = Default_Flag, uint32_t _width = 0, uint32_t _prec = 1)
        : pos(_pos)
        , field(_field)
        , flag(_flag)
        , width(_width)
        , prec(_prec)
    {
    }

    uint32_t  pos   = 0;
    TypeField field = TypeField::Char_TypeField;
    Flag      flag  = Default_Flag;
    uint32_t  width = 0;
    uint32_t  prec  = 1;
};

template <typename T>
QString formatString(T, Format f)
{
    static_assert(std::is_arithmetic<T>::value, "not arithmetic");

    if (f.field == TypeField::Char_TypeField)
    {
        if (std::is_same<T, char>::value || std::is_same<T, unsigned char>::value)
            return QString::asprintf("%%%u$c", f.pos);
        else
            return QString();
    }

    switch (sizeof(T))
    {
    case bit8_t:
        return QString::asprintf("%%%u$%c%u.%uhh%c", f.pos, (char) f.flag, f.width, f.prec, f.field);
    case bit16_t:
        return QString::asprintf("%%%u$%c%u.%uh%c", f.pos, (char) f.flag, f.width, f.prec, f.field);
    case bit32_t:
        return QString::asprintf("%%%u$%c%u.%u%c", f.pos, (char) f.flag, f.width, f.prec, f.field);
    case bit64_t:
        return QString::asprintf("%%%u$%c%u.%ull%c", f.pos, (char) f.flag, f.width, f.prec, f.field);
    default:
        return QString();
    };
}
