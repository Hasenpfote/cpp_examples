#pragma once
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <new>
#include <cassert>
#include <iterator>
#if defined(__clang__)
#include <experimental/memory_resource>
#else
#include <memory_resource>
#endif
#include "alignment.h"

namespace container
{

namespace pmr
{

namespace stack_resource_detail
{
#if defined(__clang__)
using memory_resource = std::experimental::pmr::memory_resource;
#else
using memory_resource = std::pmr::memory_resource;
#endif

namespace v1
{

template<std::size_t N>
class stack_resource final : public memory_resource
{
private:
    static constexpr std::size_t offset_bytes = sizeof(void*);

public:
    static constexpr std::size_t size() noexcept { return N; }

public:
    stack_resource() = default;
    ~stack_resource() = default;

    stack_resource(const stack_resource&) = delete;
    stack_resource& operator = (const stack_resource&) = delete;

    stack_resource(stack_resource&&) = delete;
    stack_resource& operator = (stack_resource&&) = delete;

    std::size_t usage() const noexcept
    {
        return static_cast<std::size_t>(ptr_ - buffer_);
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        assert(alignment::is_power_of_2(alignment));
        /*
            addr: raw                  aligend         next
                   |---------|------------|------~------|
            size:   <- gap -> <- offset -> <- request ->
        */
        auto raw_ptr = ptr_;

        auto raw_addr = reinterpret_cast<std::uintptr_t>(raw_ptr);

        auto aligned_addr = alignment::align_up(raw_addr + offset_bytes, alignment);

        auto actual_bytes = bytes + (aligned_addr - raw_addr);

        if((ptr_ + actual_bytes) > std::end(buffer_))
            throw std::bad_alloc();
        ptr_ += actual_bytes;
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
        std::ptrdiff_t offset_addr = aligned_addr - offset_bytes;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
        auto pp_offset_addr = reinterpret_cast<void**>(offset_addr);
        *pp_offset_addr = raw_ptr;

        return reinterpret_cast<void*>(aligned_addr);
    }

    void do_deallocate(void* p, std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
    {
        assert(p != nullptr);

        auto aligned_ptr = static_cast<std::byte*>(p);
        assert(pointer_in_buffer(aligned_ptr));

        auto aligned_addr = reinterpret_cast<std::uintptr_t>(p);
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
        std::ptrdiff_t offset_addr = aligned_addr - offset_bytes;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
        auto pp_offset_addr = reinterpret_cast<void**>(offset_addr);
        auto raw_ptr = static_cast<std::byte*>(*pp_offset_addr);

        if((aligned_ptr + bytes) == ptr_)
            ptr_ = raw_ptr;
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

private:
    bool pointer_in_buffer(std::byte* p) noexcept
    {
        return (p >= std::begin(buffer_)) && (p < std::end(buffer_));
    }

private:
    std::byte buffer_[N]{};
    std::byte* ptr_ = buffer_;
};

}   // v1

namespace v2
{

template<std::size_t N, std::size_t Alignment = alignof(std::max_align_t)>
class arena final
{
    static_assert(alignment::is_power_of_2(Alignment), "Alignment must be a power of 2.");
private:
    static constexpr std::size_t align_up(std::size_t x) noexcept
    {
        return alignment::align_up(x, Alignment);
    }

public:
    arena() = default;
    ~arena() = default;

    arena(const arena&) = delete;
    arena& operator = (const arena&) = delete;

    arena(arena&&) = delete;
    arena& operator = (arena&&) = delete;

    std::size_t usage() const noexcept
    {
        return static_cast<std::size_t>(ptr_ - buffer_);
    }

    std::byte* allocate(std::size_t bytes, [[maybe_unused]] std::size_t alignment)
    {
        assert(alignment::is_power_of_2(alignment));
        assert(alignment <= Alignment);

        auto actual_bytes = align_up(bytes);
        if(actual_bytes > static_cast<decltype(actual_bytes)>(std::end(buffer_) - ptr_))
            throw std::bad_alloc();

        auto ret = ptr_;
        ptr_ += actual_bytes;   // Always aligned on an `Alignment`-byte boundary.

        return ret;
    }

    void deallocate(std::byte* p, std::size_t bytes)
    {
        assert(pointer_in_buffer(p));

        auto actual_bytes = align_up(bytes);
        if((p + actual_bytes) == ptr_)
            ptr_ = p;
    }

private:
    bool pointer_in_buffer(std::byte* p) noexcept
    {
        return (p >= std::begin(buffer_)) && (p < std::end(buffer_));
    }

private:
    alignas(Alignment) std::byte buffer_[N]{};
    std::byte* ptr_ = buffer_;
};

template<std::size_t N, std::size_t Alignment = alignof(std::max_align_t)>
class stack_resource final : public memory_resource
{
    static_assert(alignment::is_power_of_2(Alignment), "Alignment must be a power of 2.");
public:
    static constexpr std::size_t size() noexcept { return N; }

public:
    stack_resource() = default;
    ~stack_resource() = default;

    stack_resource(const stack_resource&) = delete;
    stack_resource& operator = (const stack_resource&) = delete;

    stack_resource(stack_resource&&) = delete;
    stack_resource& operator = (stack_resource&&) = delete;

    std::size_t usage() const noexcept
    {
        return arena_.usage();
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        return static_cast<void*>(arena_.allocate(bytes, alignment));
    }

    void do_deallocate(void* p, std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
    {
        arena_.deallocate(static_cast<std::byte*>(p), bytes);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

private:
    arena<N, Alignment> arena_;
};

}   // v2

}   // namespace stack_resource_detail

template<std::size_t N>
using stack_resource = stack_resource_detail::v2::stack_resource<N>;

}   // namespace pmr

}   // namespace container
