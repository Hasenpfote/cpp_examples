#include <cassert>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm>
#include <memory>
#include <iterator>

template<typename SCB, typename Traits>
class _scb_const_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename Traits::value_type;
    using pointer           = typename Traits::pointer;
    using reference         = typename Traits::reference;
    using difference_type   = typename Traits::difference_type;
    using _Tptr             = typename Traits::value_type*;

    _scb_const_iterator() = delete;
    _scb_const_iterator(const _scb_const_iterator&) = default;
    _scb_const_iterator(_scb_const_iterator&&) = default;
    _scb_const_iterator& operator=(const _scb_const_iterator&) = default;
    _scb_const_iterator& operator=(_scb_const_iterator&&) = default;

    _scb_const_iterator(const SCB* cb, _Tptr ptr)
        : cb_(cb), ptr_(ptr)
    {}

    reference operator*()
    {
        return *ptr_;
    }

    pointer operator->()
    {
        return ptr_;
    }

    _scb_const_iterator& operator++()
    {
        assert(ptr_ != cb_->buffer_end());

        ptr_ = const_cast<_Tptr>(cb_->increment(ptr_));

        if(ptr_ == cb_->buffer_begin())
            ptr_ = const_cast<_Tptr>(cb_->buffer_end());

        return *this;
    }

    _scb_const_iterator operator++(int)
    {
        _scb_const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    _scb_const_iterator& operator--()
    {
        assert(ptr_ != cb_->buffer_begin());

        if(ptr_ == cb_->array_end())
            ptr_ = const_cast<_Tptr>(cb_->buffer_begin());

        ptr_ = const_cast<_Tptr>(cb_->decrement(ptr_));

        return *this;
    }

    _scb_const_iterator operator--(int)
    {
        _scb_const_iterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const _scb_const_iterator& rhs)
    {
        return ptr_ == rhs.ptr_;
    }

    bool operator!=(const _scb_const_iterator& rhs)
    {
        return ptr_ != rhs.ptr_;
    }

private:
    const SCB* cb_;
    _Tptr ptr_;
};

template<typename SCB, typename Traits>
class _scb_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename Traits::value_type;
    using pointer           = typename Traits::pointer;
    using reference         = typename Traits::reference;
    using difference_type   = typename Traits::difference_type;

    _scb_iterator() = delete;
    _scb_iterator(const _scb_iterator&) = default;
    _scb_iterator(_scb_iterator&&) = default;
    _scb_iterator& operator=(const _scb_iterator&) = default;
    _scb_iterator& operator=(_scb_iterator&&) = default;

    _scb_iterator(const SCB* cb, pointer ptr)
        : cb_(cb), ptr_(ptr)
    {}

    reference operator*()
    {
        return *ptr_;
    }

    pointer operator->()
    {
        return ptr_;
    }

    _scb_iterator& operator++()
    {
        assert(ptr_ != cb_->buffer_end());

        ptr_ = const_cast<pointer>(cb_->increment(ptr_));

        if(ptr_ == cb_->buffer_begin())
            ptr_ = const_cast<pointer>(cb_->buffer_end());

        return *this;
    }

    _scb_iterator operator++(int)
    {
        _scb_iterator temp = *this;
        ++(*this);
        return temp;
    }

    _scb_iterator& operator--()
    {
        assert(ptr_ != cb_->buffer_begin());

        if(ptr_ == cb_->array_end())
            ptr_ = const_cast<pointer>(cb_->buffer_begin());

        ptr_ = const_cast<pointer>(cb_->decrement(ptr_));

        return *this;
    }

    _scb_iterator operator--(int)
    {
        _scb_iterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const _scb_iterator& rhs)
    {
        return ptr_ == rhs.ptr_;
    }

    bool operator!=(const _scb_iterator& rhs)
    {
        return ptr_ != rhs.ptr_;
    }

private:
    const SCB* cb_;
    pointer ptr_;
};

template<typename T, typename Allocator = std::allocator<T>>
class simple_circular_buffer final
{
public:
    using self_type         = simple_circular_buffer<T, Allocator>;
    using value_type        = typename std::allocator_traits<Allocator>::value_type;
    using pointer           = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer     = typename std::allocator_traits<Allocator>::const_pointer;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using difference_type   = typename std::allocator_traits<Allocator>::difference_type;
    using size_type         = typename std::allocator_traits<Allocator>::size_type;
    using allocator_type    = Allocator;

    using iterator = _scb_iterator<self_type, std::iterator_traits<pointer>>;
    using const_iterator = _scb_const_iterator<self_type, std::iterator_traits<const_pointer>>;

    using array_range_t = std::pair<pointer, size_type>;
    using const_array_range_t = std::pair<const_pointer, size_type>;

