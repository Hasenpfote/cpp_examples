#include <dual.h>

namespace
{

using value_type = double;
using Dual = dual::Dual<value_type>;
using nlimits = std::numeric_limits<value_type>;

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
void constexpr_test()
{
    constexpr Dual z(1.0, 2.0);
    constexpr Dual w(3.0, 4.0);

    //
    {
        constexpr auto real = z.real();
        (void)real;
        constexpr auto dual = z.dual();
        (void)dual;
    }
    // is_zero
    {
        constexpr auto tolerance = nlimits::epsilon();
        static_assert(Dual(0.0, 0.0).is_zero(tolerance) == true, "");
        static_assert(Dual(1.0, 0.0).is_zero(tolerance) == false, "");
        static_assert(Dual(0.0, 1.0).is_zero(tolerance) == false, "");
        static_assert(Dual(1.0, 1.0).is_zero(tolerance) == false, "");
    }
    // is_real
    {
        constexpr auto tolerance = nlimits::epsilon();
        static_assert(Dual(0.0, 0.0).is_real(tolerance) == true, "");
        static_assert(Dual(1.0, 0.0).is_real(tolerance) == true, "");
        static_assert(Dual(0.0, 1.0).is_real(tolerance) == false, "");
        static_assert(Dual(1.0, 1.0).is_real(tolerance) == false, "");
    }
    // is_pure
    {
        constexpr auto tolerance = nlimits::epsilon();
        static_assert(Dual(0.0, 0.0).is_pure(tolerance) == true, "");
        static_assert(Dual(1.0, 0.0).is_pure(tolerance) == false, "");
        static_assert(Dual(0.0, 1.0).is_pure(tolerance) == true, "");
        static_assert(Dual(1.0, 1.0).is_pure(tolerance) == false, "");
    }
    // is_unit
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    {
        constexpr auto tolerance = nlimits::epsilon();
        static_assert(Dual(0.0, 0.0).is_unit(tolerance) == false, "");
        static_assert(Dual(1.0, 0.0).is_unit(tolerance) == true, "");
        static_assert(Dual(0.0, 1.0).is_unit(tolerance) == false, "");
        static_assert(Dual(1.0, 1.0).is_unit(tolerance) == true, "");
    }
#endif
    // is_close_to
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    {
        constexpr auto rel_tolerance = 1e-5;
        constexpr auto abs_tolerance = 1e-8;
        static_assert(z.is_close_to(Dual(1.0, 2.0), rel_tolerance, abs_tolerance) == true, "");
        static_assert(w.is_close_to(Dual(3.0, 4.0), rel_tolerance, abs_tolerance) == true, "");
        static_assert(z.is_close_to(w, rel_tolerance, abs_tolerance) == false, "");
    }
#endif
    // Unary plus/minus
    {
        constexpr auto tolerance = nlimits::epsilon();
        constexpr auto d1 = +z;
        constexpr auto d2 = -z;
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
        static_assert(dual::detail::are_close(z.real(), d1.real(), tolerance) == true, "");
        static_assert(dual::detail::are_close(z.dual(), d1.dual(), tolerance) == true, "");
        static_assert(dual::detail::are_close(-z.real(), d2.real(), tolerance) == true, "");
        static_assert(dual::detail::are_close(-z.dual(), d2.dual(), tolerance) == true, "");
#else
        (void)d1;
        (void)d2;
#endif
    }
    // Addition
    {
        constexpr auto d = z + w;
        //static_assert(d.real() == value_type(4.0), "");
        (void)d;
    }
    // Subtraction
    {
        constexpr auto d = z - w;
        (void)d;
    }
    // Multiplication
    {
        constexpr auto d1 = z * w;
        (void)d1;
        constexpr auto d2 = z * value_type(3.0);
        (void)d2;
        constexpr auto d3 = value_type(3.0) * z;
        (void)d3;
    }
    // Division
    {
        constexpr auto d1 = z / w;
        (void)d1;
        constexpr auto d2 = z / value_type(3.0);
        (void)d2;
    }
    // abs
    {
        constexpr auto s = dual::abs(z);
        (void)s;
    }
    // norm
    {
        constexpr auto s = dual::norm(z);
        (void)s;
    }
    // conjugate
    {
        constexpr auto d = dual::conjugate(z);
        (void)d;
    }
    // inverse
    {
        constexpr auto d = dual::inverse(z);
        (void)d;
    }
    // normalize
    {
        constexpr auto d = dual::normalize(w);
        (void)d;
    }
    // square/cube
    {
        constexpr auto d1 = dual::square(z);
        (void)d1;
        constexpr auto d2 = dual::cube(z);
        (void)d2;
    }
}
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

}   // namespace
