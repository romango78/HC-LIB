// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ARRAY_H_
#define _HC_LIB_ARRAY_H_

#include <stddef.h>
#include "collections/generic/IFixedCollection.h"
#include "errors/GenericErrors.h"

/// @brief Fixed-length array of trivial elements.
/// @tparam T Element type. Must be trivially copyable. A type that allocates on the heap and frees that memory in its destructor is not supported.
/// @tparam LENGTH Number of elements (1..65535). Every slot is occupied.
/// @note Copy and move are disabled. Storage is a compile-time array (no malloc).
/// The enumerator is the only heap allocation.
/// clear() and setValue() assign into the existing slot. They do not destroy the stored element first.
/// For a trivially copyable T that assignment replaces the bytes. For a heap-owning T the previous allocation is not released, so clear() leaks, and destroying the Array can free the same block again.
template<typename T, uint16_t LENGTH>
class Array : public IFixedCollection<T>
{
    static_assert(LENGTH >= 1, "Array LENGTH must be at least 1.");

private:
    /// @brief Element storage. Index 0 is the first element.
    T m_items[LENGTH];

    /// @brief Walks indexes 0 .. LENGTH - 1. The array keeps the elements.
    class Enumerator : public EnumeratorBase<T, uint16_t>
    {
        private:
            /// @brief Array being enumerated. Not owned.
            const Array* const m_array;

        protected:
            /// @brief Returns the index of the first element.
            /// @return 0. The array is never empty.
            Expected<uint16_t, Error> getHead() const override
            {
                return static_cast<uint16_t>(0);
            }

            /// @brief Returns the index after _t_key_.
            /// @param t_key Current index.
            /// @return t_key + 1, or GenericError::InvalidOperation when _t_key_ is the last index.
            Expected<uint16_t, Error> getNext(uint16_t t_key) const override
            {
                if (static_cast<uint32_t>(t_key) + 1u >= LENGTH)
                {
                    return make_error(GenericError::InvalidOperation);
                }
                return static_cast<uint16_t>(t_key + 1u);
            }

            /// @brief Returns the element at _t_key_.
            /// @param t_key Index previously returned by getHead() or getNext().
            /// @return A copy of the stored element.
            T getByKey(uint16_t t_key) const override
            {
                return m_array->m_items[t_key];
            }

        public:
            /// @brief Binds the enumerator to _t_array_.
            /// @param t_array Array to walk. Must outlive the enumerator.
            explicit Enumerator(const Array* t_array)
                : m_array(t_array)
            {}
    };

public:
    /// @brief Initializes every element with T().
    Array()
        : m_items()
    {}

    /// @brief Prevents copying of the Array.
    Array(const Array&) = delete;

    /// @brief Prevents copying of the Array.
    Array& operator=(const Array&) = delete;

    /// @brief Prevents moving of the Array.
    Array(Array&&) = delete;

    /// @brief Prevents moving of the Array.
    Array& operator=(Array&&) = delete;

    /// @brief Number of elements. Always LENGTH.
    /// @return LENGTH.
    uint32_t getCount() const override
    {
        return LENGTH;
    }

    /// @brief Returns a reference to the element at _t_index_.
    /// @param t_index Zero-based index. Must be less than getCount().
    /// @return The stored element. An index past the end returns a default T() that is not stored in the array.
    T& operator[](const uint32_t t_index) override
    {
        if (t_index >= LENGTH)
        {
            static T outOfRange = T();
            return outOfRange;
        }
        return m_items[t_index];
    }

    /// @brief Returns a copy of the element at _t_index_.
    /// @param t_index Zero-based index.
    /// @return The stored element, or GenericError::OutOfRange when _t_index_ is past the end.
    Expected<T, Error> getValue(const uint32_t t_index) const
    {
        if (t_index >= LENGTH)
        {
            return make_error(GenericError::OutOfRange);
        }
        return m_items[t_index];
    }

    /// @brief Replaces the element at _t_index_.
    /// @param t_item Element to store.
    /// @param t_index Zero-based index.
    /// @return True, or GenericError::OutOfRange when _t_index_ is past the end.
    Expected<bool, Error> setValue(const T& t_item, const uint32_t t_index)
    {
        if (t_index >= LENGTH)
        {
            return make_error(GenericError::OutOfRange);
        }
        m_items[t_index] = t_item;
        return true;
    }

    /// @brief Writes T() into every element.
    /// @note The length is unchanged.
    void clear() override
    {
        for (uint16_t i = 0; i < LENGTH; ++i)
        {
            m_items[i] = T();
        }
    }

    /// @brief Returns an enumerator positioned before the first element.
    /// @return An enumerator that walks indexes 0 .. LENGTH - 1.
    /// @note The caller owns the enumerator through the unique_ptr.
    std::unique_ptr<IEnumerator<T>> getEnumerator() const override
    {
        return std::unique_ptr<IEnumerator<T>>(new Enumerator(this));
    }
};

#endif
