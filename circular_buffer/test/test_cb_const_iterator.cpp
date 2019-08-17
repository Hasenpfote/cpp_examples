#include <typeinfo>
#include <algorithm>
#include <gtest/gtest.h>
#include <circular_buffer.h>

namespace
{

/*
    Input <- Forward <- Bidirectional <- Random Access
    Output
 */
class CBConstIteratorTest : public ::testing::Test {};

struct Foo
{
    Foo() = default;
    explicit Foo(int value) : value_(value) {}
    int value_;
};

TEST_F(CBConstIteratorTest, is_input_iterator)
{
    // Is copy-constructible, copy-assignable and destructible
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb::const_iterator a;
        auto b(a);
        b = a;
    }

    // Can be compared for equivalence using the equality/inequality operators
    // (meaningful if both iterators are be in domain).
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        {
            auto a = cb.cbegin();
            auto b(a);
            EXPECT_EQ(true, a == b);
            EXPECT_EQ(false, a != b);
        }
        {
            const auto a = cb.cbegin();
            const auto b(a);
            EXPECT_EQ(true, a == b);
            EXPECT_EQ(false, a != b);
        }
    }

    // Can be dereferenced as an rvalue(if in a dereferenceable state).
    {
        using my_cb = simple_circular_buffer<Foo>;

        my_cb cb(3);
        cb.push_back(Foo(1));
        {
            auto a = cb.cbegin();
            EXPECT_EQ(true, typeid(*a) == typeid(Foo));
            EXPECT_EQ(1, a->value_);
        }
        {
            const auto a = cb.cbegin();
            EXPECT_EQ(true, typeid(*a) == typeid(Foo));
            EXPECT_EQ(1, a->value_);
        }
    }

    // Can be incremented(if in a dereferenceable state).
    // The result is either also dereferenceable or a past - the - end iterator.
    // The previous iterator value is not required to be dereferenceable after the increase
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(4);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.push_back(4);

        auto a = cb.cbegin();
        ++a;
        EXPECT_EQ(2, *a);
        (void)a++;
        EXPECT_EQ(3, *a);
        EXPECT_EQ(3, *a++);
#if !defined(NDEBUG)
        auto b = cb.cend();
        EXPECT_DEATH({ b++; }, "");
#endif
    }

    // Lvalues are swappable.
#if 0
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        auto a = cb.cbegin();
        auto b = cb.cbegin();
        b++;
        b++;
        EXPECT_EQ(1, *a);
        EXPECT_EQ(3, *b);
        std::iter_swap(a, b);
        EXPECT_EQ(3, *a);
        EXPECT_EQ(1, *b);
    }
#endif
}

TEST_F(CBConstIteratorTest, is_output_iterator)
{
    // Is copy - constructible, copy - assignable and destructible
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb::const_iterator a;
        auto b(a);
        b = a;
    }

    // Can be dereferenced as an lvalue (if in a dereferenceable state).
    // It shall only be dereferenced as the left - side of an assignment statement.
    // Once dereferenced, its iterator value may no longer be dereferenceable.
#if 0
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(1);
        cb.push_back(1);
        {
            auto a = cb.cbegin();
            int value = 10;
            *a = value;
            EXPECT_EQ(10, *a);
        }
        {
            const auto a = cb.cbegin();
            int value = 10;
            *a = value;
            EXPECT_EQ(10, *a);
        }
    }
#endif
    // Can be incremented.
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(4);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.push_back(4);

        auto a = cb.cbegin();
        ++a;
        EXPECT_EQ(2, *a);
        a++;
        EXPECT_EQ(3, *a);
#if 0
        auto x = a;
        int value = 10;
        *a++ = value;
        EXPECT_EQ(10, *x);
#endif
#if !defined(NDEBUG)
        auto b = cb.cend();
        EXPECT_DEATH({ b++; }, "");
#endif
    }

    // Lvalues are swappable.
#if 0
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        auto a = cb.cbegin();
        auto b = cb.cbegin();
        b++;
        b++;
        EXPECT_EQ(1, *a);
        EXPECT_EQ(3, *b);
        std::iter_swap(a, b);
        EXPECT_EQ(3, *a);
        EXPECT_EQ(1, *b);
    }
#endif
}

