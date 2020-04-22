#pragma once
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <limits>
#include <ostream>
#include <typeinfo>

namespace dual
{

namespace detail
{

template<typename T>
constexpr T
abs(T x)
{
    return x < T(0.0) ? -x : x;
}

template<typename T>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
constexpr
#endif
std::enable_if_t<std::is_floating_point<T>::value, bool>
are_close(T lhs, T rhs, T rel_tolerance, T abs_tolerance)
{
    return detail::abs(lhs - rhs)
        <= std::max(abs_tolerance, rel_tolerance * std::max(detail::abs(lhs), detail::abs(rhs)));
}

template<typename T>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
constexpr
#endif
std::enable_if_t<std::is_floating_point<T>::value, bool>
are_close(T lhs, T rhs, T tolerance)
{
    return detail::abs(lhs - rhs)
        <= tolerance * std::max(T(1.0), std::max(detail::abs(lhs), detail::abs(rhs)));
}

}   // namespace detail

template<typename T>
class Dual final
{
    static_assert(std::is_floating_point<T>::value,
        "Template parameter T must be floating_point type.");
public:
    using value_type = T;

/* Constructors */
    constexpr Dual(T real = T(), T dual = T())
        : real_(real), dual_(dual)
    {}

    constexpr Dual(const Dual&) = default;
    constexpr Dual& operator = (const Dual&) = default;

    constexpr Dual(Dual&&) = default;
    constexpr Dual& operator = (Dual&&) = default;

/* Destructor */
    ~Dual() = default;

/* Getters */
    constexpr T real() const noexcept { return real_; }
    constexpr T dual() const noexcept { return dual_; }

/* Setters */
    constexpr T& real() noexcept { return real_; }
    constexpr T& dual() noexcept { return dual_; }

/* Casting operators */

/* Assignment operators */
    constexpr Dual& operator += (const Dual&);
    constexpr Dual& operator -= (const Dual&);
    constexpr Dual& operator *= (const Dual&);
    constexpr Dual& operator *= (T);
    constexpr Dual& operator /= (const Dual&);
    constexpr Dual& operator /= (T);

/* Query functions */

    /*!
     * Returns true if this dual number is 0 + 0e and false otherwise.
     */
    constexpr bool is_zero(T tolerance) const;

    /*!
     * Returns true if this dual number has a zero dual component (of the form a + 0e) and false otherwise.
     */
    constexpr bool is_real(T tolerance) const;

    /*!
     * Returns true if this dual number has a zero real component (of the form 0 + be) and false otherwise.
     */
    constexpr bool is_pure(T tolerance) const;

    /*!
     * Returns true if either component of this is one of either +infinity or -infinity and false otherwise.
     */
    bool is_inf() const;