    friend iterator;
    friend const_iterator;

public:
    simple_circular_buffer() = delete;
    ~simple_circular_buffer() = default;
    simple_circular_buffer(const simple_circular_buffer&) = default;
    simple_circular_buffer(simple_circular_buffer&&) = default;
    simple_circular_buffer& operator=(const simple_circular_buffer&) = default;
    simple_circular_buffer& operator=(simple_circular_buffer&&) = default;

    explicit simple_circular_buffer(size_type capacity = 100)
        : array_(capacity)
    {
        assert(capacity > 1);
        clear();
    }

    reference operator[](size_type index);
    const_reference operator[](size_type index) const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    void clear() noexcept;

    template<typename U = T>
    std::enable_if_t<std::is_copy_constructible<U>::value, void>
    push_front(const_reference item);

    template<typename U = T>
    std::enable_if_t<std::is_move_constructible<U>::value, void>
    push_front(value_type&& item);

    template<typename U = T>
    std::enable_if_t<std::is_copy_constructible<U>::value, void>
    push_back(const_reference item);

    template<typename U = T>
    std::enable_if_t<std::is_move_constructible<U>::value, void>
    push_back(value_type&& item);

    void pop_front();
    void pop_back();

    size_type head() const noexcept { return head_; }
    size_type tail() const noexcept { return tail_; }

    size_type size() const noexcept { return contents_size_; }
    size_type max_size() const noexcept { return array_.max_size; }

    bool is_empty() const noexcept { return contents_size_ == 0; }
    bool is_full() const noexcept { return contents_size_ == capacity(); }

    size_type capacity() const noexcept { return array_.size(); }

    pointer data() noexcept { return array_.data(); }
    const_pointer data() const noexcept { return array_.data(); }

    array_range_t array_one();
    const_array_range_t array_one() const;

    array_range_t array_two();
    const_array_range_t array_two() const;

    bool is_linearized() const;
    void linearize();

    allocator_type get_allocator() const noexcept { return array_.get_allocator(); }

    iterator begin(){ return iterator(this, buffer_begin()); }
    const_iterator begin() const { return const_iterator(this, const_cast<pointer>(buffer_begin())); }

    iterator end(){ return iterator(this, buffer_end()); }
    const_iterator end() const { return const_iterator(this, const_cast<pointer>(buffer_end())); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

private:
    template<typename U>
    void push_front_fwd(U&& item);

    template<typename U>
    void push_back_fwd(U&& item);

    pointer array_begin();
    const_pointer array_begin() const;

    pointer array_end();
    const_pointer array_end() const;

    pointer buffer_begin();
    const_pointer buffer_begin() const;

    pointer buffer_end();
    const_pointer buffer_end() const;

    pointer increment(const_pointer ptr);
    const_pointer increment(const_pointer ptr) const;

    pointer decrement(const_pointer ptr);
    const_pointer decrement(const_pointer ptr) const;

private:
    std::vector<value_type, allocator_type> array_;
    size_type head_;
    size_type tail_;
    size_type contents_size_;
};

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::reference
simple_circular_buffer<T, Allocator>::operator[](size_type index)
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.operator[](index));
#else
    return const_cast<reference>(std::as_const(*this).operator[](index));
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_reference
simple_circular_buffer<T, Allocator>::operator[](size_type index) const
{
    assert(!is_empty());
    assert(index < capacity());
    constexpr auto max_limit = std::numeric_limits<std::size_t>::max();
    auto diff = max_limit - capacity();
    auto padding = (index > diff)? diff : static_cast<decltype(diff)>(0);
    auto actual_index = (head_ + index + padding) % capacity();
    return array_[actual_index];
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::reference
simple_circular_buffer<T, Allocator>::front()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.front());
#else
    return const_cast<reference>(std::as_const(*this).front());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_reference
simple_circular_buffer<T, Allocator>::front() const
{
    assert(!is_empty());
    return array_[head_];
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::reference
simple_circular_buffer<T, Allocator>::back()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.back());
#else
    return const_cast<reference>(std::as_const(*this).back());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_reference
simple_circular_buffer<T, Allocator>::back() const
{
    assert(!is_empty());
    auto index = (tail_ > 0)? tail_ - 1 : capacity() - 1;
    return array_[index];
}

template<typename T, typename Allocator>
void simple_circular_buffer<T, Allocator>::clear() noexcept
{
    head_ = 0;
    tail_ = 0;
    contents_size_ = 0;
}

template<typename T, typename Allocator>
template<typename U>
std::enable_if_t<std::is_copy_constructible<U>::value, void>
simple_circular_buffer<T, Allocator>::push_front(const_reference item)
{
    push_front_fwd(item);
}

template<typename T, typename Allocator>
template<typename U>
std::enable_if_t<std::is_move_constructible<U>::value, void>
simple_circular_buffer<T, Allocator>::push_front(value_type&& item)
{
    push_front_fwd(std::move(item));
}

