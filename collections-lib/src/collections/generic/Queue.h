// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_QUEUE_H_
#define _HC_LIB_QUEUE_H_

#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "Expected.h"
#include "errors/GenericErrors.h"

#define GROWTH_FACTOR 2
#define DEFAULT_CAPACITY 4

/// @brief First-in, first-out buffer of trivial (POD) elements.
/// @tparam T Element type. Must be trivially copyable; storage is malloc/realloc/free.
/// @note Copy and move are disabled. The buffer is not a ring; dequeue shifts remaining items.
template<typename T>
class Queue
{
private:
    uint16_t m_count;
    uint16_t m_capacity;
    T* m_data;

    void shift();
    bool tryAllocate(uint16_t t_size);
    bool tryResize();   
    bool tryTrim();     
public:
    /// @brief Initializes a new instance of the Queue class that is empty and has the default initial capacity.
    /// @note The default initial capacity is 4.
    Queue()
        : Queue(DEFAULT_CAPACITY) {};

    /// @brief Initializes a new instance of the Queue class that is empty and has the specified initial capacity.
    /// @param t_capacity The initial number of elements that the Queue can contain.
    /// @note The initial capacity is 0.
    explicit Queue(uint16_t t_capacity);

    /// @brief Prevents copying and moving of the Queue.
    /// @note Copy and move are disabled.
    Queue(const Queue&) = delete;

    /// @brief Prevents copying and moving of the Queue.
    /// @note Copy and move are disabled.
    Queue& operator=(const Queue&) = delete;

    /// @brief Prevents copying and moving of the Queue.
    /// @note Copy and move are disabled.
    Queue(Queue&&) = delete;

    /// @brief Prevents copying and moving of the Queue.
    /// @note Copy and move are disabled.
    Queue& operator=(Queue&&) = delete;

    /// @brief Destroys the Queue.
    ~Queue();

    /// @brief Removes all elements from the Queue.
    /// @note The capacity of the Queue is unchanged.
    void clear();

    /// @brief Gets the number of elements contained in the Queue.
    /// @return The number of elements contained in the Queue.
    /// @note The capacity of the Queue is unchanged.
    uint16_t count() const;

    /// @brief Removes and returns the front element.
    /// @return The element, or GenericError::InvalidOperation if empty.
    /// @note The element is removed from the front of the Queue.
    Expected<T, Error> dequeue();

    /// @brief Appends an element.
    /// @return GenericError::NoError, or GenericError::OutOfMemory if the buffer cannot grow.    
    /// @note The element is added to the end of the Queue.
    Error enqueue(const T &t_item);

    /// @brief Returns the front element without removing it.
    /// @return The element, or GenericError::InvalidOperation if empty.
    /// @note The element is not removed from the front of the Queue.
    Expected<T, Error> peek();
};

template<typename T>
Queue<T>::Queue(uint16_t t_capacity)
    : m_count(0), m_capacity(0), m_data(nullptr)
{
    if(tryAllocate(t_capacity))
    {
        m_capacity = t_capacity;
    }
};

template<typename T>
Queue<T>::~Queue()
{
    if(m_data)
    {
        free(m_data);
    }
    m_data = nullptr;
};

template<typename T>
void Queue<T>::clear()
{
    if(m_count == 0)
    {
        return;        
    }

    if(m_data)
    {
        free(m_data);
        m_data = nullptr;
    }
    m_count = 0;
    m_capacity = 0;
    if(tryAllocate(DEFAULT_CAPACITY))
    {
        m_capacity = DEFAULT_CAPACITY;
    }
};

template<typename T>
uint16_t Queue<T>::count() const
{
    return m_count;
};

template<typename T>
Expected<T, Error> Queue<T>::dequeue()
{
    if(m_count != 0)
    {
        T item = (*m_data);
        m_count--;

        shift();

        return item;
    }
    return make_error(GenericError::InvalidOperation);
};

template<typename T>
Expected<T, Error> Queue<T>::peek()
{
    if(m_count != 0)
    {
        T item = (*m_data);
        return item;
    }
    return make_error(GenericError::InvalidOperation);
};

template<typename T>
Error Queue<T>::enqueue(const T &t_item)
{
    if(tryResize())
    {
        *(m_data + m_count) = t_item;
        m_count++;
        return to_error(GenericError::NoError);
    }
    return to_error(GenericError::OutOfMemory);
};

template<typename T>
void Queue<T>::shift()
{
    if(m_count == 0)
    {
        return;
    }
    memmove(m_data, m_data + 1, static_cast<size_t>(m_count) * sizeof(T));
    tryTrim();
};

template<typename T>
bool Queue<T>::tryAllocate(uint16_t t_size)
{
    if(t_size == 0)
    {
        m_data = nullptr;
        return false;
    }
    m_data = (T*) malloc(sizeof(T) * t_size);
    return m_data != NULL;
}

template<typename T>
bool Queue<T>::tryResize()
{
    if(m_count < m_capacity)
    {
        return true;
    }
    
    if(m_capacity == 0)
    {
        if(tryAllocate(DEFAULT_CAPACITY))
        {
            m_capacity = DEFAULT_CAPACITY;
            return true;
        }
    }
    else if(m_capacity < UINT16_MAX)
    {
        uint16_t newCapacity = m_capacity < UINT16_MAX/GROWTH_FACTOR ? m_capacity * GROWTH_FACTOR : UINT16_MAX;
        
        auto newMemBlock = realloc(m_data, sizeof(T) * newCapacity);
        if(newMemBlock != NULL)
        {
            m_data = (T*) newMemBlock;
            m_capacity = newCapacity;
            return true;
        }
    }

    return false;
};

template<typename T>
bool Queue<T>::tryTrim()
{
    if(m_capacity <= DEFAULT_CAPACITY)
    {
        return true;
    }

    if(m_count <= m_capacity / (2*GROWTH_FACTOR))
    {
        auto newMemBlock = realloc(m_data, sizeof(T) * (m_capacity / GROWTH_FACTOR));
        if(newMemBlock != NULL)
        {
            m_data = (T*) newMemBlock;
            m_capacity = m_capacity / GROWTH_FACTOR;
            return true;
        }
        return false;
    }
    return true;
}

#endif