    /*!
     * Returns true if either component of this is NaN and false otherwise.
     */
    bool is_nan() const;

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
    constexpr
#endif
    bool is_unit(T tolerance) const;

/* Comparison functions */

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
    constexpr
#endif
    bool is_close_to(const Dual& other, T rel_tolerance, T abs_tolerance) const;

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
    constexpr
#endif
    bool is_close_to(const Dual& other, T tolerance) const;

private:
    T real_;
    T dual_;
};

/* Assignment operators */

template<typename T>
constexpr Dual<T>&
Dual<T>::operator += (const Dual<T>& rhs)
{
    real_ = real_ + rhs.real();
    dual_ = dual_ + rhs.dual();
    return *this;
}

template<typename T>
constexpr Dual<T>&
Dual<T>::operator -= (const Dual<T>& rhs)
{
    real_ = real_ - rhs.real();
    dual_ = dual_ - rhs.dual();
    return *this;
}

template<typename T>
constexpr Dual<T>&
Dual<T>::operator *= (const Dual<T>& rhs)
{
    auto temp = real_;
    real_ = temp * rhs.real(),
    dual_ = temp * rhs.dual() + dual_ * rhs.real();
    return *this;
}

template<typename T>
constexpr Dual<T>&
Dual<T>::operator *= (T rhs)
{
    real_ = real_ * rhs;
    dual_ = dual_ * rhs;
    return *this;
}

template<typename T>
constexpr Dual<T>&
Dual<T>::operator /= (const Dual<T>& rhs)
{
    auto temp = real_;
    real_ = temp / rhs.real();
    dual_ = (dual_ * rhs.real() - temp * rhs.dual()) / (rhs.real() * rhs.real());
    return *this;
}

template<typename T>
constexpr Dual<T>&
Dual<T>::operator /= (T rhs)
{
    real_ = real_ / rhs;
    dual_ = dual_ / rhs;
    return *this;
}

/* Query functions */

template<typename T>
constexpr bool
Dual<T>::is_zero(T tolerance) const
{
    return (detail::abs(real_) <= tolerance) && (detail::abs(dual_) <= tolerance);
}

template<typename T>
constexpr bool
Dual<T>::is_real(T tolerance) const
{
    return detail::abs(dual_) <= tolerance;
}

template<typename T>
constexpr bool
Dual<T>::is_pure(T tolerance) const
{
    return detail::abs(real_) <= tolerance;
}

template<typename T>
bool
Dual<T>::is_inf() const
{
    return std::isinf(real_) || std::isinf(dual_);
}

template<typename T>
bool
Dual<T>::is_nan() const
{
    return std::isnan(real_) || std::isnan(dual_);
}

template<typename T>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
constexpr
#endif
bool
Dual<T>::is_unit(T tolerance) const
{
    return detail::are_close(real_ * real_, T(1.0), tolerance);
}

/* Comparison functions */

template<typename T>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
constexpr
#endif
bool Dual<T>::is_close_to(const Dual<T>& other, T rel_tolerance, T abs_tolerance) const
{
    return detail::are_close(real_, other.real(), rel_tolerance, abs_tolerance)
        && detail::are_close(dual_, other.dual(), rel_tolerance, abs_tolerance);
}

template<typename T>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
constexpr
#endif
bool Dual<T>::is_close_to(const Dual<T>& other, T tolerance) const
{
    return detail::are_close(real_, other.real(), tolerance)
        && detail::are_close(dual_, other.dual(), tolerance);
}

/* Unary operators */

template<typename T>
constexpr Dual<T>
operator + (const Dual<T>& rhs)
{
    return rhs;
}

template<typename T>
constexpr Dual<T>
operator - (const Dual<T>& rhs)
{
    return Dual<T>(
        -rhs.real(),
        -rhs.dual());
}

/* Binary operators */

template<typename T>
constexpr Dual<T>
operator + (const Dual<T>& lhs, const Dual<T>& rhs)
{
    Dual<T> temp(lhs);
    return temp += rhs;
}

template<typename T>
constexpr Dual<T>
operator - (const Dual<T>& lhs, const Dual<T>& rhs)
{
    Dual<T> temp(lhs);
    return temp -= rhs;
}

template<typename T>
constexpr Dual<T>
operator * (const Dual<T>& lhs, const Dual<T>& rhs)
{
    Dual<T> temp(lhs);
    return temp *= rhs;
}

template<typename T>
constexpr Dual<T>
operator * (T lhs, const Dual<T>& rhs)
{
    Dual<T> temp(rhs);
    return temp *= lhs;
}

template<typename T>
constexpr Dual<T>
operator * (const Dual<T>& lhs, T rhs)
{
    Dual<T> temp(lhs);
    return temp *= rhs;
}

template<typename T>
constexpr Dual<T>
operator / (const Dual<T>& lhs, const Dual<T>& rhs)
{
    Dual<T> temp(lhs);
    return temp /= rhs;
}

template<typename T>
constexpr Dual<T>
operator / (const Dual<T>& lhs, T rhs)
{
    Dual<T> temp(lhs);
    return temp /= rhs;
}

/* Real-valued functions */

/*!
 * This return the magnitude (Euclidian norm) of the dual number.
 */
template<typename T>
constexpr T
abs(const Dual<T>& z)
{
    return detail::abs(z.real());
}

/*!
 * This return the Cayley norm(square of the absolute value) of the dual number.
 */
template<typename T>
constexpr T
norm(const Dual<T>& z)
{
    return z.real() * z.real();
}

/* Dual-valued functions */

template<typename T>
constexpr Dual<T>
conjugate(const Dual<T>& z)
{
    return Dual<T>(
        z.real(),
        -z.dual());
}

template<typename T>
constexpr Dual<T>
inverse(const Dual<T>& z)
{
    return Dual<T>(
        T(1.0) / z.real(),
        - z.dual() / (z.real() * z.real()));
}

template<typename T>
constexpr Dual<T>
normalize(const Dual<T>& z)
{
    return z / abs(z);
}

/* Trigonometric functions */

template<typename T>
Dual<T>
sin(const Dual<T>& z)
{
    return Dual<T>(
        std::sin(z.real()),
        z.dual() * std::cos(z.real()));
}

template<typename T>
Dual<T>
cos(const Dual<T>& z)
{
    return Dual<T>(
        std::cos(z.real()),
        - z.dual() * std::sin(z.real()));
}

template<typename T>
Dual<T>
tan(const Dual<T>& z)
{
    auto temp = std::cos(z.real());
    return Dual<T>(
        std::tan(z.real()),
        z.dual() / (temp * temp));
}

template<typename T>
Dual<T>
asin(const Dual<T>& z)
{
    return Dual<T>(
        std::asin(z.real()),
        z.dual() / std::sqrt(T(1.0) - z.real() * z.real()));
}

template<typename T>
Dual<T>
acos(const Dual<T>& z)
{
    return Dual<T>(
        std::acos(z.real()),
        - z.dual() / std::sqrt(T(1.0) - z.real() * z.real()));
}

template<typename T>
Dual<T>
atan(const Dual<T>& z)
{
    return Dual<T>(
        std::atan(z.real()),
        z.dual() / (T(1.0) + z.real() * z.real()));
}

/* Hyperbolic functions */

template<typename T>
Dual<T>
sinh(const Dual<T>& z)
{
    return Dual<T>(
        std::sinh(z.real()),
        z.dual() * std::cosh(z.real()));
}

template<typename T>
Dual<T>
cosh(const Dual<T>& z)
{
    return Dual<T>(
        std::cosh(z.real()),
        z.dual() * std::sinh(z.real()));
}

template<typename T>
Dual<T>
tanh(const Dual<T>& z)
{
    auto temp = std::cosh(z.real());
    return Dual<T>(
        std::tanh(z.real()),
        z.dual() / (temp * temp));
}

template<typename T>
Dual<T>
asinh(const Dual<T>& z)
{
    return Dual<T>(
        std::asinh(z.real()),
        z.dual() / std::sqrt(z.real() * z.real() + T(1.0)));
}

template<typename T>
Dual<T>
acosh(const Dual<T>& z)
{
    return Dual<T>(
        std::acosh(z.real()),
        z.dual() / std::sqrt(z.real() * z.real() - T(1.0)));
}

template<typename T>
Dual<T>
atanh(const Dual<T>& z)
{
    return Dual<T>(
        std::atanh(z.real()),
        z.dual() / (T(1.0) - z.real() * z.real()));
}

/* Exponential and Logarithmic functions */

template<typename T>
Dual<T>
exp(const Dual<T>& z)
{
    auto temp = std::exp(z.real());
    return Dual<T>(
        temp,
        z.dual() * temp);
}

template<typename T>
Dual<T>
log(const Dual<T>& z)
{
    return Dual<T>(
        std::log(z.real()),
        z.dual() / z.real());
}

template<typename T>
Dual<T>
log2(const Dual<T>& z)
{
    return Dual<T>(
        std::log2(z.real()),
        z.dual() / (z.real() * std::log(T(2.0))));
}

template<typename T>
Dual<T>
log10(const Dual<T>& z)
{
    return Dual<T>(
        std::log10(z.real()),
        z.dual() / (z.real() * std::log(T(10.0))));
}

/* Powers */

template<typename T>
Dual<T>
pow(const Dual<T>& z, const Dual<T>& w)
{
    auto temp = std::pow(z.real(), w.real());
    return Dual<T>(
        temp,
        temp * (z.dual() * w.real() / z.real() + w.dual() * std::log(z.real())));
}

template<typename T>
Dual<T>
pow(const Dual<T>& z, T x)
{
    auto temp = std::pow(z.real(), x);
    return Dual<T>(
        temp,
        temp * (z.dual() * x / z.real()));
}

/* Nth roots */

template<typename T>
constexpr Dual<T>
square(const Dual<T>& z)
{
    return Dual<T>(
        z.real() * z.real(),
        T(2.0) * z.real() * z.dual());
}

template<typename T>
Dual<T>
sqrt(const Dual<T>& z)
{
    auto temp = std::sqrt(z.real());
    return Dual<T>(
        temp,
        z.dual() * T(0.5) / temp);
}

template<typename T>
constexpr Dual<T>
cube(const Dual<T>& z)
{
    auto r_squared = z.real() * z.real();
    return Dual<T>(
        z.real() * r_squared,
        T(3.0) * r_squared * z.dual());
}

template<typename T>
Dual<T>
cbrt(const Dual<T>& z)
{
    return Dual<T>(
        std::cbrt(z.real()),
        z.dual() / (T(3.0) * std::cbrt(z.real() * z.real())));
}

/* Stream Operators */

template<typename T>
std::ostream& operator << (std::ostream& os, const Dual<T>& z)
{
    const auto flags = os.flags();
    os << "[" << z.real() << ", "
              << z.dual() << "]";
    os.flags(flags);
    return os;
}

}   // namespace dual
