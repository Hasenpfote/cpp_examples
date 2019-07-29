#include <iostream>
#include "foo.h"
#include "bar.h"

int main()
{
    Foo f1(2);
    std::cout << f1.IsOdd() << ", " << f1.IsEven() << std::endl;

    Foo f2(3);
    std::cout << f2.IsOdd() << ", " << f2.IsEven() << std::endl;

    Bar b;
    std::cout << b.IsReady() << std::endl;

    return 0;
}