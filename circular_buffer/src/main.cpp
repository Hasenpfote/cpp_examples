#include <iostream>
#include "circular_buffer.h"
#include "custom_allocator.h"
#include "custom_resource.h"

namespace
{

using namespace container;

template<typename T>
void print(const circular_buffer<T>& cb)
{
    for(const auto& elem : cb)
        std::cout << elem << " ";
    std::cout << std::endl;
    std::cout << "--- head=" << cb.head() << " tail=" << cb.tail() << " size=" << cb.size() << std::endl;
}

void test_cb()
{
    {
        circular_buffer<int> cb(5);
        std::cout << "is_empty: " << std::boolalpha << cb.is_empty() << std::endl;

        for(auto it = cb.begin(); it != cb.end(); it++);
        for(auto it = cb.cbegin(); it != cb.cend(); it++);
        for(auto elem : cb){ (void)elem; }

        const auto& c_cb = cb;
        for(auto it = c_cb.begin(); it != c_cb.end(); it++);
        for(auto it = c_cb.cbegin(); it != c_cb.cend(); it++);
        for(auto elem : c_cb){ (void)elem; }
    }
    {
        circular_buffer<int> cb(5);
        for(int i = 0; i < 8; i++)
        {
            std::cout << "# push_back" << std::endl;
            cb.push_back(i + 1);
            print(cb);
        }
        for(int i = 0; i < 3; i++)
        {
            std::cout << "# pop_front" << std::endl;
            cb.pop_front();
            print(cb);
        }
    }
    std::cout << std::endl << std::endl;
    {
        circular_buffer<int> cb(5);
        for(int i = 0; i < 8; i++)
        {
            std::cout << "# push_front" << std::endl;
            cb.push_front(i + 1);
            print(cb);
        }
        for(int i = 0; i < 3; i++)
        {
            std::cout << "# pop_back" << std::endl;
            cb.pop_back();
            print(cb);
        }
    }
}

void test_custom_allocator()
{
#if defined(__has_include) && __has_include(<memory_resource>)
    using value_type = int;
    std::cout << "allocator(custom) ---" << std::endl;
    {
        using allocator_type = CustomAllocator<value_type>;

        circular_buffer<value_type, allocator_type> cb(3);
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        for(auto x : cb)
            std::cout << x << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "pmr(default) ---" << std::endl;
    {
        pmr::circular_buffer<value_type> cb(3);
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        for(auto x : cb)
            std::cout << x << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "pmr(custom) ---" << std::endl;
    {
        CustomResource test_resource;
        pmr::circular_buffer<value_type> cb(3, &test_resource);
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        for(auto x : cb)
            std::cout << x << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "pmr(custom) ---" << std::endl;
    {
        CustomResource test_resource;
        pmr::circular_buffer<value_type> cb1(3);
        cb1.push_back(0);
        cb1.push_back(1);
        cb1.push_back(2);

        pmr::circular_buffer<value_type> cb2(std::move(cb1), &test_resource);

        {
            bool is_same = typeid(std::pmr::polymorphic_allocator<int>) == typeid(CustomResource);
            std::cout << "is_same: " << is_same << std::endl;
        }

        std::cout << "cb1.capacity=" << cb1.capacity() << std::endl;

        for(auto x : cb2)
            std::cout << x << " ";
        std::cout << std::endl;
    }
#endif
}

}   // namespace

int main()
{
    test_cb();
    test_custom_allocator();
    return 0;
}
