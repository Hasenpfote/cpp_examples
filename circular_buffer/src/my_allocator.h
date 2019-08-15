#pragma once
#include <cstdlib>
#include <iostream>

template <class T>
struct MyAllocator
{
    using value_type = T;

    MyAllocator(){}

    template <class U>
    MyAllocator(const MyAllocator<U>&){}

    T* allocate(std::size_t n)
    {
        std::cout << __func__ << std::endl;
        return reinterpret_cast<T*>(std::malloc(sizeof(T) * n));
    }

    void deallocate(T* p, std::size_t n)
    {
        std::cout << __func__ << std::endl;
        static_cast<void>(n);
        std::free(p);
    }
};

template <class T, class U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&)
{
    std::cout << __func__ << std::endl;
    return true;
}

template <class T, class U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&)
{
    std::cout << __func__ << std::endl;
    return false;
}