#include <iostream>
#include <functional>
#include <chrono>
#if defined(__clang__)
#include <experimental/vector>
#else
#include <vector>
#endif
#include <algorithm>
#include <iterator>
#include "stack_resource.h"

namespace
{

template<typename F, typename... Args>
auto measure(F&& f, Args&& ... args)
{
    auto begin = std::chrono::steady_clock::now();
    std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    return end - begin;
}

#if defined(__clang__)
namespace std_pmr = std::experimental::pmr;
#else
namespace std_pmr = std::pmr;
#endif

constexpr std::size_t num_elems = 100;
constexpr std::size_t num_bytes = 1024;

void test_std_vector(std::size_t num_trials)
{
    for(std::size_t i = 0; i < num_trials; i++)
    {
        std::vector<int> v(num_elems);
        for(std::size_t j = 0; j < num_elems; j++)
            v[j] = static_cast<int>(j);
    }
}

void test_pmr_vector_with_sr_v1(std::size_t num_trials)
{
    using stack_resource = container::pmr::stack_resource_detail::v1::stack_resource<num_bytes>;
    stack_resource sr;

    for(std::size_t i = 0; i < num_trials; i++)
    {
        std_pmr::vector<int> v(num_elems, &sr);
        for(std::size_t j = 0; j < num_elems; j++)
            v[j] = static_cast<int>(j);
    }
}

void test_pmr_vector_with_sr_v2(std::size_t num_trials)
{
    using stack_resource = container::pmr::stack_resource_detail::v2::stack_resource<num_bytes>;
    stack_resource sr;

    for(std::size_t i = 0; i < num_trials; i++)
    {
        std_pmr::vector<int> v(num_elems, &sr);
        for(std::size_t j = 0; j < num_elems; j++)
            v[j] = static_cast<int>(j);
    }
}

}   // namespace

int main()
{
#if 0
    {
        using stack_resource = container::pmr::stack_resource<100>;
        stack_resource sr;
        std::cout << "usage: " << sr.usage() << " / " << stack_resource::size() << std::endl;
        {
            using value_type = int;
            std_pmr::vector<value_type> v(3, &sr);
            std::cout << "usage: " << sr.usage() << " / " << stack_resource::size() << std::endl;

            const auto alignment = alignof(value_type);
            const auto ptr = v.data();

            std::cout
                << "0x" << ptr
                << " is_aligned: " << std::boolalpha << container::pmr::stack_resource_detail::is_aligned(ptr, alignment)
                << std::endl;

            v[0] = 1;
            v[1] = 2;
            v[2] = 3;

            std::copy(v.begin(), v.end(), std::ostream_iterator<value_type>(std::cout, ","));
            std::cout << std::endl;
        }
        std::cout << "usage: " << sr.usage() << " / " << stack_resource::size() << std::endl;
    }
#else
    {
        const std::size_t num_trials = 100000;

        auto diff1 = std::chrono::duration_cast<std::chrono::nanoseconds>(measure(test_std_vector, num_trials));
        auto diff2 = std::chrono::duration_cast<std::chrono::nanoseconds>(measure(test_pmr_vector_with_sr_v1, num_trials));
        auto diff3 = std::chrono::duration_cast<std::chrono::nanoseconds>(measure(test_pmr_vector_with_sr_v2, num_trials));

        std::cout << "diff1: " << diff1.count() << std::endl;
        std::cout << "diff2: " << diff2.count() << std::endl;
        std::cout << "diff3: " << diff3.count() << std::endl;
    }
#endif
    std::cout << "done" << std::endl;
    return 0;
}
