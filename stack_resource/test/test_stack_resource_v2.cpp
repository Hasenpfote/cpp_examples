#include <type_traits>
#include <cstdint>
#include <vector>
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <stack_resource.h>

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

class StackResourceV2Test : public ::testing::Test {};

TEST_F(StackResourceV2Test, equality)
{
    using namespace container::pmr::stack_resource_detail::v2;
    {
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
    {
        stack_resource<10, 4> sr1;
        stack_resource<10, 4> sr2;
        stack_resource<11, 4> sr3;

        EXPECT_EQ(true, sr1 == sr1);
        EXPECT_EQ(false, sr1 == sr2);
        EXPECT_EQ(false, sr1 == sr3);

        EXPECT_EQ(false, sr1 != sr1);
        EXPECT_EQ(true, sr1 != sr2);
        EXPECT_EQ(true, sr1 != sr3);
    }
}

TEST_F(StackResourceV2Test, is_aligned)
{
    using namespace container::pmr::stack_resource_detail::v2;
    {
        using value_type = std::int32_t;

        stack_resource<100> sr;
        std::pmr::vector<value_type> v(3, &sr);

        EXPECT_EQ(true, is_aligned(v.data(), alignof(value_type)));
    }
    {
        using value_type = std::int32_t;
#if defined(_MSC_VER) && defined(_WIN64)
        {
            stack_resource<100, 8> sr;
            std::pmr::vector<value_type> v(3, &sr);
            EXPECT_EQ(true, is_aligned(v.data(), alignof(value_type)));
        }
#if !defined(NDEBUG)
        {
            stack_resource<100, 4> sr;
            EXPECT_DEATH(
                {
                    std::pmr::vector<value_type> v(3, &sr);
                }, "");
        }
#endif
#else
        stack_resource<100, 4> sr;
        std::pmr::vector<value_type> v(3, &sr);
        EXPECT_EQ(true, is_aligned(v.data(), alignof(value_type)));
#endif
    }
#if !defined(NDEBUG)
    {
        using value_type = std::int32_t;

        stack_resource<100, 2> sr;
        EXPECT_DEATH(
            {
                std::pmr::vector<value_type> v(3, &sr);
            }, "");
    }
#endif
}

TEST_F(StackResourceV2Test, out_of_memory)
{
    using namespace container::pmr::stack_resource_detail::v2;
    using value_type = std::int32_t;
    {
        stack_resource<1> sr;
        EXPECT_THROW(
            {
                std::pmr::vector<value_type> v(3, &sr);
            }, std::bad_alloc);
    }
}

TEST_F(StackResourceV2Test, normally_behavior)
{
    using namespace container::pmr::stack_resource_detail::v2;
    using value_type = std::int32_t;
    {
        stack_resource<100> sr;
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