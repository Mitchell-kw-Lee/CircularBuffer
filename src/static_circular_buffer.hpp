//
// Created by emdash00 on 11/2/19.
//

#ifndef CIRCULARBUFFER_CIRCULAR_BUFFER_HPP
#define CIRCULARBUFFER_CIRCULAR_BUFFER_HPP

#include <stdlib.h>
#include <memory.h>
#include "circular_buffer_iterator.hpp"

#ifndef SIZE_MAX
    #define SIZE_MAX ((const size_t)(~0))
#endif



template <typename T>
class circular_buffer
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    T &operator[](size_type index)
    {
        return this->m_buf[index];
    }

    const T &operator[](size_type index) const
    {
        return this->m_buf[index];
    }

    explicit circular_buffer(size_type size) :
    m_buf(new T[size]), m_capacity(size) { }

    ~circular_buffer()
    {
        delete[] this->m_buf;
    }

    void clear()
    {
        this->m_head = this->m_tail = -1;
        this->m_size = 0;
    }

    bool empty() const
    {
        //if head and tail are equal, we are empty
        return this->m_size == 0;
    }

    bool full() const
    {
        return this->m_capacity == this->m_size;
    }

    reference front()
    {
        if (this->empty()) {
            //throw EXCEPTION_NONCONTINUABLE;
            throw std::out_of_range("queue empty");
        }
        return this->m_buf[this->m_head];
    }
    const_reference front() const
    {
        return this->front();
    }

    reference back()
    {
        if (this->empty()) {
            //throw EXCEPTION_NONCONTINUABLE;
            throw std::out_of_range("queue empty");
        }
        return this->m_buf[this->m_tail];
    }
    const_reference back() const
    {
        return this->back();
    }

    size_type getRealIndex(size_type nLogicalIndex) {
        if (!this->empty()) {
            return getNextIndex(this->m_head, nLogicalIndex); //get_LogicalIndex
        }
        return -1;
    }

    size_type getLogicalIndex(size_type real_index) {
        if (!this->empty()) {
            if (real_index < this->m_head) {

                return real_index + ((this->m_capacity - 1) - this->m_head);
            }
            return real_index - this->m_head;
        }
        return -1;
    }

    size_type getRealIndexAtLast() {
        return this->m_tail;
    }

    size_type getRealIndexAtFirst() {
        return this->m_head;
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
       if (index < this->m_logical_capacity && index >= 0)
       {
           return(this->m_buf[index]);
       }

       throw std::out_of_range("Index out of range");
    }

    reference from_back(size_type index)
    {
        if (index < this->m_logical_capacity && index >= 0)
        {
            return(this->m_buf[(this->m_logical_capacity - 1) - index]);
        }

        throw std::out_of_range("'index' must be between a valid array index.");
    }

    const_reference from_back(size_type index) const
    {
        if (index < this->m_capacity && index >= 0)
        {
            return(this->m_buf[(this->m_logical_capacity - 1) - index]);
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
        if (this->full()) {
            throw std::out_of_range("buffer full");
        }

        increment_tail();
        this->m_buf[this->m_tail] = item;

        if (this->size() == 1) {
            this->m_head = this->m_tail;
        }
   }

    reference pop_front()
    {
        reference value = front();
        this->increment_head();
        return value;
    }

    void resize(size_type n)
    {
        if (n <= 0)
        {
            throw std::domain_error("'n' must be at least 1");
        }

        pointer arr = new value_type[n];

        if (this->m_capacity > n)
        {
            size_type i = 0;
            size_type start = this->m_head + (this->m_capacity - n);

            // Efficient but needs a lot of bookkeeping.
            // Only do it for size > 10
            if (this->m_size > 10)
            {
                // Check if the array is fragmented
                if ((this->m_tail - n) < 0) {
                    const size_type seg1_begin = ((this->m_buf + this->m_capacity - 1) - n);
                    const size_type seg1_end = this->m_capacity - 1;
                    const size_type seg1_size = seg1_end - seg1_begin;

                    memccpy(arr, seg1_begin, seg1_end);
                    memccpy(arr + seg1_size, this->m_buf, this->m_tail);
                }
                else
                {
                    memccpy(arr, this->m_tail - n, this->m_tail);
                }
            }
            else
            {
                do
                {
                    arr[i] = this->m_buf[(start + i) % this->m_logical_capacity];
                    ++i;
                } while(i < n);
            }

            this->m_head = 0;
            this->m_tail = n - 1;

            this->m_capacity = n;
            //this->m_logical_capacity = this->m_capacity;
        }
        else if (this->m_capacity <  n)
        {
            memcpy(arr, this->m_buf, this->m_capacity * sizeof(value_type));
        }

        delete[] this->m_buf;
        this->m_capacity = n;
        //this->m_full = false;

        this->m_buf = arr;
    }

    void reserve(size_type n)
    {
        if (n <= 0)
        {
            throw std::domain_error("'n' must be at least 1");
        }

        if (this->m_capacity > n)
        {
            // Shrinking
            // Optimize for time so don't actually resize, consume old elements.
            do
            {
                increment_head();
                --this->m_logical_capacity;
            } while(this->m_logical_capacity > n);
        }
        else if (this->m_capacity <  n)
        {
            // Growing
            pointer arr = new value_type[n];
            memcpy(arr, this->m_buf, this->m_capacity * sizeof(value_type));

            delete[] this->m_buf;
            this->m_capacity = n;
            //this->m_full = false;

            this->m_buf = arr;
        }
    }

private:
    value_type *m_buf;

    size_type m_head = -1, m_tail = -1;
    size_type m_size = 0, m_capacity;

    inline size_type getNextIndex(size_type baseIndex, size_type offset) {
        return (baseIndex + offset) % (this->m_capacity - 1);
    }

    inline void increment_tail()
    {
        this->m_tail = getNextIndex(this->m_tail, 1);
        ++this->m_size;
    }

    inline void increment_head()
    {
        this->m_head = getNextIndex(this->m_head, 1);
        --this->m_size;
    }
};

#endif //CIRCULARBUFFER_CIRCULAR_BUFFER_HPP
