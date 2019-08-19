#include <iostream>
#include "circular_buffer.h"
#include "my_allocator.h"

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
        for(auto elem : cb);

        const auto& c_cb = cb;
        for(auto it = c_cb.begin(); it != c_cb.end(); it++);
        for(auto it = c_cb.cbegin(); it != c_cb.cend(); it++);
        for(auto elem : c_cb);
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
    using value_type = int;
    using allocator_type = MyAllocator<value_type>;

    circular_buffer<value_type, allocator_type> cb(5);
    cb.push_back(0);
    cb.push_back(1);
    cb.push_back(2);
    for(auto x : cb)
        std::cout << x << std::endl;
}

int main()
{
#if 0
    test_cb();
#else
    test_custom_allocator();
#endif
    return 0;
}