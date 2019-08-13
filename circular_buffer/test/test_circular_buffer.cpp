#include <gtest/gtest.h>
#include <circular_buffer.h>

namespace
{

class CircularBufferTest : public ::testing::Test {};

TEST_F(CircularBufferTest, subscript_operator)
{
    {
        simple_circular_buffer<int> scb(3);

        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.push_back(3);

        EXPECT_EQ(1, scb[0]);
        EXPECT_EQ(2, scb[1]);
        EXPECT_EQ(3, scb[2]);
    }
    {
        simple_circular_buffer<int> scb(3);

        scb.push_front(0);
        scb.push_front(1);
        scb.push_front(2);
        scb.push_front(3);

        EXPECT_EQ(3, scb[0]);
        EXPECT_EQ(2, scb[1]);
        EXPECT_EQ(1, scb[2]);
    }
}

TEST_F(CircularBufferTest, front)
{
    {
        simple_circular_buffer<int> scb(3);

        scb.push_front(0);
        EXPECT_EQ(0, scb.front());

        scb.push_front(1);
        EXPECT_EQ(1, scb.front());

        scb.push_front(2);
        EXPECT_EQ(2, scb.front());

        scb.push_front(3);
        EXPECT_EQ(3, scb.front());
    }
    {
        simple_circular_buffer<int> scb(3);

        scb.push_back(0);
        EXPECT_EQ(0, scb.front());

        scb.push_back(1);
        EXPECT_EQ(0, scb.front());

        scb.push_back(2);
        EXPECT_EQ(0, scb.front());

        scb.push_back(3);
        EXPECT_EQ(1, scb.front());
    }
}

TEST_F(CircularBufferTest, back)
{
    {
        simple_circular_buffer<int> scb(3);

        scb.push_front(0);
        EXPECT_EQ(0, scb.back());

        scb.push_front(1);
        EXPECT_EQ(0, scb.back());

        scb.push_front(2);
        EXPECT_EQ(0, scb.back());

        scb.push_front(3);
        EXPECT_EQ(1, scb.back());
    }
    {
        simple_circular_buffer<int> scb(3);

        scb.push_back(0);
        EXPECT_EQ(0, scb.back());

        scb.push_back(1);
        EXPECT_EQ(1, scb.back());

        scb.push_back(2);
        EXPECT_EQ(2, scb.back());

        scb.push_back(3);
        EXPECT_EQ(3, scb.back());
    }
}

TEST_F(CircularBufferTest, clear)
{
    simple_circular_buffer<int> scb(3);

    scb.push_back(0);
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(1, scb.tail());
    EXPECT_EQ(1, scb.size());

    scb.clear();
    EXPECT_EQ(0, scb.head());
    EXPECT_EQ(0, scb.tail());
    EXPECT_EQ(0, scb.size());
}

TEST_F(CircularBufferTest, head)
{
    simple_circular_buffer<int> scb(3);

    EXPECT_EQ(0, scb.head());

    scb.push_back(0);
    EXPECT_EQ(0, scb.head());

    scb.push_back(1);
    EXPECT_EQ(0, scb.head());

    scb.push_back(2);
    EXPECT_EQ(0, scb.head());

    scb.push_back(3);
    EXPECT_EQ(1, scb.head());
}

TEST_F(CircularBufferTest, tail)
{
    simple_circular_buffer<int> scb(3);

    EXPECT_EQ(0, scb.tail());

    scb.push_back(0);
    EXPECT_EQ(1, scb.tail());

    scb.push_back(1);
    EXPECT_EQ(2, scb.tail());

    scb.push_back(2);
    EXPECT_EQ(0, scb.tail());
}

TEST_F(CircularBufferTest, size)
{
    simple_circular_buffer<int> scb(3);

    scb.push_back(0);
    EXPECT_EQ(1, scb.size());

    scb.push_back(1);
    EXPECT_EQ(2, scb.size());

    scb.push_back(2);
    EXPECT_EQ(3, scb.size());

    scb.push_back(3);
    EXPECT_EQ(3, scb.size());

    scb.pop_front();
    EXPECT_EQ(2, scb.size());
}

TEST_F(CircularBufferTest, is_empty)
{
    simple_circular_buffer<int> scb(3);

    EXPECT_EQ(true, scb.is_empty());

    scb.push_back(0);
    EXPECT_EQ(false, scb.is_empty());

    scb.pop_front();
    EXPECT_EQ(true, scb.is_empty());
}

TEST_F(CircularBufferTest, is_full)
{
    simple_circular_buffer<int> scb(3);

    EXPECT_EQ(false, scb.is_full());

    scb.push_back(0);
    EXPECT_EQ(false, scb.is_full());

    scb.push_back(1);
    EXPECT_EQ(false, scb.is_full());

    scb.push_back(2);
    EXPECT_EQ(true, scb.is_full());

    scb.push_back(3);
    EXPECT_EQ(true, scb.is_full());

    scb.pop_front();
    EXPECT_EQ(false, scb.is_full());
}

TEST_F(CircularBufferTest, capacity)
{
    simple_circular_buffer<int> scb(3);
    EXPECT_EQ(3, scb.capacity());
}

TEST_F(CircularBufferTest, array_one)
{
    simple_circular_buffer<int> scb(3);
    auto p = scb.data();

    scb.push_back(0);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);
    }
    scb.push_back(1);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(2, a.second);
    }
    scb.push_back(2);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(3, a.second);
    }
    scb.push_back(3);
    {
        auto a = scb.array_one();
        EXPECT_EQ(p+1, a.first);
        EXPECT_EQ(2, a.second);
    }
}