template<typename T, typename Allocator>
template<typename U>
void simple_circular_buffer<T, Allocator>::push_front_fwd(U&& item)
{
    const auto cap = capacity();

    head_ = (head_ > 0)? head_ - 1 : cap - 1;
    array_[head_] = std::forward<U>(item);

    if(contents_size_ < cap)
    {
        ++contents_size_;
    }
    else
    {
        tail_ = (tail_ > 0)? tail_ - 1 : cap - 1;
    }
}

template<typename T, typename Allocator>
template<typename U>
std::enable_if_t<std::is_copy_constructible<U>::value, void>
simple_circular_buffer<T, Allocator>::push_back(const_reference item)
{
    push_back_fwd(item);
}

template<typename T, typename Allocator>
template<typename U>
std::enable_if_t<std::is_move_constructible<U>::value, void>
simple_circular_buffer<T, Allocator>::push_back(value_type&& item)
{
    push_back_fwd(std::move(item));
}

template<typename T, typename Allocator>
template<typename U>
void simple_circular_buffer<T, Allocator>::push_back_fwd(U&& item)
{
    const auto cap = capacity();

    array_[tail_] = std::forward<U>(item);
    tail_ = (tail_ < (cap - 1))? tail_ + 1 : 0;

    if(contents_size_ < cap)
    {
        ++contents_size_;
    }
    else
    {
        head_ = (head_ < (cap - 1))? head_ + 1 : 0;
    }
}

template<typename T, typename Allocator>
void simple_circular_buffer<T, Allocator>::pop_front()
{
    assert(!is_empty());
    head_ = (head_ < (capacity() - 1))? head_ + 1 : 0;
    --contents_size_;
}

template<typename T, typename Allocator>
void simple_circular_buffer<T, Allocator>::pop_back()
{
    assert(!is_empty());
    tail_ = (tail_ > 0)? tail_ - 1 : capacity() - 1;
    --contents_size_;
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::array_range_t
simple_circular_buffer<T, Allocator>::array_one()
{
    assert(!is_empty());
    auto size = (head_ < tail_)? tail_ - head_ : capacity() - head_;
    return std::make_pair(&array_[head_], size);
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_array_range_t
simple_circular_buffer<T, Allocator>::array_one() const
{
    assert(!is_empty());
    auto size = (head_ < tail_)? tail_ - head_ : capacity() - head_;
    return std::make_pair(&array_[head_], size);
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::array_range_t
simple_circular_buffer<T, Allocator>::array_two()
{
    assert(!is_empty());
    auto size = (tail_ > head_)? 0 : tail_;
    return std::make_pair(&array_[0], size);
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_array_range_t
simple_circular_buffer<T, Allocator>::array_two() const
{
    assert(!is_empty());
    auto size = (tail_ > head_)? 0 : tail_;
    return std::make_pair(&array_[0], size);
}

template<typename T, typename Allocator>
bool simple_circular_buffer<T, Allocator>::is_linearized() const
{
    return head_ == 0;
}

template<typename T, typename Allocator>
void simple_circular_buffer<T, Allocator>::linearize()
{
    if(is_linearized())
        return;

    std::rotate(array_.begin(), array_.begin() + head_, array_.end());

    head_ = 0;
    tail_ = (is_full())? 0 : contents_size_;
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::array_begin()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto & temp = *this;
    return const_cast<pointer>(temp.array_begin());
#else
    return const_cast<pointer>(std::as_const(*this).array_begin());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::array_begin() const
{
    return array_.data();
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::array_end()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto & temp = *this;
    return const_cast<pointer>(temp.array_end());
#else
    return const_cast<pointer>(std::as_const(*this).array_end());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::array_end() const
{
    return array_.data() + array_.size();
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::buffer_begin()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto & temp = *this;
    return const_cast<pointer>(temp.buffer_begin());
#else
    return const_cast<pointer>(std::as_const(*this).buffer_begin());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::buffer_begin() const
{
    return &array_[head_];
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::buffer_end()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto & temp = *this;
    return const_cast<pointer>(temp.buffer_end());
#else
    return const_cast<pointer>(std::as_const(*this).buffer_end());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::buffer_end() const
{
    return (is_full())? array_end() : &array_[tail_];
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::increment(const_pointer ptr)
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<pointer>(temp.increment());
#else
    return const_cast<pointer>(std::as_const(*this).increment());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::increment(const_pointer ptr) const
{
    if(++ptr == array_end())
        ptr = array_begin();
    return ptr;
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::pointer
simple_circular_buffer<T, Allocator>::decrement(const_pointer ptr)
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto & temp = *this;
    return const_cast<pointer>(temp.decrement());
#else
    return const_cast<pointer>(std::as_const(*this).decrement());
#endif
}

template<typename T, typename Allocator>
typename simple_circular_buffer<T, Allocator>::const_pointer
simple_circular_buffer<T, Allocator>::decrement(const_pointer ptr) const
{
    if(ptr == array_begin())
        ptr = array_end();
    --ptr;
    return ptr;
}