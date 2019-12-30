#include <iostream>
#include <type_traits>

#define Assert_Type_Traits(testname, classname) \
    static_assert(testname<classname>::value, #testname "<" #classname "> faild");

#define Is_Default_Constructible(classname) \
    Assert_Type_Traits(std::is_default_constructible, classname)

#define Is_Trivially_Default_Constructible(classname) \
    Assert_Type_Traits(std::is_trivially_default_constructible, classname)

using namespace std;

struct A
{
    A()
    noexcept
        : a(0)
    {
    }
    explicit A(double b)
        : a(b)
    {
    }
    double a;
};

Is_Default_Constructible(A)

    Is_Trivially_Default_Constructible(A);

static_assert(std::is_nothrow_default_constructible<A>::value, "A");

struct B final
{
    B() = default;
    B(double b)
        : a(b)
    {
    }
    double a;
};
static_assert(std::is_default_constructible<B>::value, "B");
static_assert(std::is_trivially_default_constructible<B>::value, "B");
static_assert(std::is_nothrow_default_constructible<B>::value, "B");

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
