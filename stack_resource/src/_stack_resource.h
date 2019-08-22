#pragma once
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <new>
#include <cassert>
#if defined(__clang__)
#include <experimental/memory_resource>
#else
#include <memory_resource>
#endif

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
}   // namespace stack_resource_detail

template<std::size_t N>
class stack_resource final
    : public stack_resource_detail::memory_resource
{
    static constexpr std::size_t offset_bytes = sizeof(void*);

public:
    stack_resource() = default;
    ~stack_resource() = default;
    stack_resource(const stack_resource&) = delete;
    stack_resource& operator = (const stack_resource&) = delete;
    stack_resource(stack_resource&&) = delete;
    stack_resource& operator = (stack_resource&&) = delete;

public:
    static constexpr std::size_t size() noexcept
    {
        return N;
    }

    std::size_t usage() const noexcept
    {
        return static_cast<std::size_t>(ptr_ - buffer_);
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        assert(is_power_of_two(alignment));
        /*
            addr: raw                  aligend         next
                   |---------|------------|------~------|
            size:   <- gap -> <- offset -> <- request ->
        */
        auto raw_ptr = ptr_;

        auto raw_addr = reinterpret_cast<std::uintptr_t>(raw_ptr);

        auto aligned_addr = align_up(raw_addr + offset_bytes, alignment);

        auto actual_bytes = bytes + (aligned_addr - raw_addr);

        if((ptr_ + actual_bytes) > std::end(buffer_))
            throw std::bad_alloc();
        ptr_ += actual_bytes;

        std::ptrdiff_t offset_addr = aligned_addr - offset_bytes;

        auto pp_offset_addr = reinterpret_cast<void**>(offset_addr);
        *pp_offset_addr = raw_ptr;

        return reinterpret_cast<void*>(aligned_addr);
    }

    void do_deallocate(void* p, std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
    {
        assert(p != nullptr);

        auto aligned_ptr = reinterpret_cast<std::byte*>(p);
        assert(pointer_in_buffer(aligned_ptr));

        auto aligned_addr = reinterpret_cast<std::uintptr_t>(p);

        std::ptrdiff_t offset_addr = aligned_addr - offset_bytes;

        auto pp_offset_addr = reinterpret_cast<void**>(offset_addr);
        auto raw_ptr = reinterpret_cast<std::byte*>(*pp_offset_addr);

        if((aligned_ptr + bytes) == ptr_)
            ptr_ = raw_ptr;
    }

    bool do_is_equal(const stack_resource_detail::memory_resource& other) const noexcept override
    {
        return this == &other;
    }

private:
    static constexpr bool is_power_of_two(std::size_t x)
    {
        return x && !(x & (x - 1));
    }

    static std::uintptr_t align_up(std::uintptr_t raw_address, std::size_t alignment)
    {
        assert(alignment > 0);
        --alignment;
        return (raw_address + alignment) & ~alignment;
    }

    bool pointer_in_buffer(std::byte* p) noexcept
    {
        return (p >= buffer_) && (p < (buffer_ + N));
    }

private:
    std::byte buffer_[N]{};
    std::byte* ptr_ = buffer_;
};

}   // namespace pmr

}   // namespace container