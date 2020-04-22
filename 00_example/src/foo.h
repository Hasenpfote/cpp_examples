#pragma once

class Foo final
{
public:
    Foo(int number);
    ~Foo() = default;

    bool IsOdd() const noexcept;
    bool IsEven() const noexcept;

private:
    int number_;
};
