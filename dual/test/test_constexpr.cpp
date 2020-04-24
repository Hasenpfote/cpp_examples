#include <dual.h>

#define STATIC_ASSERT_EXPECT_NEAR(expected, actual, rel_tol, abs_tol) \
    static_assert(dual::detail::are_close(expected, actual, rel_tol, abs_tol), #expected " == " #actual)

#define STATIC_ASSERT_EXPECT_TRUE(condition) \
    static_assert(condition, #condition)

#define STATIC_ASSERT_EXPECT_FALSE(condition) \
    static_assert(!condition, "!" #condition)

namespace dual_tests
{

using Dual = dual::Dual<double>;

constexpr auto rel_tolerance = 1e-5;
constexpr auto abs_tolerance = 1e-8;

constexpr Dual z(1.0, 2.0);
constexpr Dual w(3.0, 4.0);

// getter
namespace getter
{
    STATIC_ASSERT_EXPECT_NEAR(1.0, z.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(2.0, z.dual(), rel_tolerance, abs_tolerance);

    STATIC_ASSERT_EXPECT_NEAR(3.0, w.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(4.0, w.dual(), rel_tolerance, abs_tolerance);
}
// is_zero
namespace is_zero
{
    STATIC_ASSERT_EXPECT_TRUE(Dual(0.0, 0.0).is_zero(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(1.0, 0.0).is_zero(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(0.0, 1.0).is_zero(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(1.0, 1.0).is_zero(abs_tolerance));
}
// is_real
namespace is_real
{
    STATIC_ASSERT_EXPECT_TRUE(Dual(0.0, 0.0).is_real(abs_tolerance));
    STATIC_ASSERT_EXPECT_TRUE(Dual(1.0, 0.0).is_real(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(0.0, 1.0).is_real(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(1.0, 1.0).is_real(abs_tolerance));
}
// is_pure
namespace is_pure
{
    STATIC_ASSERT_EXPECT_TRUE(Dual(0.0, 0.0).is_pure(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(1.0, 0.0).is_pure(abs_tolerance));
    STATIC_ASSERT_EXPECT_TRUE(Dual(0.0, 1.0).is_pure(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(1.0, 1.0).is_pure(abs_tolerance));
}
// is_unit
namespace is_unit
{
    STATIC_ASSERT_EXPECT_FALSE(Dual(0.0, 0.0).is_unit(abs_tolerance));
    STATIC_ASSERT_EXPECT_TRUE(Dual(1.0, 0.0).is_unit(abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(Dual(0.0, 1.0).is_unit(abs_tolerance));
    STATIC_ASSERT_EXPECT_TRUE(Dual(1.0, 1.0).is_unit(abs_tolerance));
}
// is_close_to
namespace is_close_to
{
    STATIC_ASSERT_EXPECT_TRUE(z.is_close_to(Dual(1.0, 2.0), rel_tolerance, abs_tolerance));
    STATIC_ASSERT_EXPECT_TRUE(w.is_close_to(Dual(3.0, 4.0), rel_tolerance, abs_tolerance));
    STATIC_ASSERT_EXPECT_FALSE(z.is_close_to(w, rel_tolerance, abs_tolerance));
}
// Unary plus
namespace unary_plus
{
    constexpr auto d1 = +z;
    STATIC_ASSERT_EXPECT_NEAR(z.real(), d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(z.dual(), d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = +w;
    STATIC_ASSERT_EXPECT_NEAR(w.real(), d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(w.dual(), d2.dual(), rel_tolerance, abs_tolerance);
}
// Unary plus
namespace unary_minus
{
    constexpr auto d1 = -z;
    STATIC_ASSERT_EXPECT_NEAR(-z.real(), d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(-z.dual(), d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = -w;
    STATIC_ASSERT_EXPECT_NEAR(-w.real(), d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(-w.dual(), d2.dual(), rel_tolerance, abs_tolerance);
}
// Addition
namespace addition
{
    constexpr auto d = z + w;
    STATIC_ASSERT_EXPECT_NEAR(4.0, d.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(6.0, d.dual(), rel_tolerance, abs_tolerance);
}
// Subtraction
namespace subtraction
{
    constexpr auto d = z - w;
    STATIC_ASSERT_EXPECT_NEAR(-2.0, d.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(-2.0, d.dual(), rel_tolerance, abs_tolerance);
}
// Multiplication
namespace multiplication
{
    constexpr auto d1 = z * w;
    STATIC_ASSERT_EXPECT_NEAR(3.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(10.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = z * 3.0;
    STATIC_ASSERT_EXPECT_NEAR(3.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(6.0, d2.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d3 = 3.0 * z;
    STATIC_ASSERT_EXPECT_NEAR(3.0, d3.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(6.0, d3.dual(), rel_tolerance, abs_tolerance);
}
// Division
namespace division
{
    constexpr auto d1 = z / w;
    STATIC_ASSERT_EXPECT_NEAR(1.0 / 3.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(2.0 / 9.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = z / 3.0;
    STATIC_ASSERT_EXPECT_NEAR(1.0 / 3.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(2.0 / 3.0, d2.dual(), rel_tolerance, abs_tolerance);
}
// abs
namespace abs
{
    constexpr auto s1 = dual::abs(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, s1, rel_tolerance, abs_tolerance);

    constexpr auto s2 = dual::abs(w);
    STATIC_ASSERT_EXPECT_NEAR(3.0, s2, rel_tolerance, abs_tolerance);
}
// norm
namespace norm
{
    constexpr auto s1 = dual::norm(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, s1, rel_tolerance, abs_tolerance);

    constexpr auto s2 = dual::norm(w);
    STATIC_ASSERT_EXPECT_NEAR(9.0, s2, rel_tolerance, abs_tolerance);
}
// conjugate
namespace conjugate
{
    constexpr auto d1 = dual::conjugate(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(-2.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = dual::conjugate(w);
    STATIC_ASSERT_EXPECT_NEAR(3.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(-4.0, d2.dual(), rel_tolerance, abs_tolerance);
}
// inverse
namespace inverse
{
    constexpr auto d1l = dual::inverse(z) * z;
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1l.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(0.0, d1l.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d1r = z * dual::inverse(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1r.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(0.0, d1r.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2l = dual::inverse(w) * w;
    STATIC_ASSERT_EXPECT_NEAR(1.0, d2l.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(0.0, d2l.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2r = w * dual::inverse(w);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d2r.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(0.0, d2r.dual(), rel_tolerance, abs_tolerance);
}
// normalize
namespace normalize
{
    constexpr auto d1 = dual::normalize(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(2.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = dual::normalize(w);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(4.0 / 3.0, d2.dual(), rel_tolerance, abs_tolerance);
}
// square
namespace square
{
    constexpr auto d1 = dual::square(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(4.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = dual::square(w);
    STATIC_ASSERT_EXPECT_NEAR(9.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(24.0, d2.dual(), rel_tolerance, abs_tolerance);
}
// cube
namespace cube
{
    constexpr auto d1 = dual::cube(z);
    STATIC_ASSERT_EXPECT_NEAR(1.0, d1.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(6.0, d1.dual(), rel_tolerance, abs_tolerance);

    constexpr auto d2 = dual::cube(w);
    STATIC_ASSERT_EXPECT_NEAR(27.0, d2.real(), rel_tolerance, abs_tolerance);
    STATIC_ASSERT_EXPECT_NEAR(108.0, d2.dual(), rel_tolerance, abs_tolerance);
}

}   // namespace dual_tests
