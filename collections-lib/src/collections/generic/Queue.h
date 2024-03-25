// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _QUEUE_
#define _QUEUE_

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "Expected.h"

#define GROWTH_FACTOR 2
#define DEFAULT_CAPACITY 4

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
    // Initializes a new instance of the Queue class that 
    // is empty, has the default initial capacity
    Queue()
        : Queue(DEFAULT_CAPACITY) {};
    // Initializes a new instance of the Queue class that 
    // is empty, has the specified initial capacity
    Queue(uint16_t t_capacity);
    ~Queue();

    // Removes all objects from the Queue
    void clear();
    // Gets the number of elemets contained in the Queue
    uint16_t count();
    // Removes and returns the element at the beginning 
    // of the Queue.
    // Returns INVALID_OPERATION_ERROR if the Queue is empty.
    Expected<T> dequeue();
    // Adds an element to the end of the Queue.
    // Returns INVALID_OPERATION_ERROR if the Queue is full 
    // and cannot be extended
    err_t enqueue(const T t_item);
    // Returns the object at the beginning of the Queue
    // without removing it
    // Returns INVALID_OPERATION_ERROR if the Queue is empty.
    Expected<T> peek();
};

template<typename T>
Queue<T>::Queue(uint16_t t_capacity)
{
    m_count = 0;
    m_capacity = 0;
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
    }
    m_count = 0;
    m_capacity = 0;
    if(tryAllocate(DEFAULT_CAPACITY))
    {
        m_capacity = DEFAULT_CAPACITY;
    }
};

template<typename T>
uint16_t Queue<T>::count()
{
    return m_count;
};

template<typename T>
Expected<T> Queue<T>::dequeue()
{
    if(m_count != 0)
    {
        T item = (*m_data);
        m_count--;

        shift();

        return item;
    }
    return Expected<T>::fromError(INVALID_OPERATION_ERROR);
};

template<typename T>
Expected<T> Queue<T>::peek()
{
    if(m_count != 0)
    {
        T item = (*m_data);
        return item;
    }
    return Expected<T>::fromError(INVALID_OPERATION_ERROR);
};

template<typename T>
err_t Queue<T>::enqueue(const T t_item)
{
    if(tryResize())
    {
        *(m_data + m_count) = t_item;
        m_count++;
        return NO_ERROR;
    }
    return INVALID_OPERATION_ERROR;
};

template<typename T>
void Queue<T>::shift()
{
    if(m_count == 0)
    {
        return;
    }
    memmove(m_data, m_data + 1, m_count);
    tryTrim();
};

template<typename T>
bool Queue<T>::tryAllocate(uint16_t t_size)
{
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