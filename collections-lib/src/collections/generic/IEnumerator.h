// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_I_ENUMERATOR_H_
#define _HC_LIB_I_ENUMERATOR_H_

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
        virtual T getCurrent() = 0;
};

template <class T>
struct EnumeratedItem
{
    T item;
    EnumeratedItem<T> *next = nullptr;
};

/// @brief Abstract enumerator that walks a singly linked list of EnumeratedItem{T}.
/// @note Owns the list. Copy and move are disabled.
template <class T>
class EnumeratorBase : public IEnumerator<T>
{
    protected:
        EnumeratedItem<T> *m_head;
        EnumeratedItem<T> *m_current;
        bool m_isReseted;

    public:
        const T NULLITEM = {};

        EnumeratorBase()
            : m_head(nullptr), m_current(nullptr), m_isReseted(true)
        {}

        EnumeratorBase(const EnumeratorBase&) = delete;
        EnumeratorBase& operator=(const EnumeratorBase&) = delete;
        EnumeratorBase(EnumeratorBase&&) = delete;
        EnumeratorBase& operator=(EnumeratorBase&&) = delete;

        ~EnumeratorBase() override
        {
            m_current = m_head;
            while(m_current)
            {
                EnumeratedItem<T> *item = m_current->next;
                delete m_current;
                m_current = item;
            }
            m_head = nullptr;
        }

        void reset() override
        {
            m_isReseted = true;
            m_current = nullptr;
        };

        bool moveNext() override
        {
            if (!m_head)
            {
                return false;
            }

            if(m_isReseted)
            {
                m_current = m_head;
                m_isReseted = false;
            }
            else if (m_current && m_current->next)
            {
                m_current = m_current->next;
            }
            else
            {
                m_current = nullptr;
                return false;
            }

            return true;
        };

        T getCurrent() override
        {
            if(!m_current)
            {
                return T();
            }
            return m_current->item;            
        };
};

#endif
