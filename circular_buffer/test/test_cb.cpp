#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <circular_buffer.h>

namespace
{

class CBTest : public ::testing::Test {};

using namespace container;

TEST_F(CBTest, move_construction)
{
    circular_buffer<int> cb(3);

    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    auto capacity = cb.capacity();
    auto head = cb.head();
    auto tail = cb.tail();
    auto size = cb.size();

    circular_buffer<int> cb2(std::move(cb));

    EXPECT_EQ(0, cb.capacity());
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, cb.tail());
    EXPECT_EQ(0, cb.size());

    EXPECT_EQ(capacity, cb2.capacity());
    EXPECT_EQ(head, cb2.head());
    EXPECT_EQ(tail, cb2.tail());
    EXPECT_EQ(size, cb2.size());
}

TEST_F(CBTest, move_assignment)
{
    circular_buffer<int> cb(3);

    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    auto capacity = cb.capacity();
    auto head = cb.head();
    auto tail = cb.tail();
    auto size = cb.size();

    circular_buffer<int> cb2(1);
    cb2 = std::move(cb);

    EXPECT_EQ(0, cb.capacity());
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, cb.tail());
    EXPECT_EQ(0, cb.size());

    EXPECT_EQ(capacity, cb2.capacity());
    EXPECT_EQ(head, cb2.head());
    EXPECT_EQ(tail, cb2.tail());
    EXPECT_EQ(size, cb2.size());
}

TEST_F(CBTest, subscript_operator)
{
#if !defined(NDEBUG)
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        EXPECT_DEATH({ cb[0]; }, "");
        EXPECT_DEATH({ cb[2]; }, "");
        EXPECT_DEATH({ cb[3]; }, "");

        EXPECT_DEATH({ c_cb[0]; }, "");
        EXPECT_DEATH({ c_cb[2]; }, "");
        EXPECT_DEATH({ c_cb[3]; }, "");

        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();

        EXPECT_NONFATAL_FAILURE(EXPECT_DEATH({ cb[0]; }, ""), "");
        EXPECT_NONFATAL_FAILURE(EXPECT_DEATH({ cb[1]; }, ""), "");
        EXPECT_DEATH({ cb[2]; }, "");
        EXPECT_DEATH({ cb[3]; }, "");

        EXPECT_NONFATAL_FAILURE(EXPECT_DEATH({ c_cb[0]; }, ""), "");
        EXPECT_NONFATAL_FAILURE(EXPECT_DEATH({ c_cb[1]; }, ""), "");
        EXPECT_DEATH({ c_cb[2]; }, "");
        EXPECT_DEATH({ c_cb[3]; }, "");
    }
#endif
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        EXPECT_EQ(1, cb[0]);
        EXPECT_EQ(2, cb[1]);
        EXPECT_EQ(3, cb[2]);

        EXPECT_EQ(1, c_cb[0]);
        EXPECT_EQ(2, c_cb[1]);
        EXPECT_EQ(3, c_cb[2]);
    }
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_front(0);
        cb.push_front(1);
        cb.push_front(2);
        cb.push_front(3);

        EXPECT_EQ(3, cb[0]);
        EXPECT_EQ(2, cb[1]);
        EXPECT_EQ(1, cb[2]);

        EXPECT_EQ(3, c_cb[0]);
        EXPECT_EQ(2, c_cb[1]);
        EXPECT_EQ(1, c_cb[2]);
    }
}

TEST_F(CBTest, at)
{
#if !defined(NDEBUG)
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        EXPECT_DEATH({ cb.at(0); }, "");
        EXPECT_DEATH({ cb.at(2); }, "");
        EXPECT_DEATH({ cb.at(3); }, "");

        EXPECT_DEATH({ c_cb.at(0); }, "");
        EXPECT_DEATH({ c_cb.at(2); }, "");
        EXPECT_DEATH({ c_cb.at(3); }, "");
    }
#endif
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.pop_front();

        EXPECT_EQ(2, cb.at(0));
        EXPECT_EQ(3, cb.at(1));

        EXPECT_EQ(2, c_cb.at(0));
        EXPECT_EQ(3, c_cb.at(1));

        EXPECT_THROW({ cb.at(2); }, std::out_of_range);
        EXPECT_THROW({ c_cb.at(2); }, std::out_of_range);
    }
}

