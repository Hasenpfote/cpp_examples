#pragma once
#include <cstdlib>
#include <iostream>

template <class T>
struct CustomAllocator
{
    using value_type = T;

    CustomAllocator()
    {
        std::cout << __func__ << std::endl;
    }

    template <class U>
    CustomAllocator(const CustomAllocator<U>&)
    {
        std::cout << __func__ << std::endl;
    }

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
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&)
{
    std::cout << __func__ << std::endl;
    return true;
}

template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&)
{
    std::cout << __func__ << std::endl;
    return false;
}