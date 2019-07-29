#include <gtest/gtest.h>
#include <foo.h>

namespace
{

class FooTest : public ::testing::Test {};

TEST_F(FooTest, IsOdd)
{
    Foo f1(2);
    EXPECT_EQ(false, f1.IsOdd());

    Foo f2(3);
    EXPECT_EQ(true, f2.IsOdd());
}

TEST_F(FooTest, IsEven)
{
    Foo f1(4);
    EXPECT_EQ(true, f1.IsEven());

    Foo f2(5);
    EXPECT_EQ(false, f2.IsEven());
}

}