TEST_F(CBTest, front)
{
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_front(0);
        EXPECT_EQ(0, cb.front());
        EXPECT_EQ(0, c_cb.front());

        cb.push_front(1);
        EXPECT_EQ(1, cb.front());
        EXPECT_EQ(1, c_cb.front());

        cb.push_front(2);
        EXPECT_EQ(2, cb.front());
        EXPECT_EQ(2, c_cb.front());

        cb.push_front(3);
        EXPECT_EQ(3, cb.front());
        EXPECT_EQ(3, c_cb.front());
    }
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_back(0);
        EXPECT_EQ(0, cb.front());
        EXPECT_EQ(0, c_cb.front());

        cb.push_back(1);
        EXPECT_EQ(0, cb.front());
        EXPECT_EQ(0, c_cb.front());

        cb.push_back(2);
        EXPECT_EQ(0, cb.front());
        EXPECT_EQ(0, c_cb.front());

        cb.push_back(3);
        EXPECT_EQ(1, cb.front());
        EXPECT_EQ(1, c_cb.front());
    }
}

TEST_F(CBTest, back)
{
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_front(0);
        EXPECT_EQ(0, cb.back());
        EXPECT_EQ(0, c_cb.back());

        cb.push_front(1);
        EXPECT_EQ(0, cb.back());
        EXPECT_EQ(0, c_cb.back());

        cb.push_front(2);
        EXPECT_EQ(0, cb.back());
        EXPECT_EQ(0, c_cb.back());

        cb.push_front(3);
        EXPECT_EQ(1, cb.back());
        EXPECT_EQ(1, c_cb.back());
    }
    {
        circular_buffer<int> cb(3);
        const auto& c_cb = cb;

        cb.push_back(0);
        EXPECT_EQ(0, cb.back());
        EXPECT_EQ(0, c_cb.back());

        cb.push_back(1);
        EXPECT_EQ(1, cb.back());
        EXPECT_EQ(1, c_cb.back());

        cb.push_back(2);
        EXPECT_EQ(2, cb.back());
        EXPECT_EQ(2, c_cb.back());

        cb.push_back(3);
        EXPECT_EQ(3, cb.back());
        EXPECT_EQ(3, c_cb.back());
    }
}

TEST_F(CBTest, clear)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    cb.push_back(0);
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    EXPECT_EQ(1, cb.tail());
    EXPECT_EQ(1, c_cb.tail());

    EXPECT_EQ(1, cb.size());
    EXPECT_EQ(1, c_cb.size());

    EXPECT_EQ(3, cb.capacity());
    EXPECT_EQ(3, c_cb.capacity());

    cb.clear();
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    EXPECT_EQ(0, cb.tail());
    EXPECT_EQ(0, c_cb.tail());

    EXPECT_EQ(0, cb.size());
    EXPECT_EQ(0, c_cb.size());

    EXPECT_EQ(3, cb.capacity());
    EXPECT_EQ(3, c_cb.capacity());

    EXPECT_NONFATAL_FAILURE(
        EXPECT_DEATH(
            {
                circular_buffer<int> cb2(3);
                cb2.clear();
                cb2.push_back(1);
            },
            ""),
        "");
}

TEST_F(CBTest, head)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    cb.push_back(0);
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    cb.push_back(1);
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    cb.push_back(2);
    EXPECT_EQ(0, cb.head());
    EXPECT_EQ(0, c_cb.head());

    cb.push_back(3);
    EXPECT_EQ(1, cb.head());
    EXPECT_EQ(1, c_cb.head());
}

TEST_F(CBTest, tail)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(0, cb.tail());
    EXPECT_EQ(0, c_cb.tail());

    cb.push_back(0);
    EXPECT_EQ(1, cb.tail());
    EXPECT_EQ(1, c_cb.tail());

    cb.push_back(1);
    EXPECT_EQ(2, cb.tail());
    EXPECT_EQ(2, c_cb.tail());

    cb.push_back(2);
    EXPECT_EQ(0, cb.tail());
    EXPECT_EQ(0, c_cb.tail());
}

TEST_F(CBTest, size)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    cb.push_back(0);
    EXPECT_EQ(1, cb.size());
    EXPECT_EQ(1, c_cb.size());

    cb.push_back(1);
    EXPECT_EQ(2, cb.size());
    EXPECT_EQ(2, c_cb.size());

    cb.push_back(2);
    EXPECT_EQ(3, cb.size());
    EXPECT_EQ(3, c_cb.size());

    cb.push_back(3);
    EXPECT_EQ(3, cb.size());
    EXPECT_EQ(3, c_cb.size());

    cb.pop_front();
    EXPECT_EQ(2, cb.size());
    EXPECT_EQ(2, c_cb.size());
}

