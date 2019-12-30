#include <boost/exception/all.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <utility>

using namespace boost;

#pragma execution_character_set("utf-8")

struct ErrorEnum
{
    typedef enum
    {
        One = 0,
        Two,
        Three
    } EnumType;
};
struct ErrorEnum1
{
    typedef enum
    {
        One = 0,
        Two,
        Three
    } EnumType;
};
struct ErrorEnum2
{
};

std::string ClassEnumToString(ErrorEnum::EnumType v)
{
    std::cout << "ErrorEnum template" << std::endl;
    if (v < 0)
        return "v < 0 ErrorEnum";
    else if (v == 0)
        return "v = 0 ErrorEnum";
    else
        return "v> 0 ErrorEnum";
}

std::string ClassEnumToString(ErrorEnum1::EnumType v)
{
    std::cout << "ErrorEnum1 template" << std::endl;
    if (v < 0)
        return "v < 0 ErrorEnum1";
    else if (v == 0)
        return "v = 0 ErrorEnum1";
    else
        return "v> 0 ErrorEnum1";
}

typedef error_info<ErrorEnum, ErrorEnum::EnumType>   errinfo_errrenum;
typedef error_info<ErrorEnum1, ErrorEnum1::EnumType> errinfo_errrenum1;

inline std::string to_string(errinfo_errrenum const &e)
{
    std::ostringstream tmp;
    tmp << '[' << error_info_name(e) << "] = " << e.value() << ", \"" << ClassEnumToString(e.value()) << "\"\n";
    return tmp.str();
}

inline std::string to_string(errinfo_errrenum1 const &e)
{
    std::ostringstream tmp;
    tmp << '[' << error_info_name(e) << "] = " << e.value() << ", \"" << ClassEnumToString(e.value()) << "\"\n";
    return tmp.str();
}

struct my_exception
    : public virtual boost::exception
    , public virtual std::exception
{
    const char *what() const override
    {
        return "Custom exception";
    }
};

struct format_exception : public my_exception
{
    const char *what() const override
    {
        return "ErrorFormat";
    }
};

typedef error_info<struct tag_range_min, size_t>   errinfo_range_min;
typedef error_info<struct tag_range_max, size_t>   errinfo_range_max;
typedef error_info<struct tag_range_index, size_t> errinfo_range_index;

#define POS_EXCeption_add_baseinfo(x)       \
    (std::string("[") +                     \
     std::string(__FILE__) +                \
     std::string("(") +                     \
     std::to_string(__LINE__) +             \
     std::string("): Throw in function ") + \
     std::string(__FUNCSIG__) +             \
     std::string("]: ") +                   \
     std::string((x)))

int main()
{
    system("chcp 65001");
    try
    {
        BOOST_THROW_EXCEPTION(enable_error_info(std::range_error("超出范围"))
                              << errinfo_range_min(0)
                              << errinfo_range_max(20)
                              << errinfo_range_index(99));
    }
    catch (exception &e)
    {
        std::cout << diagnostic_information(e) << std::endl;
    }

    try
    {
        BOOST_THROW_EXCEPTION(my_exception()
                              << errinfo_errno(1));
    }
    catch (exception &e)
    {
        std::cout << diagnostic_information(e) << std::endl;
    }
    std::cout << POS_EXCeption_add_baseinfo("test") << std::endl;
    return 0;
}
