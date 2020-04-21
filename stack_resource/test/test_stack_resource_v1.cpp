#include <type_traits>
#include <cstdint>
#include <vector>
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <stack_resource.h>
#if defined(__clang__)
#include <experimental/vector>
template<typename T>
using pmr_vector = std::experimental::pmr::vector<T>;
#else
template<typename T>
using pmr_vector = std::pmr::vector<T>;
#endif

namespace
{

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T>, bool>
is_aligned(std::uintptr_t address, T alignment)
{
    return (address % alignment) == 0u;
}

template<typename T, typename U>
std::enable_if_t<std::is_integral_v<U>&& std::is_unsigned_v<U>, bool>
is_aligned(const T* ptr, U alignment)
{
    return is_aligned(reinterpret_cast<std::uintptr_t>(ptr), alignment);
}

class StackResourceV1Test : public ::testing::Test {};

TEST_F(StackResourceV1Test, equality)
{
    using namespace container::pmr::stack_resource_detail::v1;

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

TEST_F(StackResourceV1Test, is_aligned)
{
    using namespace container::pmr::stack_resource_detail::v1;
    using value_type = std::int32_t;

    stack_resource<100> sr;
    pmr_vector<value_type> v(3, &sr);

    EXPECT_EQ(true, is_aligned(v.data(), alignof(value_type)));
}

TEST_F(StackResourceV1Test, out_of_memory)
{
    using namespace container::pmr::stack_resource_detail::v1;
    using value_type = std::int32_t;

    stack_resource<1> sr;

    EXPECT_THROW(
        {
            pmr_vector<value_type> v(3, &sr);
        }, std::bad_alloc);
}

TEST_F(StackResourceV1Test, normally_behavior)
{
    using namespace container::pmr::stack_resource_detail::v1;
    using value_type = std::int32_t;
    {
        stack_resource<100> sr;
        pmr_vector<value_type> v(10, &sr);
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