TEST_F(CBTest, is_empty)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(true, cb.is_empty());
    EXPECT_EQ(true, c_cb.is_empty());

    cb.push_back(0);
    EXPECT_EQ(false, cb.is_empty());
    EXPECT_EQ(false, c_cb.is_empty());

    cb.pop_front();
    EXPECT_EQ(true, cb.is_empty());
    EXPECT_EQ(true, c_cb.is_empty());
}

TEST_F(CBTest, is_full)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(false, cb.is_full());
    EXPECT_EQ(false, c_cb.is_full());

    cb.push_back(0);
    EXPECT_EQ(false, cb.is_full());
    EXPECT_EQ(false, c_cb.is_full());

    cb.push_back(1);
    EXPECT_EQ(false, cb.is_full());
    EXPECT_EQ(false, c_cb.is_full());

    cb.push_back(2);
    EXPECT_EQ(true, cb.is_full());
    EXPECT_EQ(true, c_cb.is_full());

    cb.push_back(3);
    EXPECT_EQ(true, cb.is_full());
    EXPECT_EQ(true, c_cb.is_full());

    cb.pop_front();
    EXPECT_EQ(false, cb.is_full());
    EXPECT_EQ(false, c_cb.is_full());
}

TEST_F(CBTest, capacity)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(3, cb.capacity());
    EXPECT_EQ(3, c_cb.capacity());
}

TEST_F(CBTest, array_one)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    auto p = cb.data();
    auto cp = c_cb.data();

    cb.push_back(0);
    {
        auto a = cb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);

        auto ca = c_cb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(1, ca.second);
    }
    cb.push_back(1);
    {
        auto a = cb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(2, a.second);

        auto ca = c_cb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(2, ca.second);
    }
    cb.push_back(2);
    {
        auto a = cb.array_one();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(3, a.second);

        auto ca = c_cb.array_one();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(3, ca.second);
    }
    cb.push_back(3);
    {
        auto a = cb.array_one();
        EXPECT_EQ(p + 1, a.first);
        EXPECT_EQ(2, a.second);

        auto ca = c_cb.array_one();
        EXPECT_EQ(cp + 1, ca.first);
        EXPECT_EQ(2, ca.second);
    }
}

TEST_F(CBTest, array_two)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    auto p = cb.data();
    auto cp = c_cb.data();

    cb.push_back(0);
    {
        auto a = cb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_cb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    cb.push_back(1);
    {
        auto a = cb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_cb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    cb.push_back(2);
    {
        auto a = cb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(0, a.second);

        auto ca = c_cb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(0, ca.second);
    }
    cb.push_back(3);
    {
        auto a = cb.array_two();
        EXPECT_EQ(p, a.first);
        EXPECT_EQ(1, a.second);

        auto ca = c_cb.array_two();
        EXPECT_EQ(cp, ca.first);
        EXPECT_EQ(1, ca.second);
    }
}

TEST_F(CBTest, is_linearized)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    EXPECT_EQ(true, cb.is_linearized());
    EXPECT_EQ(true, c_cb.is_linearized());

    cb.push_back(0);
    EXPECT_EQ(true, cb.is_linearized());
    EXPECT_EQ(true, c_cb.is_linearized());

    cb.push_back(1);
    EXPECT_EQ(true, cb.is_linearized());
    EXPECT_EQ(true, c_cb.is_linearized());

    cb.push_back(2);
    EXPECT_EQ(true, cb.is_linearized());
    EXPECT_EQ(true, c_cb.is_linearized());

    cb.push_back(3);
    EXPECT_EQ(false, cb.is_linearized());
    EXPECT_EQ(false, c_cb.is_linearized());

    cb.push_front(4);
    EXPECT_EQ(true, cb.is_linearized());
    EXPECT_EQ(true, c_cb.is_linearized());
}

