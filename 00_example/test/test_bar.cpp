#include <gtest/gtest.h>
#include <bar.h>

namespace
{

class BarTest : public ::testing::Test {};

TEST_F(BarTest, IsReady)
{
    Bar b;
    EXPECT_EQ(false, b.IsReady());
}

}