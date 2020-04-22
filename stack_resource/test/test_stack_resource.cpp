#include <type_traits>
#include <cstdint>
#include <vector>
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <stack_resource.h>

namespace
{

template<typename T, typename U>
std::enable_if_t<std::is_integral_v<U>&& std::is_unsigned_v<U>, bool>
is_aligned(const T* ptr, U alignment)
{
    return is_aligned(reinterpret_cast<std::uintptr_t>(ptr), alignment);
}

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T>, bool>
is_aligned(std::uintptr_t address, T alignment)
{
    return (address % alignment) == 0u;
}

class StackResourceTest : public ::testing::Test {};

TEST_F(StackResourceTest, equality)
{
    using namespace container::pmr;

    stack_resource<10> sr1;
    stack_resource<10> sr2;
    stack_resource<11> sr3;

    EXPECT_EQ(true, sr1 == sr1);
    EXPECT_EQ(false, sr1 == sr2);
    EXPECT_EQ(false, sr1 == sr3);

    EXPECT_EQ(false, sr1 != sr1);
    EXPECT_EQ(true, sr1 != sr2);
    EXPECT_EQ(true, sr1 != sr3);
}

TEST_F(StackResourceTest, is_aligned)
{
    using value_type = int;

    container::pmr::stack_resource<100> sr;
    std::pmr::vector<value_type> v(3, &sr);

    EXPECT_EQ(true, is_aligned(v.data(), alignof(value_type)));
}

TEST_F(StackResourceTest, out_of_memory)
{
    using value_type = int;

    container::pmr::stack_resource<1> sr;

    EXPECT_THROW(
        {
            std::pmr::vector<value_type> v(3, &sr);
        }, std::bad_alloc);
}

TEST_F(StackResourceTest, normally_behavior)
{
    using value_type = int;
    {
        container::pmr::stack_resource<100> sr;
        std::pmr::vector<value_type> v(10, &sr);
        auto size = v.size();
        for(auto i = decltype(size)(0); i < size; i++)
        {
            auto x = static_cast<value_type>(i) + 1;
            v[i] = x;
            EXPECT_EQ(x, v[i]);
        }
    }
}

}
