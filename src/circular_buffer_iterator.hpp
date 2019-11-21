//
// Created by emdash00 on 11/6/19.
//

#ifndef CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP
#define CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP

#include <cstddef>
#include <iterator>

template <typename T, typename T_nonconst=T, typename elem_type = typename T::value_type>
class circular_buffer_iterator
{
public:
    typedef std::random_access_iterator_tag iterator_category;

    typedef T cbuf_type;
    typedef size_t size_type;
    typedef elem_type value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const T* const_pointer;
    typedef T* Pointer;
    typedef T& Reference;
    typedef circular_buffer_iterator<T, T_nonconst, elem_type> self_type;

    typedef circular_buffer_iterator<self_type, T_nonconst> iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    circular_buffer_iterator() = default;
    circular_buffer_iterator(cbuf_type *b, size_t start_pos) : m_buf(b), m_pos(start_pos) { }

    circular_buffer_iterator (const circular_buffer_iterator<
            T_nonconst, T_nonconst, typename T_nonconst::value_type> &other)
            : m_buf(other.m_buf), m_pos(other.m_pos) { }

    friend class circular_buffer_iterator<const T, T, const elem_type>;

    elem_type &operator*() { return (*this->m_buf)[this->m_pos]; }
    elem_type *operator->() { return &(operator*()); }

    self_type &operator++()
    {
        ++this->m_pos;
        return *this;
    }
    self_type operator++(int)
    {
        self_type tmp(*this);
        ++(*this);
        return tmp;
    }
    self_type &operator--()
    {
        --this->m_pos;
        return *this;
    }
    self_type operator--(int)
    {
        self_type tmp(*this);
        --(*this);
        return tmp;
    }

    self_type &operator+=(difference_type n)
    {
        this->m_pos += n;
        return *this;
    }
    self_type &operator-=(difference_type n)
    {
        this->m_pos -= n;
        return *this;
    }

    self_type operator+(difference_type n)
    {
        self_type tmp(*this);
        tmp += n;
        return tmp;
    }
    self_type operator-(difference_type n)
    {
        self_type tmp(*this);
        tmp -= n;
        return tmp;
    }
    self_type operator%=(difference_type n)
    {
        this->m_pos %= n;
        return *this;
    }
    self_type operator%(difference_type n)
    {
        self_type tmp(*this);
        tmp %= n;
        return tmp;
    }

    bool operator==(const self_type &other) const
    {
        return(this->m_buf == other.m_buf && this->m_pos == other.m_pos);
    }
    bool operator!=(const self_type &other) const
    {
        return(this->m_buf != other.m_buf || this->m_pos != other.m_pos);
    }

    bool operator<(const self_type &other) const
    {
        return(this->m_pos < other.m_pos);
    }
    bool operator<=(const self_type &other) const
    {
        return(this->m_pos <= other.m_pos);
    }

    bool operator>(const self_type &other) const
    {
        return(this->m_pos > other.m_pos);
    }
    bool operator>=(const self_type &other) const
    {
        return(this->m_pos >= other.m_pos);
    }

private:
    cbuf_type *m_buf;
    size_type m_pos;
};

#endif //CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP
