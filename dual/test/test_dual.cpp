#include <gtest/gtest.h>
#include <dual.h>

namespace
{

template<typename T>
class DualTest
    : public ::testing::Test
{
protected:
    static const T PI;

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    absolute_tolerance(){ return 1e-3f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    absolute_tolerance(){ return 1e-8; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    relative_tolerance(){ return 1e-5f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    relative_tolerance(){ return 1e-5; }
};

template<typename T>
const T DualTest<T>::PI = std::acos(-T(1));

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualTest, MyTypes);

TYPED_TEST(DualTest, AdditionAssignment)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = Dual(TypeParam(1), TypeParam(2));
    d += Dual(TypeParam(3), TypeParam(4));
    EXPECT_NEAR(TypeParam(4), d.real(), atol);
    EXPECT_NEAR(TypeParam(6), d.dual(), atol);
}

TYPED_TEST(DualTest, SubtractionAssignment)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = Dual(TypeParam(1), TypeParam(2));
    d -= Dual(TypeParam(3), TypeParam(4));
    EXPECT_NEAR(-TypeParam(2), d.real(), atol);
    EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
}

TYPED_TEST(DualTest, MultiplicationAssignment)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = Dual(TypeParam(1), TypeParam(2));
        d *= Dual(TypeParam(3), TypeParam(4));
        EXPECT_NEAR(TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(10), d.dual(), atol);
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2));
        d *= TypeParam(3);
        EXPECT_NEAR(TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(6), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, DivisionAssignment)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = Dual(TypeParam(1), TypeParam(2));
        d /= Dual(TypeParam(3), TypeParam(4));
        EXPECT_NEAR(TypeParam(1) / TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / TypeParam(9), d.dual(), atol);
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2));
        d /= Dual(TypeParam(0), TypeParam(4));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2));
        d /= TypeParam(3);
        EXPECT_NEAR(TypeParam(1) / TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / TypeParam(3), d.dual(), atol);
    }
    {
        auto d = Dual(-TypeParam(1), TypeParam(2));
        d /= TypeParam(0);
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TYPED_TEST(DualTest, is_zero)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_TRUE(Dual(TypeParam(0), TypeParam(0)).is_zero(atol));
    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(0)).is_zero(atol));
    EXPECT_FALSE(Dual(TypeParam(0), TypeParam(2)).is_zero(atol));
    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(2)).is_zero(atol));
}

TYPED_TEST(DualTest, is_real)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_TRUE(Dual(TypeParam(0), TypeParam(0)).is_real(atol));
    EXPECT_TRUE(Dual(TypeParam(1), TypeParam(0)).is_real(atol));
    EXPECT_FALSE(Dual(TypeParam(0), TypeParam(2)).is_real(atol));
    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(2)).is_real(atol));
}

TYPED_TEST(DualTest, is_pure)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_TRUE(Dual(TypeParam(0), TypeParam(0)).is_pure(atol));
    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(0)).is_pure(atol));
    EXPECT_TRUE(Dual(TypeParam(0), TypeParam(2)).is_pure(atol));
    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(2)).is_pure(atol));
}

TYPED_TEST(DualTest, is_inf)
{
    using Dual = dual::Dual<TypeParam>;
    using nlimits = std::numeric_limits<TypeParam>;

    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(2)).is_inf());
    EXPECT_TRUE(Dual(nlimits::infinity(), TypeParam(2)).is_inf());
    EXPECT_TRUE(Dual(TypeParam(1), nlimits::infinity()).is_inf());
}

TYPED_TEST(DualTest, is_nan)
{
    using Dual = dual::Dual<TypeParam>;
    using nlimits = std::numeric_limits<TypeParam>;

    EXPECT_FALSE(Dual(TypeParam(1), TypeParam(2)).is_nan());
    EXPECT_TRUE(Dual(nlimits::quiet_NaN(), TypeParam(2)).is_nan());
    EXPECT_TRUE(Dual(TypeParam(1), nlimits::quiet_NaN()).is_nan());
    EXPECT_TRUE(Dual(nlimits::signaling_NaN(), TypeParam(2)).is_nan());
    EXPECT_TRUE(Dual(TypeParam(1), nlimits::signaling_NaN()).is_nan());
}