TEST_F(CBConstIteratorTest, is_forward_iterator)
{
    // For mutable iterators(non - constant iterators) :
    // Can be dereferenced as an lvalue(if in a dereferenceable state)
#if 0
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(1);
        cb.push_back(1);
        {
            auto a = cb.cbegin();
            int value = 10;
            *a = value;
            EXPECT_EQ(10, *a);
        }
        {
            const auto a = cb.cbegin();
            int value = 10;
            *a = value;
            EXPECT_EQ(10, *a);
        }

    }
#endif
}

TEST_F(CBConstIteratorTest, is_bidirectional_iterator)
{
    // Can be decremented (if a dereferenceable iterator value precedes it).
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        auto a = cb.cend();
        --a;
        EXPECT_EQ(3, *a);
        a--;
        EXPECT_EQ(2, *a);
#if 0
        auto x = a;
        int value = 10;
        *a-- = value;
        EXPECT_EQ(10, *x);
#endif
#if !defined(NDEBUG)
        auto b = cb.cbegin();
        EXPECT_DEATH({ b--; }, "");
#endif
    }
}

TEST_F(CBConstIteratorTest, is_random_access_iterator)
{
    // Supports the arithmetic operators + and - between an iterator and an integer value,
    // or subtracting an iterator from another.
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();
        {
            auto a = cb.cbegin();
            auto b = cb.cend();
#if !defined(NDEBUG)
            EXPECT_DEATH({ a - 1; }, "");
            EXPECT_DEATH({ a + 3; }, "");
            EXPECT_DEATH({ b + 1; }, "");
            EXPECT_DEATH({ b - 3; }, "");
#endif
            EXPECT_EQ(3, *(a + 1));
            EXPECT_EQ(3, *(1 + a));
            EXPECT_EQ(3, *(b - 1));
            EXPECT_EQ(2, b - a);
        }
        {
            const auto a = cb.cbegin();
            const auto b = cb.cend();
#if !defined(NDEBUG)
            EXPECT_DEATH({ a - 1; }, "");
            EXPECT_DEATH({ a + 3; }, "");
            EXPECT_DEATH({ b + 1; }, "");
            EXPECT_DEATH({ b - 3; }, "");
#endif
            EXPECT_EQ(3, *(a + 1));
            EXPECT_EQ(3, *(1 + a));
            EXPECT_EQ(3, *(b - 1));
            EXPECT_EQ(2, b - a);
        }
    }

    // Can be compared with inequality relational operators (<, >, <= and >=).
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();
        {
            auto a = cb.cbegin();
            auto b = cb.cend();

            EXPECT_EQ(true, a < b);
            EXPECT_EQ(false, a > b);
            EXPECT_EQ(true, a <= b);
            EXPECT_EQ(false, a >= b);
        }
        {
            const auto a = cb.cbegin();
            const auto b = cb.cend();

            EXPECT_EQ(true, a < b);
            EXPECT_EQ(false, a > b);
            EXPECT_EQ(true, a <= b);
            EXPECT_EQ(false, a >= b);
        }
    }

    // Supports compound assignment operations += and -=
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();

        auto a = cb.cbegin();
        auto b = cb.cend();
#if !defined(NDEBUG)
        EXPECT_DEATH({ a -= 1; }, "");
        EXPECT_DEATH({ a += 3; }, "");
        EXPECT_DEATH({ b += 1; }, "");
        EXPECT_DEATH({ b -= 3; }, "");
#endif
        a += 1;
        EXPECT_EQ(3, *a);

        b -= 1;
        EXPECT_EQ(3, *b);
    }

    // Supports the offset dereference operator ([])
    {
        using my_cb = simple_circular_buffer<int>;

        my_cb cb(3);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();

        auto a = cb.cbegin();
        const auto ca = cb.cbegin();
#if !defined(NDEBUG)
        EXPECT_DEATH({ a[-1]; }, "");
        EXPECT_DEATH({ ca[-1]; }, "");

        EXPECT_DEATH({ a[3]; }, "");
        EXPECT_DEATH({ ca[3]; }, "");
#endif
        EXPECT_EQ(2, a[0]);
        EXPECT_EQ(2, ca[0]);

        EXPECT_EQ(3, a[1]);
        EXPECT_EQ(3, ca[1]);
#if 0
        a[0] = 4;
        EXPECT_EQ(4, a[0]);
        EXPECT_EQ(4, ca[0]);

        a[1] = 5;
        EXPECT_EQ(5, a[1]);
        EXPECT_EQ(5, ca[1]);
#endif
    }
}

}