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
#include "Expected.h"
#include "errors/GenericErrors.h"

/// @brief First-in, first-out ring of trivial (POD) elements.
/// @tparam T Element type. Must be trivially copyable.
/// @tparam CAPACITY Number of slots (1..65535). All slots are usable. Default 16.
/// @note Copy and move are disabled. Storage is a fixed array (no malloc).
template<typename T, uint16_t CAPACITY = 16>
class Queue
{
    static_assert(CAPACITY >= 1, "Queue CAPACITY must be at least 1.");

private:
    /// @brief Index of the front element.
    uint16_t m_head;
    /// @brief Number of stored elements (0..CAPACITY).
    uint16_t m_count;
    /// @brief Data storage.
    T m_data[CAPACITY];

    uint16_t indexAt(uint16_t t_offset) const;

public:
    /// @brief Initializes an empty queue. Capacity is CAPACITY.
    Queue()
        : m_head(0), m_count(0) {};

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

    /// @brief Removes all elements from the Queue.
    /// @note The capacity of the Queue is unchanged.
    void clear();

    /// @brief Number of elements currently stored.
    uint16_t count() const;

    /// @brief Removes and returns the front element.
    /// @return The element, or GenericError::InvalidOperation if empty.
    Expected<T, Error> dequeue();

    /// @brief Appends an element.
    /// @return True if the element is added, false if the buffer is full.    
    /// @note The element is added to the end of the Queue.
    bool enqueue(const T &t_item);

    /// @brief Returns the front element without removing it.
    /// @return The element, or GenericError::InvalidOperation if empty.
    Expected<T, Error> peek() const;
};

template<typename T, uint16_t CAPACITY>
uint16_t Queue<T, CAPACITY>::indexAt(uint16_t t_offset) const
{
    uint32_t index = static_cast<uint32_t>(m_head) + t_offset;
    if(index >= CAPACITY)
    {
        index -= CAPACITY;
    }
    return static_cast<uint16_t>(index);
}

template<typename T, uint16_t CAPACITY>
void Queue<T, CAPACITY>::clear()
{
    m_head = 0;
    m_count = 0;
}

template<typename T, uint16_t CAPACITY>
uint16_t Queue<T, CAPACITY>::count() const
{
    return m_count;
}

template<typename T, uint16_t CAPACITY>
Expected<T, Error> Queue<T, CAPACITY>::dequeue()
{
    if(m_count == 0)
    {
        return make_error(GenericError::InvalidOperation);
    }
    T item = m_data[m_head];
    m_head = indexAt(1);
    --m_count;
    return item;
}

template<typename T, uint16_t CAPACITY>
bool Queue<T, CAPACITY>::enqueue(const T &t_item)
{
    if(m_count == CAPACITY)
    {
        return false;
    }
    m_data[indexAt(m_count)] = t_item;
    ++m_count;
    return true;
}

template<typename T, uint16_t CAPACITY>
Expected<T, Error> Queue<T, CAPACITY>::peek() const
{
    if(m_count == 0)
    {
        return make_error(GenericError::InvalidOperation);
    }
    return m_data[m_head];
}

#endif
