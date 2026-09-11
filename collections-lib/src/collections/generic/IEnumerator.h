// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_I_ENUMERATOR_H_
#define _HC_LIB_I_ENUMERATOR_H_

#include "errors/GenericErrors.h"
#include "Expected.h"

/// @brief Supports a simple iteration over a generic collection.
template <typename T>
class IEnumerator
{
    public:
        IEnumerator() = default;
        virtual ~IEnumerator() = default;

        /// @brief Positions the enumerator before the first element.
        /// @note The preferred implementation is to move the enumerator 
        /// to the beginning of the collection, before the first element. 
        virtual void reset() = 0;

        /// @brief Advances to the next element.
        /// @return true if the enumerator is on an element; false if it passed the end.
        /// @note After an enumerator is created or after the Reset method is called, 
        /// an enumerator is positioned before the first element of the collection, 
        /// and the first call to the MoveNext method moves the enumerator over 
        /// the first element of the collection.
        /// If MoveNext passes the end of the collection, the enumerator is positioned 
        /// after the last element in the collection and MoveNext returns false. 
        /// When the enumerator is at this position, subsequent calls to MoveNext 
        /// also return false until Reset is called.
        virtual bool moveNext() = 0;

        /// @brief Returns the element at the current position.
        /// @pre moveNext() has succeeded since the last reset().
        /// @return The element in the collection at the current position of the enumerator.
        /// @note Current is undefined under any of the following conditions:
        ///   - The enumerator is positioned before the first element in the collection, 
        ///     immediately after the enumerator is created. MoveNext must be called to 
        ///     advance the enumerator to the first element of the collection before 
        ///     reading the value of Current.
        ///   - The last call to MoveNext returned false, which indicates the end of the collection.
        ///   - The enumerator is invalidated due to changes made in the collection, 
        ///     such as adding, modifying, or deleting elements.
        virtual Expected<T, Error> getCurrent() const = 0;
};

/// @brief Abstract enumerator that walks a collection by key.
/// @tparam T Item type. Must be trivially copyable.
/// @tparam TKey Key type. Must be default-constructible and trivially copyable.
/// @note Copy and move are disabled. Storage stays in the derived collection.
template <typename T, typename TKey>
class EnumeratorBase : public IEnumerator<T>
{
    protected:
        TKey m_current;
        bool m_isReseted;
        bool m_isEnded;

        /// @brief Returns the key of the first item.
        /// @return The first key, or an error when the collection is empty.
        virtual Expected<TKey, Error> getHead() const = 0;

        /// @brief Returns the key after _t_key_.
        /// @param t_key Current key.
        /// @return The next key, or an error when _t_key_ has no successor.
        virtual Expected<TKey, Error> getNext(TKey t_key) const = 0;

        /// @brief Returns the item for a valid key.
        /// @param t_key Key from getHead() or getNext(). Called only when the cursor is on an item.
        virtual T getByKey(TKey t_key) const = 0;

    public:
        EnumeratorBase()
            : m_current(), m_isReseted(true), m_isEnded(false)
        {}

        EnumeratorBase(const EnumeratorBase&) = delete;
        EnumeratorBase& operator=(const EnumeratorBase&) = delete;
        EnumeratorBase(EnumeratorBase&&) = delete;
        EnumeratorBase& operator=(EnumeratorBase&&) = delete;

        void reset() override
        {
            m_isReseted = true;
            m_isEnded = false;
        }

        bool moveNext() override
        {
            if (m_isEnded)
            {
                return false;
            }

            Expected<TKey, Error> key = m_isReseted ? getHead() : getNext(m_current);
            if (!key.hasValue())
            {
                m_isEnded = true;
                m_isReseted = false;
                return false;
            }

            m_current = key.getValue();
            m_isReseted = false;
            return true;
        }

        Expected<T, Error> getCurrent() const override
        {
            if (m_isReseted || m_isEnded)
            {
                return make_error(GenericError::InvalidOperation);
            }
            return getByKey(m_current);
        }
};

#endif