TEST_F(CircularBufferTest, array_two)
{
    simple_circular_buffer<int> scb(3);
    auto p = scb.data();

    scb.push_back(0);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(1);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(2);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(3);
    {
        auto a = scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);
    }
}

TEST_F(CircularBufferTest, is_linearized)
{
    simple_circular_buffer<int> scb(3);

    EXPECT_EQ(true, scb.is_linearized());

    scb.push_back(0);
    EXPECT_EQ(true, scb.is_linearized());

    scb.push_back(1);
    EXPECT_EQ(true, scb.is_linearized());

    scb.push_back(2);
    EXPECT_EQ(true, scb.is_linearized());

    scb.push_back(3);
    EXPECT_EQ(false, scb.is_linearized());

    scb.push_front(4);
    EXPECT_EQ(true, scb.is_linearized());
}

TEST_F(CircularBufferTest, linearize)
{
    simple_circular_buffer<int> scb(3);

    scb.push_back(0);
    scb.push_back(1);
    scb.push_back(2);
    scb.push_back(3);

    auto p = scb.data();
    EXPECT_EQ(3, *(p + 0));
    EXPECT_EQ(1, *(p + 1));
    EXPECT_EQ(2, *(p + 2));

    scb.linearize();
    p = scb.data();
    EXPECT_EQ(1, *(p + 0));
    EXPECT_EQ(2, *(p + 1));
    EXPECT_EQ(3, *(p + 2));
}

TEST_F(CircularBufferTest, iterator)
{
    simple_circular_buffer<int> scb(3);

    // empty
    {
        int counter = 0;
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        int counter = 0;
        scb.push_back(0);
        scb.push_back(1);
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);
        counter = 0;
        for(auto it = scb.begin(); it != scb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        auto it = scb.begin();

        EXPECT_EQ(1, *it);
        it++;
        EXPECT_EQ(2, *it);
        it++;
        EXPECT_EQ(3, *it);
    }
}

TEST_F(CircularBufferTest, const_iterator)
{
    simple_circular_buffer<int> scb(3);

    // empty
    {
        int counter = 0;
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        int counter = 0;
        scb.push_back(0);
        scb.push_back(1);
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);
        counter = 0;
        for(auto it = scb.cbegin(); it != scb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        auto it = scb.cbegin();

        EXPECT_EQ(1, *it);
        it++;
        EXPECT_EQ(2, *it);
        it++;
        EXPECT_EQ(3, *it);
    }
}


class ConstCircularBufferTest : public ::testing::Test {};

TEST_F(ConstCircularBufferTest, subscript_operator)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        scb.push_back(1);
        scb.push_back(2);
        scb.push_back(3);

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

        EXPECT_EQ(3, c_scb[0]);
        EXPECT_EQ(2, c_scb[1]);
        EXPECT_EQ(1, c_scb[2]);
    }
}

TEST_F(ConstCircularBufferTest, front)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_front(0);
        EXPECT_EQ(0, c_scb.front());

        scb.push_front(1);
        EXPECT_EQ(1, c_scb.front());

        scb.push_front(2);
        EXPECT_EQ(2, c_scb.front());

        scb.push_front(3);
        EXPECT_EQ(3, c_scb.front());
    }
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(1);
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(2);
        EXPECT_EQ(0, c_scb.front());

        scb.push_back(3);
        EXPECT_EQ(1, c_scb.front());
    }
}