TYPED_TEST(DualTest, is_unit)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_FALSE(Dual(TypeParam(0), TypeParam(0)).is_unit(atol));
    EXPECT_TRUE(Dual(TypeParam(1), TypeParam(0)).is_unit(atol));
    EXPECT_FALSE(Dual(TypeParam(0), TypeParam(2)).is_unit(atol));
    EXPECT_TRUE(Dual(TypeParam(1), TypeParam(2)).is_unit(atol));
}

TYPED_TEST(DualTest, is_close_to)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();
    constexpr auto rtol = DualTest<TypeParam>::relative_tolerance();

    {
        auto d = Dual(TypeParam(0), TypeParam(0));

        EXPECT_TRUE(d.is_close_to(Dual(TypeParam(0), TypeParam(0)), rtol, atol));
        EXPECT_TRUE(d.is_close_to(Dual(TypeParam(0), TypeParam(0)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(1), TypeParam(0)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(1), TypeParam(0)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(0), TypeParam(1)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(0), TypeParam(1)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(1), TypeParam(1)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(1), TypeParam(1)), atol));
    }
    {
        auto d = Dual(TypeParam(100), TypeParam(200));

        EXPECT_TRUE(d.is_close_to(Dual(TypeParam(100), TypeParam(200)), rtol, atol));
        EXPECT_TRUE(d.is_close_to(Dual(TypeParam(100), TypeParam(200)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(100), TypeParam(0)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(100), TypeParam(0)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(0), TypeParam(200)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(0), TypeParam(200)), atol));

        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(200), TypeParam(100)), rtol, atol));
        EXPECT_FALSE(d.is_close_to(Dual(TypeParam(200), TypeParam(100)), atol));
    }
}

TYPED_TEST(DualTest, UnaryPlus)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = + Dual(TypeParam(1), TypeParam(2));
    EXPECT_NEAR(TypeParam(1), d.real(), atol);
    EXPECT_NEAR(TypeParam(2), d.dual(), atol);
}

TYPED_TEST(DualTest, UnaryMinus)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = - Dual(TypeParam(1), TypeParam(2));
    EXPECT_NEAR(-TypeParam(1), d.real(), atol);
    EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
}

TYPED_TEST(DualTest, Addition)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = Dual(TypeParam(1), TypeParam(2)) + Dual(TypeParam(3), TypeParam(4));
    EXPECT_NEAR(TypeParam(4), d.real(), atol);
    EXPECT_NEAR(TypeParam(6), d.dual(), atol);
}

TYPED_TEST(DualTest, Subtraction)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = Dual(TypeParam(1), TypeParam(2)) - Dual(TypeParam(3), TypeParam(4));
    EXPECT_NEAR(-TypeParam(2), d.real(), atol);
    EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
}

