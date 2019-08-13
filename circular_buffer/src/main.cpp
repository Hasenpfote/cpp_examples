#include <iostream>
#include "circular_buffer.h"

template<typename T>
void print(const simple_circular_buffer<T>& scb)
{
    for(const auto& elem : scb)
        std::cout << elem << " ";
    std::cout << std::endl;
    std::cout << "--- head=" << scb.head() << " tail=" << scb.tail() << " size=" << scb.size() << std::endl;
}

int main()
{
    {
        simple_circular_buffer<int> scb(5);
        std::cout << "is_empty: "<< std::boolalpha << scb.is_empty() << std::endl;

        for(auto it = scb.begin(); it != scb.end(); it++);
        for(auto it = scb.cbegin(); it != scb.cend(); it++);
        for(auto elem : scb);

        const auto& cscb = scb;
        for(auto it = cscb.begin(); it != cscb.end(); it++);
        for(auto it = cscb.cbegin(); it != cscb.cend(); it++);
        for(auto elem : cscb);
    }
    {
        simple_circular_buffer<int> scb(5);
        for(int i = 0; i < 8; i++)
        {
            std::cout << "# push_back" << std::endl;
            scb.push_back(i+1);
            print(scb);
        }
        for(int i = 0; i < 3; i++)
        {
            std::cout << "# pop_front" << std::endl;
            scb.pop_front();
            print(scb);
        }
    }
    std::cout << std::endl << std::endl;
    {
        simple_circular_buffer<int> scb(5);
        for(int i = 0; i < 8; i++)
        {
            std::cout << "# push_front" << std::endl;
            scb.push_front(i+1);
            print(scb);
        }
        for(int i = 0; i < 3; i++)
        {
            std::cout << "# pop_back" << std::endl;
            scb.pop_back();
            print(scb);
        }
    }
    return 0;
}