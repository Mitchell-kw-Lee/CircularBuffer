//
// Created by emdash00 on 11/20/19.
//

#ifndef CIRCULARBUFFER_STATIC_CIRCULAR_BUFFER_HPP
#define CIRCULARBUFFER_STATIC_CIRCULAR_BUFFER_HPP

#include <stdlib.h>
#include <stdexcept>

#ifndef SIZE_MAX
#define SIZE_MAX ((const size_t)(~0))
#endif


template <typename T, size_t _size=10>
class static_circular_buffer
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    //friend class circular_buffer_iterator;

    T &operator[](size_type index)
    {
        return this->m_buf[index];
    }

    const T &operator[](size_type index) const
    {
        return this->m_buf[index];
    }

    static_circular_buffer()  = default;
    ~static_circular_buffer() = default;

    void clear()
    {
        this->m_head = this->m_tail;
        this->m_full = false;
    }

    bool empty() const
    {
        return (!this->m_size);
    }

    bool full() const
    {
        return this->m_full;
    }

    reference front()
    {
        return this->m_buf[this->m_head];
    }
    reference back()
    {
        return this->m_buf[this->m_tail];
    }

    const_reference front() const
    {
        return this->m_buf[this->m_head];
    }
    const_reference back() const
    {
        return this->m_buf[this->m_tail];
    }

    reference from_front(size_type index)
    {
        if (index < this->m_capacity && index >= 0)
        {
            return(this->m_buf[index]);
        }

        throw std::out_of_range("'index' must be between a valid array index.");
    }

    const_reference from_front(size_type index) const
    {
        if (index < this->m_capacity && index >= 0)
        {
            return(this->m_buf[index]);
        }

        throw std::out_of_range("Index out of range");
    }

    reference from_back(size_type index)
    {
        if (index < this->m_capacity && index >= 0)
        {
            return(this->m_buf[(this->m_capacity - 1) - index]);
        }

        throw std::out_of_range("'index' must be between a valid array index.");
    }

    const_reference from_back(size_type index) const
    {
        if (index < this->m_capacity && index >= 0)
        {
            return(this->m_buf[(this->m_capacity - 1) - index]);
        }

        throw std::out_of_range("Index out of range");
    }

    size_type size() const
    {
        return this->m_size;
    }

    size_type capacity() const
    {
        return(this->m_capacity);
    }

    /* Largest size possible due to the upper limit of the size_t type */
    size_type max_size() const
    {
        return(SIZE_MAX);
    }

    void push_back(const value_type &item)
    {
        if (this->m_full)
            increment_head();

        increment_tail();

        std::cout << "tail: " << this->m_tail << std::endl;
        std::cout << "size: " << this->m_size << std::endl;

        this->m_buf[this->m_tail] = item;
    }

    void pop_front()
    {
        this->increment_head();
    }

private:
    value_type m_buf[_size];

    bool m_full = false;

    size_type m_head = 1, m_tail = 0;
    size_type m_size = 0;
    const size_type m_capacity = _size;

    inline void increment_tail()
    {
        this->m_tail = (this->m_tail + 1) % this->m_capacity;
        ++this->m_size;

        this->m_full = (this->m_size == this->m_capacity);
    }

    void increment_head()
    {
        this->m_head = (this->m_head + 1) % this->m_capacity;
        --this->m_size;

        this->m_full = false;
    }
};

#endif //CIRCULARBUFFER_STATIC_CIRCULAR_BUFFER_HPP
