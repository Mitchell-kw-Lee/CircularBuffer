//
// Created by emdash00 on 11/2/19.
//

#ifndef CIRCULARBUFFER_CIRCULAR_BUFFER_HPP
#define CIRCULARBUFFER_CIRCULAR_BUFFER_HPP

#include <stdlib.h>
#include <string.h>

#ifndef SIZE_MAX
    #define SIZE_MAX ((const size_t)(~0))
#endif

template <typename... _T>
class circular_buffer;

template <typename T>
class circular_buffer<T>
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    friend class circular_buffer_iterator;

    T &operator[](size_type index)
    {
        return this->m_buf[index];
    }

    const T &operator[](size_type index) const
    {
        return this->m_buf[index];
    }

    explicit circular_buffer(size_type size) :
    m_buf(new T[size]), m_logical_capacity(size), m_capacity(size) { }

    ~circular_buffer()
    {
        delete[] this->m_buf;
    }

    void clear()
    {
        this->m_head = this->m_tail;
        this->m_full = false;
        this->m_empty = true;
    }

    bool empty() const
    {
        //if head and tail are equal, we are empty
        return this->m_empty;
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
        if (this->m_full)
        {
            this->increment_head();
        }

        if (this->m_empty)
        {
            this->m_empty = false;
            this->m_size++;
        }
        else
        {
            this->increment_tail();
        }

        this->m_buf[this->m_tail] = item;
    }

    void pop_front()
    {
        this->increment_head();
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
            // Copy n elements
            do
            {
                arr[i] = this->m_buf[(start + i) % this->m_logical_capacity];
                ++i;
            } while(i < n);
            this->m_head = 0;
            this->m_tail = n - 1;

            this->m_capacity = n;
            this->m_logical_capacity = this->m_capacity;
        }
        else if (this->m_capacity <  n)
        {
            memcpy(arr, this->m_buf, this->m_capacity * sizeof(value_type));
        }

        delete[] this->m_buf;
        this->m_capacity = n;
        this->m_full = false;

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
            this->m_full = false;

            this->m_buf = arr;
        }
    }

    /*
    void resize(size_type n, const_reference val)
    {
        pointer arr = new value_type[n];
        memcpy(arr, this->m_buf, this->m_capacity * sizeof(value_type));
        delete[] this->m_buf;
        do
        {
            this->m_buf[this->m_capacity] = val;
            this->m_capacity++;
        } while(this->m_capacity < this->m_buf);
        this->m_buf = arr;
    }
    */

private:
    value_type *m_buf;

    bool m_full = false, m_empty = true;

    size_type m_head = 0, m_tail = 0; // head = tail and we're empty
    size_type m_size = 0, m_logical_capacity, m_capacity;

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

#endif //CIRCULARBUFFER_CIRCULAR_BUFFER_HPP
