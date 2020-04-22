#include "foo.h"

Foo::Foo(int number)
{
    number_ = number;
}

bool Foo::IsOdd() const noexcept
{
    //return (number_ % 2) == 0;  // bug
    return (number_ % 2) != 0;
}

bool Foo::IsEven() const noexcept
{
    return (number_ % 2) == 0;
}
