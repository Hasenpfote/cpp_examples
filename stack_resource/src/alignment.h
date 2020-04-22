#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace alignment
{

template<typename T>
constexpr std::enable_if_t<!std::is_same_v<T, bool> && std::is_integral_v<T> && std::is_unsigned_v<T>, bool>
is_power_of_2(T x) noexcept
{
    return x && !(x & (x - 1));
}

template<typename T>
constexpr std::enable_if_t<!std::is_same_v<T, bool> && std::is_integral_v<T> && std::is_unsigned_v<T>, bool>
is_aligned(T x, std::size_t alignment) noexcept
{
    return (x & (alignment - 1)) == 0;
}

template<typename T>
std::enable_if_t<std::is_pointer_v<T>, bool>
is_aligned(T p, std::size_t alignment) noexcept
{
    return is_aligned(reinterpret_cast<std::uintptr_t>(p), alignment);
}

inline bool
is_aligned(std::nullptr_t, std::size_t) noexcept
{
    return false;
}

template<typename T>
constexpr std::enable_if_t<!std::is_same_v<T, bool> && std::is_integral_v<T> && std::is_unsigned_v<T>, T>
align_up(T x, std::size_t alignment) noexcept
{
    return (x + (alignment - 1)) & ~(alignment - 1);
}

template<typename T>
std::enable_if_t<std::is_pointer_v<T>, T>
align_up(T p, std::size_t alignment) noexcept
{
    return reinterpret_cast<T>(align_up(reinterpret_cast<std::uintptr_t>(p), alignment));
}

inline std::nullptr_t
align_up(std::nullptr_t, std::size_t) noexcept
{
    return nullptr;
}

template<typename T>
constexpr std::enable_if_t<!std::is_same_v<T, bool> && std::is_integral_v<T> && std::is_unsigned_v<T>, T>
align_down(T x, std::size_t alignment) noexcept
{
    return x & ~(alignment - 1);
}

template<typename T>
std::enable_if_t<std::is_pointer_v<T>, T>
align_down(T p, std::size_t alignment) noexcept
{
    return reinterpret_cast<T>(align_down(reinterpret_cast<std::uintptr_t>(p), alignment));
}

inline std::nullptr_t
align_down(std::nullptr_t, std::size_t) noexcept
{
    return nullptr;
}

}   // namespace alignment
