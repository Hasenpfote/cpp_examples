#pragma once
#if defined(__has_include) && __has_include(<memory_resource>)
#include <memory_resource>
#include <iostream>

class CustomResource
    : public std::pmr::memory_resource
{
protected:
    void* do_allocate(std::size_t bytes, [[maybe_unused]] size_t alignment) override
    {
        std::cout << __func__ << std::endl;
        return ::operator new(bytes);
    }

    void do_deallocate(void* p, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
    {
        std::cout << __func__ << std::endl;
        ::operator delete(p);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        std::cout << __func__ << std::endl;
        return this == &other;
    }
};
#endif
