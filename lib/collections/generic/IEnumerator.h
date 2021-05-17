// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_ENUMERATOR_
#define _I_ENUMERATOR_

//#include <type_traits>

template <class T>
class IEnumerator
{
    public:
        IEnumerator() {};
        
        // Sets the enumerator to its initial position, 
        // which is before the first element in the collection.
        //
        // The preferred implementation is to move the enumerator 
        // to the beginning of the collection, before the first element.  
        virtual void reset() {};

        // Advances the enumerator to the next element of the collection.
        // Returns true if the enumerator was successfully advanced to 
        // the next element; false if the enumerator has passed the end of the collection.
        //
        // After an enumerator is created or after the Reset method is called, 
        // an enumerator is positioned before the first element of the collection, 
        // and the first call to the MoveNext method moves the enumerator over 
        // the first element of the collection.
        // If MoveNext passes the end of the collection, the enumerator is positioned 
        // after the last element in the collection and MoveNext returns false. 
        // When the enumerator is at this position, subsequent calls to MoveNext 
        // also return false until Reset is called.
        virtual bool moveNext() = 0;

        // Gets the element in the collection at the current position of the enumerator.
        // Current is undefined under any of the following conditions:
        //   - The enumerator is positioned before the first element in the collection, 
        //     immediately after the enumerator is created. MoveNext must be called to 
        //     advance the enumerator to the first element of the collection before 
        //     reading the value of Current.
        //   - The last call to MoveNext returned false, which indicates the end of the collection.
        //   - The enumerator is invalidated due to changes made in the collection, 
        //     such as adding, modifying, or deleting elements.
        virtual T getCurrent() = 0;
};

template <class T>
struct EnumeratedItem
{
    T item;
    EnumeratedItem<T> *next = nullptr;
};

template <class T>
class EnumeratorBase : public IEnumerator<T>
{
    protected:
        EnumeratedItem<T> *m_head;
        EnumeratedItem<T> *m_current;
        bool m_isReseted = false;

    public:
        const T NULLITEM = {};

        EnumeratorBase()
        {
            m_head = nullptr;
            m_current = nullptr;
        }
        
        ~EnumeratorBase()
        {
            if(m_head)
            {
                m_current = m_head;            
                do
                {
                    EnumeratedItem<T> *item = m_current->next;
                    delete m_current;
                    m_current = item;
                }
                while(!m_current);
            }
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