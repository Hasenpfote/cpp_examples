#include <gtest/gtest.h>
#include <circular_buffer.h>

namespace
{

class CBTest : public ::testing::Test {};

TEST_F(CBTest, subscript_operator)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.push_back(3);

        EXPECT_EQ(1, scb[0]);
        EXPECT_EQ(2, scb[1]);
        EXPECT_EQ(3, scb[2]);

        EXPECT_EQ(1, c_scb[0]);
        EXPECT_EQ(2, c_scb[1]);
        EXPECT_EQ(3, c_scb[2]);
    }
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_front(0);
        scb.push_front(1);
        scb.push_front(2);
        scb.push_front(3);

        EXPECT_EQ(3, scb[0]);
        EXPECT_EQ(2, scb[1]);
        EXPECT_EQ(1, scb[2]);

        EXPECT_EQ(3, c_scb[0]);
        EXPECT_EQ(2, c_scb[1]);
        EXPECT_EQ(1, c_scb[2]);
    }
}

TEST_F(CBTest, front)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_front(0);
        EXPECT_EQ(0, scb.front());
        EXPECT_EQ(0, c_scb.front());

        scb.push_front(1);
        EXPECT_EQ(1, scb.front());
        EXPECT_EQ(1, c_scb.front());

        scb.push_front(2);
        EXPECT_EQ(2, scb.front());
        EXPECT_EQ(2, c_scb.front());

        scb.push_front(3);
        EXPECT_EQ(3, scb.front());
        EXPECT_EQ(3, c_scb.front());
    }
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        EXPECT_EQ(0, scb.front());
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(1);
        EXPECT_EQ(0, scb.front());
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(2);
        EXPECT_EQ(0, scb.front());
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(3);
        EXPECT_EQ(1, scb.front());
        EXPECT_EQ(1, c_scb.front());
    }
}

TEST_F(CBTest, back)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_front(0);
        EXPECT_EQ(0, scb.back());
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(1);
        EXPECT_EQ(0, scb.back());
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(2);
        EXPECT_EQ(0, scb.back());
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(3);
        EXPECT_EQ(1, scb.back());
        EXPECT_EQ(1, c_scb.back());
    }
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        EXPECT_EQ(0, scb.back());
        EXPECT_EQ(0, c_scb.back());

        scb.push_back(1);
        EXPECT_EQ(1, scb.back());
        EXPECT_EQ(1, c_scb.back());

        scb.push_back(2);
        EXPECT_EQ(2, scb.back());
        EXPECT_EQ(2, c_scb.back());

        scb.push_back(3);
        EXPECT_EQ(3, scb.back());
        EXPECT_EQ(3, c_scb.back());
    }
}

TEST_F(CBTest, clear)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    scb.push_back(0);
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    EXPECT_EQ(1, scb.tail());
    EXPECT_EQ(1, c_scb.tail());

    EXPECT_EQ(1, scb.size());
    EXPECT_EQ(1, c_scb.size());

    scb.clear();
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    EXPECT_EQ(0, scb.tail());
    EXPECT_EQ(0, c_scb.tail());

    EXPECT_EQ(0, scb.size());
    EXPECT_EQ(0, c_scb.size());
}

TEST_F(CBTest, head)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(0);
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(1);
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(2);
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(3);
    EXPECT_EQ(1, scb.head());
    EXPECT_EQ(1, c_scb.head());
}

TEST_F(CBTest, tail)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(0, scb.tail());
    EXPECT_EQ(0, c_scb.tail());

    scb.push_back(0);
    EXPECT_EQ(1, scb.tail());
    EXPECT_EQ(1, c_scb.tail());

    scb.push_back(1);
    EXPECT_EQ(2, scb.tail());
    EXPECT_EQ(2, c_scb.tail());

    scb.push_back(2);
    EXPECT_EQ(0, scb.tail());
    EXPECT_EQ(0, c_scb.tail());
}

TEST_F(CBTest, size)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    scb.push_back(0);
    EXPECT_EQ(1, scb.size());
    EXPECT_EQ(1, c_scb.size());

    scb.push_back(1);
    EXPECT_EQ(2, scb.size());
    EXPECT_EQ(2, c_scb.size());

    scb.push_back(2);
    EXPECT_EQ(3, scb.size());
    EXPECT_EQ(3, c_scb.size());

    scb.push_back(3);
    EXPECT_EQ(3, scb.size());
    EXPECT_EQ(3, c_scb.size());

    scb.pop_front();
    EXPECT_EQ(2, scb.size());
    EXPECT_EQ(2, c_scb.size());
}

TEST_F(CBTest, is_empty)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(true, scb.is_empty());
    EXPECT_EQ(true, c_scb.is_empty());

    scb.push_back(0);
    EXPECT_EQ(false, scb.is_empty());
    EXPECT_EQ(false, c_scb.is_empty());

    scb.pop_front();
    EXPECT_EQ(true, scb.is_empty());
    EXPECT_EQ(true, c_scb.is_empty());
}

TEST_F(CBTest, is_full)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(false, scb.is_full());
    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(0);
    EXPECT_EQ(false, scb.is_full());
    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(1);
    EXPECT_EQ(false, scb.is_full());
    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(2);
    EXPECT_EQ(true, scb.is_full());
    EXPECT_EQ(true, c_scb.is_full());

    scb.push_back(3);
    EXPECT_EQ(true, scb.is_full());
    EXPECT_EQ(true, c_scb.is_full());

    scb.pop_front();
    EXPECT_EQ(false, scb.is_full());
    EXPECT_EQ(false, c_scb.is_full());
}

