#include <gtest/gtest.h>
#include <dual.h>

#define GTEST_COUT std::cerr << "[ INFO     ] "

namespace
{

class DualfTest : public ::testing::Test
{
protected:
    using Dual = dual::Dual<float>;
    using value_type = Dual::value_type;
    using nlimits = std::numeric_limits<value_type>;

    static constexpr auto rel_tolerance = 1e-5f;
    static constexpr auto abs_tolerance = 1e-4f;

    static const value_type PI;
};

const DualfTest::value_type DualfTest::PI = std::acos(-value_type(1.0));

TEST_F(DualfTest, AdditionAssignment)
{
    auto d = Dual(value_type(1.0), value_type(2.0));
    d += Dual(value_type(3.0), value_type(4.0));
    EXPECT_FLOAT_EQ(value_type(4.0), d.real());
    EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
}

TEST_F(DualfTest, SubtractionAssignment)
{
    auto d = Dual(value_type(1.0), value_type(2.0));
    d -= Dual(value_type(3.0), value_type(4.0));
    EXPECT_FLOAT_EQ(-value_type(2.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(2.0), d.dual());
}

TEST_F(DualfTest, MultiplicationAssignment)
{
    {
        auto d = Dual(value_type(1.0), value_type(2.0));
        d *= Dual(value_type(3.0), value_type(4.0));
        EXPECT_FLOAT_EQ(value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(10.0), d.dual());
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0));
        d *= value_type(3.0);
        EXPECT_FLOAT_EQ(value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
    }
}

TEST_F(DualfTest, DivisionAssignment)
{
    {
        auto d = Dual(value_type(1.0), value_type(2.0));
        d /= Dual(value_type(3.0), value_type(4.0));
        EXPECT_FLOAT_EQ(value_type(1.0) / value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / value_type(9.0), d.dual());
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0));
        d /= Dual(value_type(0.0), value_type(4.0));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0));
        d /= value_type(3.0);
        EXPECT_FLOAT_EQ(value_type(1.0) / value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / value_type(3.0), d.dual());
    }
    {
        auto d = Dual(value_type(-1.0), value_type(2.0));
        d /= value_type(0.0);
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TEST_F(DualfTest, is_zero)
{
    EXPECT_TRUE(Dual(value_type(0.0), value_type(0.0)).is_zero(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(1.0), value_type(0.0)).is_zero(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(0.0), value_type(2.0)).is_zero(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(1.0), value_type(2.0)).is_zero(abs_tolerance));
}

TEST_F(DualfTest, is_real)
{
    EXPECT_TRUE(Dual(value_type(0.0), value_type(0.0)).is_real(abs_tolerance));
    EXPECT_TRUE(Dual(value_type(1.0), value_type(0.0)).is_real(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(0.0), value_type(2.0)).is_real(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(1.0), value_type(2.0)).is_real(abs_tolerance));
}

TEST_F(DualfTest, is_pure)
{
    EXPECT_TRUE(Dual(value_type(0.0), value_type(0.0)).is_pure(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(1.0), value_type(0.0)).is_pure(abs_tolerance));
    EXPECT_TRUE(Dual(value_type(0.0), value_type(2.0)).is_pure(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(1.0), value_type(2.0)).is_pure(abs_tolerance));
}

TEST_F(DualfTest, is_inf)
{
    EXPECT_FALSE(Dual(value_type(1.0), value_type(2.0)).is_inf());
    EXPECT_TRUE(Dual(nlimits::infinity(), value_type(2.0)).is_inf());
    EXPECT_TRUE(Dual(value_type(1.0), nlimits::infinity()).is_inf());
}

TEST_F(DualfTest, is_nan)
{
    EXPECT_FALSE(Dual(value_type(1.0), value_type(2.0)).is_nan());
    EXPECT_TRUE(Dual(nlimits::quiet_NaN(), value_type(2.0)).is_nan());
    EXPECT_TRUE(Dual(value_type(1.0), nlimits::quiet_NaN()).is_nan());
    EXPECT_TRUE(Dual(nlimits::signaling_NaN(), value_type(2.0)).is_nan());
    EXPECT_TRUE(Dual(value_type(1.0), nlimits::signaling_NaN()).is_nan());
}

TEST_F(DualfTest, is_unit)
{
    EXPECT_FALSE(Dual(value_type(0.0), value_type(0.0)).is_unit(abs_tolerance));
    EXPECT_TRUE(Dual(value_type(1.0), value_type(0.0)).is_unit(abs_tolerance));
    EXPECT_FALSE(Dual(value_type(0.0), value_type(2.0)).is_unit(abs_tolerance));
    EXPECT_TRUE(Dual(value_type(1.0), value_type(2.0)).is_unit(abs_tolerance));
}

TEST_F(DualfTest, is_close_to)
{
    {
        auto d = Dual(value_type(0.0), value_type(0.0));

        EXPECT_TRUE(d.is_close_to(Dual(value_type(0.0), value_type(0.0)), rel_tolerance, abs_tolerance));
        EXPECT_TRUE(d.is_close_to(Dual(value_type(0.0), value_type(0.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(1.0), value_type(0.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(1.0), value_type(0.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(0.0), value_type(1.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(0.0), value_type(1.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(1.0), value_type(1.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(1.0), value_type(1.0)), abs_tolerance));
    }
    {
        auto d = Dual(value_type(100.0), value_type(200.0));

        EXPECT_TRUE(d.is_close_to(Dual(value_type(100.0), value_type(200.0)), rel_tolerance, abs_tolerance));
        EXPECT_TRUE(d.is_close_to(Dual(value_type(100.0), value_type(200.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(100.0), value_type(0.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(100.0), value_type(0.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(0.0), value_type(200.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(0.0), value_type(200.0)), abs_tolerance));

        EXPECT_FALSE(d.is_close_to(Dual(value_type(200.0), value_type(100.0)), rel_tolerance, abs_tolerance));
        EXPECT_FALSE(d.is_close_to(Dual(value_type(200.0), value_type(100.0)), abs_tolerance));
    }
}

TEST_F(DualfTest, UnaryPlus)
{
    auto d = + Dual(value_type(1.0), value_type(2.0));
    EXPECT_FLOAT_EQ(value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(value_type(2.0), d.dual());
}

TEST_F(DualfTest, UnaryMinus)
{
    auto d = - Dual(value_type(1.0), value_type(2.0));
    EXPECT_FLOAT_EQ(-value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(2.0), d.dual());
}

TEST_F(DualfTest, Addition)
{
    auto d = Dual(value_type(1.0), value_type(2.0)) + Dual(value_type(3.0), value_type(4.0));
    EXPECT_FLOAT_EQ(value_type(4.0), d.real());
    EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
}

TEST_F(DualfTest, Subtraction)
{
    auto d = Dual(value_type(1.0), value_type(2.0)) - Dual(value_type(3.0), value_type(4.0));
    EXPECT_FLOAT_EQ(-value_type(2.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(2.0), d.dual());
}

TEST_F(DualfTest, Multiplication)
{
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) * Dual(value_type(3.0), value_type(4.0));
        EXPECT_FLOAT_EQ(value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(10.0), d.dual());
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) * value_type(3.0);
        EXPECT_FLOAT_EQ(value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
    }
    {
        auto d = value_type(3.0) * Dual(value_type(1.0), value_type(2.0));
        EXPECT_FLOAT_EQ(value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
    }
}

TEST_F(DualfTest, Division)
{
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) / Dual(value_type(3.0), value_type(4.0));
        EXPECT_FLOAT_EQ(value_type(1.0) / value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / value_type(9.0), d.dual());
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) / Dual(value_type(0.0), value_type(4.0));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) / value_type(3.0);
        EXPECT_FLOAT_EQ(value_type(1.0) / value_type(3.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / value_type(3.0), d.dual());
    }
    {
        auto d = Dual(value_type(1.0), value_type(2.0)) / value_type(0.0);
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TEST_F(DualfTest, abs)
{
    EXPECT_FLOAT_EQ(value_type(0.0), dual::abs(Dual(value_type(0.0), value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(3.0), dual::abs(Dual(value_type(3.0), value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(0.0), dual::abs(Dual(value_type(0.0), value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(3.0), dual::abs(Dual(value_type(3.0), value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(3.0), dual::abs(Dual(-value_type(3.0), -value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(0.0), dual::abs(Dual(-value_type(0.0), -value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(3.0), dual::abs(Dual(-value_type(3.0), -value_type(4.0))));
}

TEST_F(DualfTest, norm)
{
    EXPECT_FLOAT_EQ(value_type(0.0), dual::norm(Dual(value_type(0.0), value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(9.0), dual::norm(Dual(value_type(3.0), value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(0.0), dual::norm(Dual(value_type(0.0), value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(9.0), dual::norm(Dual(value_type(3.0), value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(9.0), dual::norm(Dual(-value_type(3.0), -value_type(0.0))));
    EXPECT_FLOAT_EQ(value_type(0.0), dual::norm(Dual(-value_type(0.0), -value_type(4.0))));
    EXPECT_FLOAT_EQ(value_type(9.0), dual::norm(Dual(-value_type(3.0), -value_type(4.0))));
}

TEST_F(DualfTest, conjugate)
{
    auto d = dual::conjugate(Dual(value_type(0.0), value_type(0.0)));
    EXPECT_FLOAT_EQ(value_type(0.0), d.real());
    EXPECT_FLOAT_EQ(value_type(0.0), d.dual());

    d = dual::conjugate(Dual(value_type(1.0), value_type(0.0)));
    EXPECT_FLOAT_EQ(value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(value_type(0.0), d.dual());

    d = dual::conjugate(Dual(value_type(0.0), value_type(2.0)));
    EXPECT_FLOAT_EQ(value_type(0.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(2.0), d.dual());

    d = dual::conjugate(Dual(value_type(1.0), value_type(2.0)));
    EXPECT_FLOAT_EQ(value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(2.0), d.dual());

    d = dual::conjugate(Dual(-value_type(1.0), -value_type(0.0)));
    EXPECT_FLOAT_EQ(-value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(-value_type(0.0), d.dual());

    d = dual::conjugate(Dual(-value_type(0.0), -value_type(2.0)));
    EXPECT_FLOAT_EQ(-value_type(0.0), d.real());
    EXPECT_FLOAT_EQ(value_type(2.0), d.dual());

    d = dual::conjugate(Dual(-value_type(1.0), -value_type(2.0)));
    EXPECT_FLOAT_EQ(-value_type(1.0), d.real());
    EXPECT_FLOAT_EQ(value_type(2.0), d.dual());
}

TEST_F(DualfTest, inverse)
{
    {
        auto temp = Dual(value_type(3.0), value_type(4.0));

        auto d = temp * dual::inverse(temp);
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());

        d = dual::inverse(temp) * temp;
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::inverse(Dual(value_type(0.0), value_type(4.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TEST_F(DualfTest, normalize)
{
    auto src = Dual(value_type(3.0), value_type(4.0));
    auto dst = dual::abs(src) * dual::normalize(src);
    EXPECT_FLOAT_EQ(src.real(), dst.real());
    EXPECT_FLOAT_EQ(src.dual(), dst.dual());
}

TEST_F(DualfTest, sin)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(-value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(-value_type(1.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(0.0), d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(value_type(1.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(0.0), d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(-value_type(2.0), d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, cos)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(-value_type(1.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(0.0), d.dual(), abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(value_type(1.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(0.0), d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(-value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(-value_type(1.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(0.0), d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, tan)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::tan(Dual(real, dual));
#if defined(__GNUC__) && !defined(__clang__)
        if(std::abs(std::tan(real) - d.real()) >= value_type(2.0))
        {
            GTEST_COUT << "This environment causes unintended computational errors, so the test is skipped."
                << std::endl;
        }
        else
#endif
        {
            EXPECT_NEAR(std::tan(real), d.real(), abs_tolerance);
        }
        auto expected = value_type(1.0) / (std::cos(real) * std::cos(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(2.0), d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::tan(Dual(real, dual));
#if defined(__GNUC__) && !defined(__clang__)
        if(std::abs(std::tan(real) - d.real()) >= value_type(2.0))
        {
            GTEST_COUT << "This environment causes unintended computational errors, so the test is skipped."
                << std::endl;
        }
        else
#endif
        {
            EXPECT_NEAR(std::tan(real), d.real(), abs_tolerance);
        }
        auto expected = value_type(1.0) / (std::cos(real) * std::cos(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(-value_type(0.0), d.real(), abs_tolerance);
        EXPECT_NEAR(value_type(2.0), d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, asin)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * -pi/2 < z.real < +pi/2
     */

    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    // theta = -pi/4
    {
        const auto real = std::atan2(-value_type(1.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/4
    {
        const auto real = std::atan2(value_type(1.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TEST_F(DualfTest, acos)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * 0 < z.real < +pi
     */

    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    // theta = pi/4
    {
        const auto real = std::atan2(value_type(1.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = 3pi/4
    {
        const auto real = std::atan2(value_type(1.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TEST_F(DualfTest, atan)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * -pi/2 < z.real < +pi/2
     */

    constexpr auto coef = 9.999999e-1f;

    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0)) * coef;
        const auto dual = value_type(2.0);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = -pi/4
    {
        const auto real = std::atan2(-value_type(1.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/4
    {
        const auto real = std::atan2(value_type(1.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0)) * coef;
        const auto dual = value_type(2.0);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, sinh)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, abs_tolerance);
    }
}

TEST_F(DualfTest, cosh)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), abs_tolerance);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, abs_tolerance);
    }
}

TEST_F(DualfTest, tanh)
{
    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), abs_tolerance);
        auto expected = value_type(1.0) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), abs_tolerance);
        auto expected = value_type(1.0) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), abs_tolerance);
        auto expected = value_type(1.0) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), abs_tolerance);
        auto expected = value_type(1.0) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), abs_tolerance);
        auto expected = value_type(1.0) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, abs_tolerance);
    }
}

TEST_F(DualfTest, asinh)
{
    /*
     * w = sinh(z)
     * z = asinh(w)
     */

    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, acosh)
{
    /*
     * w = cosh(z)
     * z = acosh(w)
     * 0 < z.real
     */

    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, atanh)
{
    /*
     * w = tanh(z)
     * z = atanh(w)
     */

    // theta = -pi
    {
        const auto real = std::atan2(-value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = 0
    {
        const auto real = std::atan2(value_type(0.0), value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(value_type(1.0), value_type(0.0));
        const auto dual = value_type(2.0);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
    // theta = pi
    {
        const auto real = std::atan2(value_type(0.0), -value_type(1.0));
        const auto dual = value_type(2.0);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), abs_tolerance);
        EXPECT_NEAR(dual, d.dual(), abs_tolerance);
    }
}

TEST_F(DualfTest, exp)
{
    {
        auto d = dual::exp(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::exp(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(std::exp(value_type(1.0)), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::exp(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0), d.dual());
    }
    {
        const auto e = std::exp(value_type(1.0));
        auto d = dual::exp(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(e, d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) * e, d.dual());
    }
}

TEST_F(DualfTest, log)
{
    {
        auto d = dual::log(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::log(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0), d.dual());
    }
}

TEST_F(DualfTest, log2)
{
    {
        auto d = dual::log2(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log2(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::log2(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log2(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / std::log(value_type(2.0)), d.dual());
    }
}

TEST_F(DualfTest, log10)
{
    {
        auto d = dual::log10(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log10(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::log10(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log10(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0) / std::log(value_type(10.0)), d.dual());
    }
}

TEST_F(DualfTest, pow)
{
    {
        auto z = Dual(value_type(0.0), value_type(0.0));
        auto w = Dual(value_type(0.0), value_type(0.0));
        auto d = dual::pow(z, w);
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));

        d = dual::pow(z, value_type(0.0));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto z = Dual(value_type(0.0), value_type(0.0));
        auto w = Dual(value_type(1.0), value_type(2.0));
        auto d = dual::pow(z, w);
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));

        d = dual::pow(z, value_type(1.0));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto z = Dual(value_type(1.0), value_type(2.0));
        auto w = Dual(value_type(0.0), value_type(0.0));
        auto d = dual::pow(z, w);
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());

        d = dual::pow(z, value_type(0.0));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto z = Dual(value_type(1.0), value_type(2.0));
        auto w = Dual(value_type(1.0), value_type(2.0));
        auto d = dual::pow(z, w);
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0), d.dual());

        d = dual::pow(z, value_type(1.0));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(2.0), d.dual());
    }
}

TEST_F(DualfTest, square)
{
    {
        auto d = dual::square(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::square(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::square(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::square(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(4.0), d.dual());
    }
}

TEST_F(DualfTest, sqrt)
{
    {
        auto d = dual::sqrt(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::sqrt(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::sqrt(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::sqrt(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(1.0), d.dual());
    }
}

TEST_F(DualfTest, cube)
{
    {
        auto d = dual::cube(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::cube(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::cube(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::cube(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(6.0), d.dual());
    }
}

TEST_F(DualfTest, cbrt)
{
    {
        auto d = dual::cbrt(Dual(value_type(0.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::cbrt(Dual(value_type(1.0), value_type(0.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        EXPECT_FLOAT_EQ(value_type(0.0), d.dual());
    }
    {
        auto d = dual::cbrt(Dual(value_type(0.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(0.0), d.real());
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::cbrt(Dual(value_type(1.0), value_type(2.0)));
        EXPECT_FLOAT_EQ(value_type(1.0), d.real());
        auto expected = value_type(2.0) / value_type(3.0);
        EXPECT_FLOAT_EQ(expected, d.dual());
    }
}

}
