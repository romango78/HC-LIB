// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_ENUMERATOR_
#define _I_ENUMERATOR_

template <class T>
class IEnumerator
{
    public:
        IEnumerator() {};
        
        virtual void reset() {};

        virtual bool moveNext() {};

        virtual T getCurrent() {};
};

template <class T>
struct EnumeratedItem : IEnumerator
{
    T item;
    EnumeratedItem<T> *next = nullptr;
};

template <class T>
class EnumeratorBase
{
    private:
        const constexpr T NULLITEM = {};

    protected:
        EnumeratedItem<T> *m_head;
        EnumeratedItem<T> *m_current;

    public:
        IEnumerator()
        {
            m_head = nullptr;
            m_current = nullptr;
        }
        
        ~IEnumerator()
        {
            reset();
            EnumeratedItem<T> item = m_head;
            while(moveNext())
            {
                delete item;
                item = getCurrent();
            }
            delete item;
        }

        void reset() override
        {
            m_current = m_head;
        };

        bool moveNext() override
        {
            if (!m_head)
            {
                return false;
            }

            if (!m_current)
            {
                reset();
            }
            else if (m_current->next)
            {
                m_current = m_current->next;
            }
            else
            {
                return false;
            }

            return true;
        };

        T getCurrent() override
        {
            if (!m_current)
            {
                return NULLITEM;
            }
            return m_current->item;            
        };
};

#endif