TYPED_TEST(DualTest, Multiplication)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = Dual(TypeParam(1), TypeParam(2)) * Dual(TypeParam(3), TypeParam(4));
        EXPECT_NEAR(TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(10), d.dual(), atol);
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2)) * TypeParam(3);
        EXPECT_NEAR(TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(6), d.dual(), atol);
    }
    {
        auto d = TypeParam(3) * Dual(TypeParam(1), TypeParam(2));
        EXPECT_NEAR(TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(6), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, Division)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = Dual(TypeParam(1), TypeParam(2)) / Dual(TypeParam(3), TypeParam(4));
        EXPECT_NEAR(TypeParam(1) / TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / TypeParam(9), d.dual(), atol);
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2)) / Dual(TypeParam(0), TypeParam(4));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2)) / TypeParam(3);
        EXPECT_NEAR(TypeParam(1) / TypeParam(3), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / TypeParam(3), d.dual(), atol);
    }
    {
        auto d = Dual(TypeParam(1), TypeParam(2)) / TypeParam(0);
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TYPED_TEST(DualTest, abs)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_NEAR(TypeParam(0), dual::abs(Dual(TypeParam(0), TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(3), dual::abs(Dual(TypeParam(3), TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(0), dual::abs(Dual(TypeParam(0), TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(3), dual::abs(Dual(TypeParam(3), TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(3), dual::abs(Dual(-TypeParam(3), -TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(0), dual::abs(Dual(-TypeParam(0), -TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(3), dual::abs(Dual(-TypeParam(3), -TypeParam(4))), atol);
}

TYPED_TEST(DualTest, norm)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    EXPECT_NEAR(TypeParam(0), dual::norm(Dual(TypeParam(0), TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(9), dual::norm(Dual(TypeParam(3), TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(0), dual::norm(Dual(TypeParam(0), TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(9), dual::norm(Dual(TypeParam(3), TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(9), dual::norm(Dual(-TypeParam(3), -TypeParam(0))), atol);
    EXPECT_NEAR(TypeParam(0), dual::norm(Dual(-TypeParam(0), -TypeParam(4))), atol);
    EXPECT_NEAR(TypeParam(9), dual::norm(Dual(-TypeParam(3), -TypeParam(4))), atol);
}

TYPED_TEST(DualTest, conjugate)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto d = dual::conjugate(Dual(TypeParam(0), TypeParam(0)));
    EXPECT_NEAR(TypeParam(0), d.real(), atol);
    EXPECT_NEAR(TypeParam(0), d.dual(), atol);

    d = dual::conjugate(Dual(TypeParam(1), TypeParam(0)));
    EXPECT_NEAR(TypeParam(1), d.real(), atol);
    EXPECT_NEAR(TypeParam(0), d.dual(), atol);

    d = dual::conjugate(Dual(TypeParam(0), TypeParam(2)));
    EXPECT_NEAR(TypeParam(0), d.real(), atol);
    EXPECT_NEAR(-TypeParam(2), d.dual(), atol);

    d = dual::conjugate(Dual(TypeParam(1), TypeParam(2)));
    EXPECT_NEAR(TypeParam(1), d.real(), atol);
    EXPECT_NEAR(-TypeParam(2), d.dual(), atol);

    d = dual::conjugate(Dual(-TypeParam(1), -TypeParam(0)));
    EXPECT_NEAR(-TypeParam(1), d.real(), atol);
    EXPECT_NEAR(-TypeParam(0), d.dual(), atol);

    d = dual::conjugate(Dual(-TypeParam(0), -TypeParam(2)));
    EXPECT_NEAR(-TypeParam(0), d.real(), atol);
    EXPECT_NEAR(TypeParam(2), d.dual(), atol);

    d = dual::conjugate(Dual(-TypeParam(1), -TypeParam(2)));
    EXPECT_NEAR(-TypeParam(1), d.real(), atol);
    EXPECT_NEAR(TypeParam(2), d.dual(), atol);
}

TYPED_TEST(DualTest, inverse)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto temp = Dual(TypeParam(3), TypeParam(4));

        auto d = temp * dual::inverse(temp);
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);

        d = dual::inverse(temp) * temp;
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::inverse(Dual(TypeParam(0), TypeParam(4)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TYPED_TEST(DualTest, normalize)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    auto src = Dual(TypeParam(3), TypeParam(4));
    auto dst = dual::abs(src) * dual::normalize(src);
    EXPECT_NEAR(src.real(), dst.real(), atol);
    EXPECT_NEAR(src.dual(), dst.dual(), atol);
}

TYPED_TEST(DualTest, sin)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(-TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sin(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, cos)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(-TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(-TypeParam(2), d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cos(Dual(real, dual));
        EXPECT_NEAR(-TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, tan)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(std::tan(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cos(real) * std::cos(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(std::tan(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cos(real) * std::cos(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tan(Dual(real, dual));
        EXPECT_NEAR(-TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, asin)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * -pi/2 < z.real < +pi/2
     */
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    // theta = -pi/4
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/4
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::asin(dual::sin(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TYPED_TEST(DualTest, acos)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * 0 < z.real < +pi
     */
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    // theta = pi/4
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = 3pi/4
    {
        const auto real = std::atan2(TypeParam(1), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acos(dual::cos(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
}

TYPED_TEST(DualTest, atan)
{
    /*
     * w = tan(z)
     * z = atan(w)
     * -pi/2 < z.real < +pi/2
     */
    using Dual = dual::Dual<TypeParam>;
    using nlimits = std::numeric_limits<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();
    constexpr auto coef = TypeParam(1) - nlimits::epsilon();

    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0)) * coef;
        const auto dual = TypeParam(2);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = -pi/4
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/4
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0)) * coef;
        const auto dual = TypeParam(2);
        auto d = dual::atan(dual::tan(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
}

TYPED_TEST(DualTest, sinh)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), atol);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), atol);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), atol);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), atol);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::sinh(Dual(real, dual));
        EXPECT_NEAR(std::sinh(real), d.real(), atol);
        EXPECT_NEAR(std::cosh(real), d.dual() / dual, atol);
    }
}

TYPED_TEST(DualTest, cosh)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), atol);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), atol);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), atol);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), atol);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::cosh(Dual(real, dual));
        EXPECT_NEAR(std::cosh(real), d.real(), atol);
        EXPECT_NEAR(std::sinh(real), d.dual() / dual, atol);
    }
}

TYPED_TEST(DualTest, tanh)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::tanh(Dual(real, dual));
        EXPECT_NEAR(std::tanh(real), d.real(), atol);
        auto expected = TypeParam(1) / (std::cosh(real) * std::cosh(real));
        EXPECT_NEAR(expected, d.dual() / dual, atol);
    }
}

TYPED_TEST(DualTest, asinh)
{
    /*
     * w = sinh(z)
     * z = asinh(w)
     */
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::asinh(dual::sinh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
}

TYPED_TEST(DualTest, acosh)
{
    /*
     * w = cosh(z)
     * z = acosh(w)
     * 0 < z.real
     */
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::acosh(dual::cosh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
}

TYPED_TEST(DualTest, atanh)
{
    /*
     * w = tanh(z)
     * z = atanh(w)
     */
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    // theta = -pi
    {
        const auto real = std::atan2(-TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = -pi/2
    {
        const auto real = std::atan2(-TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = 0
    {
        const auto real = std::atan2(TypeParam(0), TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi/2
    {
        const auto real = std::atan2(TypeParam(1), TypeParam(0));
        const auto dual = TypeParam(2);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
    // theta = pi
    {
        const auto real = std::atan2(TypeParam(0), -TypeParam(1));
        const auto dual = TypeParam(2);
        auto d = dual::atanh(dual::tanh(Dual(real, dual)));
        EXPECT_NEAR(real, d.real(), atol);
        EXPECT_NEAR(dual, d.dual(), atol);
    }
}

TYPED_TEST(DualTest, exp)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::exp(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::exp(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(std::exp(TypeParam(1)), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::exp(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
    {
        const auto e = std::exp(TypeParam(1));
        auto d = dual::exp(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(e, d.real(), atol);
        EXPECT_NEAR(TypeParam(2) * e, d.dual(), atol);
    }
}

TYPED_TEST(DualTest, log)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::log(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::log(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, log2)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::log2(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log2(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::log2(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log2(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / std::log(TypeParam(2)), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, log10)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::log10(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::log10(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::log10(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_TRUE(std::isinf(d.real()));
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::log10(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(2) / std::log(TypeParam(10)), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, pow)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto z = Dual(TypeParam(0), TypeParam(0));
        auto w = Dual(TypeParam(0), TypeParam(0));
        auto d = dual::pow(z, w);
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));

        d = dual::pow(z, TypeParam(0));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto z = Dual(TypeParam(0), TypeParam(0));
        auto w = Dual(TypeParam(1), TypeParam(2));
        auto d = dual::pow(z, w);
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));

        d = dual::pow(z, TypeParam(1));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto z = Dual(TypeParam(1), TypeParam(2));
        auto w = Dual(TypeParam(0), TypeParam(0));
        auto d = dual::pow(z, w);
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);

        d = dual::pow(z, TypeParam(0));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto z = Dual(TypeParam(1), TypeParam(2));
        auto w = Dual(TypeParam(1), TypeParam(2));
        auto d = dual::pow(z, w);
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);

        d = dual::pow(z, TypeParam(1));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(2), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, square)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::square(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::square(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::square(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::square(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(4), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, sqrt)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::sqrt(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::sqrt(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::sqrt(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::sqrt(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(1), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, cube)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::cube(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::cube(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::cube(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::cube(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(6), d.dual(), atol);
    }
}

TYPED_TEST(DualTest, cbrt)
{
    using Dual = dual::Dual<TypeParam>;

    constexpr auto atol = DualTest<TypeParam>::absolute_tolerance();

    {
        auto d = dual::cbrt(Dual(TypeParam(0), TypeParam(0)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isnan(d.dual()));
    }
    {
        auto d = dual::cbrt(Dual(TypeParam(1), TypeParam(0)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        EXPECT_NEAR(TypeParam(0), d.dual(), atol);
    }
    {
        auto d = dual::cbrt(Dual(TypeParam(0), TypeParam(2)));
        EXPECT_NEAR(TypeParam(0), d.real(), atol);
        EXPECT_TRUE(std::isinf(d.dual()));
    }
    {
        auto d = dual::cbrt(Dual(TypeParam(1), TypeParam(2)));
        EXPECT_NEAR(TypeParam(1), d.real(), atol);
        auto expected = TypeParam(2) / TypeParam(3);
        EXPECT_NEAR(expected, d.dual(), atol);
    }
}

}
