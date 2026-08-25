// Copyright (c) 2012 Andrei Alexandrescu, Ph.D. (andrei@erdani.com). All Rights Reserved.
//
// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _EXPECTED_H_
#define _EXPECTED_H_

#include "errdef.h"
#include <new>
#include "move.h"

template <class T>
class Expected
{
    private:
        union
        {
            T m_value;
            err_t m_error;
        };  

        bool m_hasValue;

        Expected()
            : m_hasValue(false)
        {
            m_error = NO_ERROR;
        };

        void destroyValue()
        {
            if(m_hasValue)
            {
                m_value.~T();
                m_hasValue = false;
            }
        };

        void moveFrom(Expected &t_expected)
        {
            m_hasValue = t_expected.m_hasValue;
            if(m_hasValue)
            {
                new(&m_value) T(std::move(t_expected.m_value));
                t_expected.m_value.~T();
                t_expected.m_hasValue = false;
                t_expected.m_error = NO_ERROR;
            }
            else
            {
                m_error = t_expected.m_error;
            }
        };

    public:
        Expected(const T &t_value)
            : m_value(t_value), m_hasValue(true) {};

        Expected(const Expected &t_expected) 
            : m_hasValue(t_expected.m_hasValue) 
        {
            if (m_hasValue) 
            {
                new(&m_value) T(t_expected.m_value);
            }
            else 
            {
                m_error = t_expected.m_error;
            }
        };

        Expected(T &&t_value)
            : m_value(std::move(t_value)), m_hasValue(true) {};

        Expected(Expected &&t_expected) 
            : m_hasValue(false)
        {
            m_error = NO_ERROR;
            moveFrom(t_expected);
        };

        Expected& operator=(const Expected &t_expected)
        {
            if(this == &t_expected)
            {
                return *this;
            }

            destroyValue();
            m_hasValue = t_expected.m_hasValue;
            if(m_hasValue)
            {
                new(&m_value) T(t_expected.m_value);
            }
            else
            {
                m_error = t_expected.m_error;
            }
            return *this;
        };

        Expected& operator=(Expected &&t_expected)
        {
            if(this == &t_expected)
            {
                return *this;
            }

            destroyValue();
            moveFrom(t_expected);
            return *this;
        };

        ~Expected()
        {
            destroyValue();
        };

        bool hasValue() const
        {
            return m_hasValue;
        };

        T& getValue()
        {
            return m_value;
        };

        const T& getValue() const
        {
            return m_value;
        };

        err_t getError() const
        {
            if(!hasValue())
            {
                return m_error;
            }
            return NO_ERROR;
        };

        static Expected<T> fromError(err_t t_error) 
        {
            Expected<T> result;
            result.m_error = t_error;
            return result;
        };
};

#endif