TEST_F(ConstCircularBufferTest, back)
{
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_front(0);
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(1);
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(2);
        EXPECT_EQ(0, c_scb.back());

        scb.push_front(3);
        EXPECT_EQ(1, c_scb.back());
    }
    {
        simple_circular_buffer<int> scb(3);
        const auto& c_scb = scb;

        scb.push_back(0);
        EXPECT_EQ(0, c_scb.back());

        scb.push_back(1);
        EXPECT_EQ(1, c_scb.back());

        scb.push_back(2);
        EXPECT_EQ(2, c_scb.back());

        scb.push_back(3);
        EXPECT_EQ(3, c_scb.back());
    }
}

TEST_F(ConstCircularBufferTest, clear)
{
    GTEST_SKIP();
}

TEST_F(ConstCircularBufferTest, head)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(0, c_scb.head());

    scb.push_back(0);
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(1);
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(2);
    EXPECT_EQ(0, c_scb.head());

    scb.push_back(3);
    EXPECT_EQ(1, c_scb.head());
}

TEST_F(ConstCircularBufferTest, tail)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(0, c_scb.tail());

    scb.push_back(0);
    EXPECT_EQ(1, c_scb.tail());

    scb.push_back(1);
    EXPECT_EQ(2, c_scb.tail());

    scb.push_back(2);
    EXPECT_EQ(0, c_scb.tail());
}

TEST_F(ConstCircularBufferTest, size)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    scb.push_back(0);
    EXPECT_EQ(1, c_scb.size());

    scb.push_back(1);
    EXPECT_EQ(2, c_scb.size());

    scb.push_back(2);
    EXPECT_EQ(3, c_scb.size());

    scb.push_back(3);
    EXPECT_EQ(3, c_scb.size());

    scb.pop_front();
    EXPECT_EQ(2, c_scb.size());
}

TEST_F(ConstCircularBufferTest, is_empty)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(true, c_scb.is_empty());

    scb.push_back(0);
    EXPECT_EQ(false, c_scb.is_empty());

    scb.pop_front();
    EXPECT_EQ(true, c_scb.is_empty());
}

TEST_F(ConstCircularBufferTest, is_full)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(0);
    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(1);
    EXPECT_EQ(false, c_scb.is_full());

    scb.push_back(2);
    EXPECT_EQ(true, c_scb.is_full());

    scb.push_back(3);
    EXPECT_EQ(true, c_scb.is_full());

    scb.pop_front();
    EXPECT_EQ(false, c_scb.is_full());
}

TEST_F(ConstCircularBufferTest, capacity)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;
    EXPECT_EQ(3, c_scb.capacity());
}

TEST_F(ConstCircularBufferTest, array_one)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    auto p = c_scb.data();

    scb.push_back(0);
    {
        auto a = c_scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);
    }
    scb.push_back(1);
    {
        auto a = c_scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(2, a.second);
    }
    scb.push_back(2);
    {
        auto a = c_scb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(3, a.second);
    }
    scb.push_back(3);
    {
        auto a = c_scb.array_one();
        EXPECT_EQ(p + 1, a.first);
        EXPECT_EQ(2, a.second);
    }
}

TEST_F(ConstCircularBufferTest, array_two)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    auto p = c_scb.data();

    scb.push_back(0);
    {
        auto a = c_scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(1);
    {
        auto a = c_scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(2);
    {
        auto a = c_scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);
    }
    scb.push_back(3);
    {
        auto a = c_scb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);
    }
}

TEST_F(ConstCircularBufferTest, is_linearized)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(0);
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(1);
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(2);
    EXPECT_EQ(true, c_scb.is_linearized());

    scb.push_back(3);
    EXPECT_EQ(false, c_scb.is_linearized());

    scb.push_front(4);
    EXPECT_EQ(true, c_scb.is_linearized());
}

TEST_F(ConstCircularBufferTest, linearize)
{
    GTEST_SKIP();
}

TEST_F(ConstCircularBufferTest, iterator)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        int counter = 0;
        scb.push_back(0);
        scb.push_back(1);
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);
        counter = 0;
        for(auto it = c_scb.begin(); it != c_scb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        auto it = c_scb.begin();

        EXPECT_EQ(1, *it);
        it++;
        EXPECT_EQ(2, *it);
        it++;
        EXPECT_EQ(3, *it);
    }
}

TEST_F(ConstCircularBufferTest, const_iterator)
{
    simple_circular_buffer<int> scb(3);
    const auto& c_scb = scb;

    // empty
    {
        int counter = 0;
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        int counter = 0;
        scb.push_back(0);
        scb.push_back(1);
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        scb.push_back(2);
        scb.push_back(3);
        counter = 0;
        for(auto it = c_scb.cbegin(); it != c_scb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
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