TEST_F(CBTest, linearize)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    cb.push_back(0);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    auto p = cb.data();
    EXPECT_EQ(3, *(p + 0));
    EXPECT_EQ(1, *(p + 1));
    EXPECT_EQ(2, *(p + 2));

    auto cp = c_cb.data();
    EXPECT_EQ(3, *(cp + 0));
    EXPECT_EQ(1, *(cp + 1));
    EXPECT_EQ(2, *(cp + 2));

    cb.linearize();

    p = cb.data();
    EXPECT_EQ(1, *(p + 0));
    EXPECT_EQ(2, *(p + 1));
    EXPECT_EQ(3, *(p + 2));

    cp = c_cb.data();
    EXPECT_EQ(1, *(cp + 0));
    EXPECT_EQ(2, *(cp + 1));
    EXPECT_EQ(3, *(cp + 2));
}

TEST_F(CBTest, iterator_bounds)
{
#if defined(NDEBUG)
    GTEST_SKIP();
#else
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        {
            auto beg_iter = cb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }

    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.pop_front();

        {
            auto beg_iter = cb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.end();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.begin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.end();
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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        {
            auto beg_iter = cb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.pop_front();

        {
            auto beg_iter = cb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.cend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.cbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.cend();
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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        {
            auto beg_iter = cb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.pop_front();

        {
            auto beg_iter = cb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.rend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.rbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.rend();
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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        {
            auto beg_iter = cb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.pop_front();

        {
            auto beg_iter = cb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = cb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
        {
            auto beg_iter = c_cb.crbegin();
            EXPECT_DEATH({ beg_iter--; }, "");

            auto end_iter = c_cb.crend();
            EXPECT_DEATH({ end_iter++; }, "");
        }
    }
#endif
}

TEST_F(CBTest, forward_iteration)
{
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        int counter = 0;
        for(auto it = cb.begin(); it != cb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_cb.begin(); it != c_cb.end(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);

        int counter = 0;
        for(auto it = cb.begin(); it != cb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_cb.begin(); it != c_cb.end(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        cb.push_back(2);
        cb.push_back(3);

        counter = 0;
        for(auto it = cb.begin(); it != cb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_cb.begin(); it != c_cb.end(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = cb.begin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
        {
            auto it = c_cb.begin();

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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        int counter = 0;
        for(auto it = cb.cbegin(); it != cb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_cb.cbegin(); it != c_cb.cend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);

        int counter = 0;
        for(auto it = cb.cbegin(); it != cb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_cb.cbegin(); it != c_cb.cend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        cb.push_back(2);
        cb.push_back(3);

        counter = 0;
        for(auto it = cb.cbegin(); it != cb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_cb.cbegin(); it != c_cb.cend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = cb.cbegin();

            EXPECT_EQ(1, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(3, *it);
        }
        {
            auto it = c_cb.cbegin();

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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        int counter = 0;
        for(auto it = cb.rbegin(); it != cb.rend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_cb.rbegin(); it != c_cb.rend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);

        int counter = 0;
        for(auto it = cb.rbegin(); it != cb.rend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_cb.rbegin(); it != c_cb.rend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        cb.push_back(2);
        cb.push_back(3);

        counter = 0;
        for(auto it = cb.rbegin(); it != cb.rend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_cb.rbegin(); it != c_cb.rend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = cb.rbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
        {
            auto it = c_cb.rbegin();

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
    circular_buffer<int> cb(3);
    const auto& c_cb = cb;

    // empty
    {
        int counter = 0;
        for(auto it = cb.crbegin(); it != cb.crend(); it++)
            counter++;
        EXPECT_EQ(0, counter);

        counter = 0;
        for(auto it = c_cb.crbegin(); it != c_cb.crend(); it++)
            counter++;
        EXPECT_EQ(0, counter);
    }
    // not empty
    {
        cb.push_back(0);
        cb.push_back(1);

        int counter = 0;
        for(auto it = cb.crbegin(); it != cb.crend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        counter = 0;
        for(auto it = c_cb.crbegin(); it != c_cb.crend(); it++)
            counter++;
        EXPECT_EQ(2, counter);

        cb.push_back(2);
        cb.push_back(3);

        counter = 0;
        for(auto it = cb.crbegin(); it != cb.crend(); it++)
            counter++;
        EXPECT_EQ(3, counter);

        counter = 0;
        for(auto it = c_cb.crbegin(); it != c_cb.crend(); it++)
            counter++;
        EXPECT_EQ(3, counter);
    }
    //
    {
        {
            auto it = cb.crbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
        {
            auto it = c_cb.crbegin();

            EXPECT_EQ(3, *it);
            it++;
            EXPECT_EQ(2, *it);
            it++;
            EXPECT_EQ(1, *it);
        }
    }
}

}