TEST_F(CBTest, capacity)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(3, scb.capacity());
    EXPECT_EQ(3, c_scb.capacity());
}

TEST_F(CBTest, array_one)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    auto p = scb.data();
    auto cp = c_scb.data();

    scb.push_back(0);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);

        auto ca = c_scb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(1, ca.second);
    }
    scb.push_back(1);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(2, a.second);

        auto ca = c_scb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(2, ca.second);
    }
    scb.push_back(2);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(3, a.second);

        auto ca = c_scb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(3, ca.second);
    }
    scb.push_back(3);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p + 1, a.first);
        EXPECT_EQ(2, a.second);

        auto ca = c_scb.array_one();
        EXPECT_EQ(cp + 1, ca.first);
        EXPECT_EQ(2, ca.second);
    }
}

TEST_F(CBTest, array_two)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    auto p = scb.data();
    auto cp = c_scb.data();

    scb.push_back(0);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_scb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    scb.push_back(1);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_scb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    scb.push_back(2);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_scb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    scb.push_back(3);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);

        auto ca = c_scb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(1, ca.second);
    }
}

TEST_F(CBTest, is_linearized)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(true, scb.is_linearized());
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(0);
    EXPECT_EQ(true, scb.is_linearized());
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(1);
    EXPECT_EQ(true, scb.is_linearized());
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(2);
    EXPECT_EQ(true, scb.is_linearized());
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(3);
    EXPECT_EQ(false, scb.is_linearized());
    EXPECT_EQ(false, c_scb.is_linearized());

    scb.push_front(4);
    EXPECT_EQ(true, scb.is_linearized());
    EXPECT_EQ(true, c_scb.is_linearized());
}

TEST_F(CBTest, linearize)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    scb.push_back(0);
    scb.push_back(1);
    scb.push_back(2);
    scb.push_back(3);

    auto p = scb.data();
    EXPECT_EQ(3, *(p + 0));
    EXPECT_EQ(1, *(p + 1));
    EXPECT_EQ(2, *(p + 2));

    auto cp = c_scb.data();
    EXPECT_EQ(3, *(cp + 0));
    EXPECT_EQ(1, *(cp + 1));
    EXPECT_EQ(2, *(cp + 2));

    scb.linearize();

    p = scb.data();
    EXPECT_EQ(1, *(p + 0));
    EXPECT_EQ(2, *(p + 1));
    EXPECT_EQ(3, *(p + 2));

    cp = c_scb.data();
    EXPECT_EQ(1, *(cp + 0));
    EXPECT_EQ(2, *(cp + 1));
    EXPECT_EQ(3, *(cp + 2));
}

TEST_F(CBTest, iterator_bounds)
{
#if defined(NDEBUG)
    GTEST_SKIP();
#else
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        {
            auto beg_iter = scb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }

    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.pop_front();

        {
            auto beg_iter = scb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
#endif
}

TEST_F(CBTest, const_iterator_bounds)
{
#if defined(NDEBUG)
    GTEST_SKIP();
#else
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        {
            auto beg_iter = scb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.pop_front();

        {
            auto beg_iter = scb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
#endif
}

TEST_F(CBTest, reverse_iterator_bounds)
{
#if defined(NDEBUG)
    GTEST_SKIP();
#else
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        {
            auto beg_iter = scb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.pop_front();

        {
            auto beg_iter = scb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
#endif
}

TEST_F(CBTest, const_reverse_iterator_bounds)
{
#if defined(NDEBUG)
    GTEST_SKIP();
#else
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        {
            auto beg_iter = scb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.pop_front();

        {
            auto beg_iter = scb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = scb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_scb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_scb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
#endif
}

TEST_F(CBTest, forward_iteration)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);

        int counter = 0;
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);

        counter = 0;
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = scb.begin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
        {
            auto it = c_scb.begin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
    }
}

TEST_F(CBTest, forward_iteration_const)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);

        int counter = 0;
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);

        counter = 0;
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = scb.cbegin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
        {
            auto it = c_scb.cbegin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
    }
}

TEST_F(CBTest, backward_iteration)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = scb.rbegin(); it != scb.rend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_scb.rbegin(); it != c_scb.rend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);

        int counter = 0;
        for(auto it = scb.rbegin(); it != scb.rend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_scb.rbegin(); it != c_scb.rend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);

        counter = 0;
        for(auto it = scb.rbegin(); it != scb.rend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_scb.rbegin(); it != c_scb.rend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = scb.rbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
        {
            auto it = c_scb.rbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
    }
}

TEST_F(CBTest, backward_iteration_const)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = scb.crbegin(); it != scb.crend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_scb.crbegin(); it != c_scb.crend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        scb.push_back(0);
        scb.push_back(1);

        int counter = 0;
        for(auto it = scb.crbegin(); it != scb.crend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_scb.crbegin(); it != c_scb.crend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);

        counter = 0;
        for(auto it = scb.crbegin(); it != scb.crend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_scb.crbegin(); it != c_scb.crend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = scb.crbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
        {
            auto it = c_scb.crbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
    }
}

}