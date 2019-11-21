//
// Created by emdash00 on 11/6/19.
//

#ifndef CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP
#define CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <hwloc/inlines.h>

template <typename T, typename elem_type = typename T::value_type>
class circular_buffer_iterator
{
public:
    typedef std::bidirectional_iterator_tag  iterator_category;

    typedef T cbuf_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const T* const_pointer;
    typedef T* Pointer;
    typedef T& Reference;
    typedef circular_buffer_iterator<T> self_type;

    typedef circular_buffer_iterator<T> iterator;
    typedef const circular_buffer_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    circular_buffer_iterator(cbuf_type *b, size_t start_pos) : m_buf(b), m_pos(start_pos) { }

    elem_type &operator*() { return (*this->m_buf)[this->m_pos]; }
    elem_type *operator->() { return &(operator*()); }

    self_type &operator++()
    {
        ++this->m_pos;
        return *this;
    }
    self_type &operator++(int)
    {
        self_type tmp(*this);
        ++(*this);
        return tmp;
    }

    self_type operator+(difference_type n)
    {
        self_type tmp(*this);
        tmp.pos_ += n;
        return tmp;
    }
    self_type &operator+=(difference_type n)
    {
        this->m_pos += n;
        return *this;
    }



private:
    cbuf_type *m_buf;
    size_t m_pos;
};

#endif //CIRCULARBUFFER_CIRCULAR_BUFFER_ITERATOR_HPP
