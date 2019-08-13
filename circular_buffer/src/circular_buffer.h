#include <cassert>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm>

template<typename T>
class simple_circular_buffer;

template<typename T>
class _scb_const_iterator
{
public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = std::forward_iterator_tag;
    using _Tptr = T*;

    _scb_const_iterator() = delete;
    _scb_const_iterator(const _scb_const_iterator&) = default;
    _scb_const_iterator(_scb_const_iterator&&) = default;
    _scb_const_iterator& operator=(const _scb_const_iterator&) = default;
    _scb_const_iterator& operator=(_scb_const_iterator&&) = default;

    _scb_const_iterator(const simple_circular_buffer<T>* cb, _Tptr ptr)
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
        ptr_ = const_cast<_Tptr>(cb_->increment(ptr_));
        return *this;
    }

    _scb_const_iterator operator++(int)
    {
        _scb_const_iterator temp = *this;
        ptr_ = const_cast<_Tptr>(cb_->increment(ptr_));
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
    const simple_circular_buffer<T>* cb_;
    _Tptr ptr_;
};

template<typename T>
class _scb_iterator
{
public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = std::forward_iterator_tag;

    _scb_iterator() = delete;
    _scb_iterator(const _scb_iterator&) = default;
    _scb_iterator(_scb_iterator&&) = default;
    _scb_iterator& operator=(const _scb_iterator&) = default;
    _scb_iterator& operator=(_scb_iterator&&) = default;

    _scb_iterator(const simple_circular_buffer<T>* cb, pointer ptr)
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
        ptr_ = const_cast<pointer>(cb_->increment(ptr_));
        return *this;
    }

    _scb_iterator operator++(int)
    {
        _scb_iterator temp = *this;
        ptr_ = const_cast<pointer>(cb_->increment(ptr_));
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
    const simple_circular_buffer<T>* cb_;
    pointer ptr_;
};

template<typename T>
class simple_circular_buffer final
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = _scb_iterator<T>;
    using const_iterator = _scb_const_iterator<T>;

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

    void clear() noexcept
    {
        head_ = 0;
        tail_ = 0;
        contents_size_ = 0;
    }

    void push_front(const_reference item);
    void push_back(const_reference item);

    void pop_front();
    void pop_back();

    size_type head() const noexcept { return head_; }
    size_type tail() const noexcept { return tail_; }

    size_type size() const noexcept { return contents_size_; }

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

    iterator begin(){ return iterator(this, _begin()); }
    const_iterator begin() const { return const_iterator(this, const_cast<pointer>(_begin())); }

    iterator end(){ return iterator(this, _end()); }
    const_iterator end() const { return const_iterator(this, const_cast<pointer>(_end())); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

private:
    pointer _begin();
    const_pointer _begin() const;

    pointer _end();
    const_pointer _end() const;

    pointer increment(const_pointer ptr);
    const_pointer increment(const_pointer ptr) const;

private:
    std::vector<value_type> array_;
    size_type head_;
    size_type tail_;
    size_type contents_size_;
};

template<typename T>
typename simple_circular_buffer<T>::reference
simple_circular_buffer<T>::operator[](size_type index)
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.operator[](index));
#else
    return const_cast<reference>(std::as_const(*this).operator[](index));
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_reference
simple_circular_buffer<T>::operator[](size_type index) const
{
    assert(!is_empty());
    assert(index < capacity());
    constexpr auto max_limit = std::numeric_limits<std::size_t>::max();
    auto diff = max_limit - capacity();
    auto padding = (index > diff)? diff : static_cast<decltype(diff)>(0);
    auto actual_index = (head_ + index + padding) % capacity();
    return array_[actual_index];
}

template<typename T>
typename simple_circular_buffer<T>::reference
simple_circular_buffer<T>::front()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.front());
#else
    return const_cast<reference>(std::as_const(*this).front());
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_reference
simple_circular_buffer<T>::front() const
{
    assert(!is_empty());
    return array_[head_];
}

template<typename T>
typename simple_circular_buffer<T>::reference
simple_circular_buffer<T>::back()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<reference>(temp.back());
#else
    return const_cast<reference>(std::as_const(*this).back());
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_reference
simple_circular_buffer<T>::back() const
{
    assert(!is_empty());
    auto index = (tail_ > 0)? tail_ - 1 : capacity() - 1;
    return array_[index];
}

template<typename T>
void simple_circular_buffer<T>::push_front(const_reference item)
{
    const auto cap = capacity();

    head_ = (head_ > 0)? head_ - 1 : cap - 1;
    array_[head_] = item;

    if(contents_size_ < cap)
    {
        contents_size_++;
    }
    else
    {
        tail_ = (tail_ > 0)? tail_ - 1 : cap - 1;
    }
}

template<typename T>
void simple_circular_buffer<T>::push_back(const_reference item)
{
    const auto cap = capacity();

    array_[tail_] = item;
    tail_ = (tail_ < (cap - 1))? tail_ + 1 : 0;

    if(contents_size_ < cap)
    {
        contents_size_++;
    }
    else
    {
        head_ = (head_ < (cap - 1))? head_ + 1 : 0;
    }
}

template<typename T>
void simple_circular_buffer<T>::pop_front()
{
    assert(!is_empty());
    head_ = (head_ < (capacity() - 1))? head_ + 1 : 0;
    contents_size_--;
}

template<typename T>
void simple_circular_buffer<T>::pop_back()
{
    assert(!is_empty());
    tail_ = (tail_ > 0)? tail_ - 1 : capacity() - 1;
    contents_size_--;
}

template<typename T>
typename simple_circular_buffer<T>::array_range_t
simple_circular_buffer<T>::array_one()
{
    assert(!is_empty());
    auto size = (head_ < tail_)? tail_ - head_ : capacity() - head_;
    return std::make_pair(&array_[head_], size);
}

template<typename T>
typename simple_circular_buffer<T>::const_array_range_t
simple_circular_buffer<T>::array_one() const
{
    assert(!is_empty());
    auto size = (head_ < tail_)? tail_ - head_ : capacity() - head_;
    return std::make_pair(&array_[head_], size);
}

template<typename T>
typename simple_circular_buffer<T>::array_range_t
simple_circular_buffer<T>::array_two()
{
    assert(!is_empty());
    auto size = (tail_ > head_)? 0 : tail_;
    return std::make_pair(&array_[0], size);
}

template<typename T>
typename simple_circular_buffer<T>::const_array_range_t
simple_circular_buffer<T>::array_two() const
{
    assert(!is_empty());
    auto size = (tail_ > head_)? 0 : tail_;
    return std::make_pair(&array_[0], size);
}

template<typename T>
bool simple_circular_buffer<T>::is_linearized() const
{
    return head_ == 0;
}

template<typename T>
void simple_circular_buffer<T>::linearize()
{
    if(is_linearized())
        return;

    std::rotate(array_.begin(), array_.begin() + head_, array_.end());

    head_ = 0;
    tail_ = (is_full())? 0 : contents_size_;
}

template<typename T>
typename simple_circular_buffer<T>::pointer
simple_circular_buffer<T>::_begin()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<pointer>(temp._begin());
#else
    return const_cast<pointer>(std::as_const(*this)._begin());
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_pointer
simple_circular_buffer<T>::_begin() const
{
    return &array_[head_];
}

template<typename T>
typename simple_circular_buffer<T>::pointer
simple_circular_buffer<T>::_end()
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<pointer>(temp._end());
#else
    return const_cast<pointer>(std::as_const(*this)._end());
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_pointer
simple_circular_buffer<T>::_end() const
{
    return (is_full())? array_.data() + array_.size() : &array_[tail_];
}

template<typename T>
typename simple_circular_buffer<T>::pointer
simple_circular_buffer<T>::increment(const_pointer ptr)
{
#if (defined(_MSC_VER) && (_MSVC_LANG < 201703L)) || (__cplusplus < 201703L)
    const auto& temp = *this;
    return const_cast<pointer>(temp.increment());
#else
    return const_cast<pointer>(std::as_const(*this).increment());
#endif
}

template<typename T>
typename simple_circular_buffer<T>::const_pointer
simple_circular_buffer<T>::increment(const_pointer ptr) const
{
    assert(!is_empty());

    auto ar_interval = std::make_pair(array_.data(), array_.data() + array_.size());
    auto it_interval = std::make_pair(_begin(), _end());

    assert(ptr != it_interval.second);
    if(ptr == (ar_interval.second - 1))
        ptr = ar_interval.first;
    else
        ptr++;

    if((head_ == tail_) && (ptr == it_interval.first))
        ptr = it_interval.second;

    return ptr;
}