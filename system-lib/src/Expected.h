// Copyright (c) 2012 Andrei Alexandrescu, Ph.D. (andrei@erdani.com). All Rights Reserved.
//
// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file Expected.h
/// @brief Defines the Expected{T, E} idiom for returning either a value or an error.
/// @note Exactly one of _T_ or _E_ is alive. A moved-from instance holds a default-constructed _E_.
#ifndef _HC_LIB_EXPECTED_H_
#define _HC_LIB_EXPECTED_H_

#include <new>
#include "lib-utility.h"

/// @brief Wraps an error so it can be distinguished from a success value of type _T_.
/// @tparam E The stored error type. Must be copyable or movable into _Expected_.
/// @note Use make_error() to construct an instance. This type is not the library _Error_ struct.
template <typename E>
class Unexpected
{
    public:
        /// @brief The stored error.
        E error;

        /// @brief Constructs an instance that holds a copy of _t_error_.
        /// @param t_error The error.
        Unexpected(const E &t_error): error(t_error) {};

        /// @brief Constructs an instance that holds _t_error_ by move.
        /// @param t_error The error to move from.
        Unexpected(E &&t_error): error(std::move(t_error)) {};
};

/// @brief Creates an error wrapper of type _Unexpected{E}_.
/// @tparam E The error type.
/// @param t_error The error to wrap. May be a struct, class, or enum.
/// @return An unexpected value that constructs _Expected{T, E}_.
template <typename E>
Unexpected<typename std::decay<E>::type> make_error(E&& t_error)
{
    return Unexpected<typename std::decay<E>::type>(std::forward<E>(t_error));
}

/// @brief Holds either a value of type _T_ or an error of type _E_.
/// @tparam T The success value type.
/// @tparam E The error type. Defaults to _Error_. Must be default-constructible.
/// @note Call hasValue() before getValue(). getValue() is undefined when the instance holds an error.
/// @note Exactly one of _T_ or _E_ is alive. A moved-from instance holds a default-constructed _E_.
template <typename T, typename E>
class Expected
{
    private:
        /// @brief The union of the value and error.
        union
        {
            T m_value;
            E m_error;
        };  

        /// @brief Indicates whether the instance holds a value.
        bool m_hasValue;

        /// @brief Destroys the active union member.
        void destroy()
        {
            if(m_hasValue)
            {
                m_value.~T();
            }
            else
            {
                m_error.~E();
            }
            m_hasValue = false;
        };

        /// @brief Moves the value or error from _t_other_ into an uninitialized instance.
        /// @param t_other The source instance. After the call it holds a default-constructed error.
        void _move(Expected &t_other)
        {
            m_hasValue = t_other.m_hasValue;
            if(m_hasValue)
            {
                ::new(static_cast<void*>(&m_value)) T(std::move(t_other.m_value));
            }
            else
            {
                ::new(static_cast<void*>(&m_error)) E(std::move(t_other.m_error));
            }            
        };

        /// @brief Copies the value or error from _t_other_ into an uninitialized instance.
        /// @param t_other The instance to copy.
        void _copy(const Expected &t_other)
        {
            m_hasValue = t_other.m_hasValue;
            if(m_hasValue)
            {
                ::new(static_cast<void*>(&m_value)) T(t_other.m_value);
            }
            else
            {
                ::new(static_cast<void*>(&m_error)) E(t_other.m_error);
            }
        };

    public:
        /// @brief Constructs an instance that holds a copy of _t_value_.
        /// @param t_value The success value.
        Expected(const T &t_value)
            : m_hasValue(true) {
                /// @note This is a placement new.
                ::new(static_cast<void*>(&m_value)) T(t_value);
            };

        /// @brief Constructs an instance that holds _t_value_ by move.
        /// @param t_value The success value to move from.
        Expected(T &&t_value) noexcept
            : m_hasValue(true) {
                /// @note This is a placement new.
                ::new(static_cast<void*>(&m_value)) T(std::move(t_value));
            };

        /// @brief Constructs an instance that holds _t_error_ by copy.
        /// @param t_error The error to copy from.
        Expected(const Unexpected<E> &t_error)
            : m_hasValue(false) {
                ::new(static_cast<void*>(&m_error)) E(t_error.error);
            };

        /// @brief Constructs an instance that holds _t_error_ by move.
        /// @param t_error The error to move from.
        Expected(Unexpected<E> &&t_error) noexcept
            : m_hasValue(false) {
                ::new(static_cast<void*>(&m_error)) E(std::move(t_error.error));
            };

        /// @brief Constructs an instance that holds an error converted from _t_error_.
        /// @tparam U A struct, class, or other type convertible to _E_.
        /// @param t_error The wrapped error to convert.
        template<typename U>
        Expected(const Unexpected<U> &t_error)
            : m_hasValue(false) {
                ::new(static_cast<void*>(&m_error)) E(t_error.error);
            };

        /// @brief Constructs an instance that holds an error converted from _t_error_.
        /// @tparam U A struct, class, or other type convertible to _E_.
        /// @param t_error The wrapped error to convert.
        template<typename U>
        Expected(Unexpected<U> &&t_error) noexcept
            : m_hasValue(false) {
                ::new(static_cast<void*>(&m_error)) E(std::move(t_error.error));
            };

        /// @brief Copies the value or error from _t_other_.
        /// @param t_other The instance to copy.
        Expected(const Expected &t_other)             
        {
            _copy(t_other);
        };

        /// @brief Moves the value or error from _t_other_.
        /// @param t_other The instance to move from. After the call it holds a default-constructed error.
        Expected(Expected &&t_other) noexcept
            : m_hasValue(false)
        {
            _move(t_other);
        };

        /// @brief Copy-assigns the value or error from _t_other_.
        /// @param t_other The instance to copy.
        /// @return A reference to this instance.
        Expected& operator=(const Expected &t_other)
        {
            /// @note This is a self-assignment check.
            if(this == &t_other)
            {
                return *this;
            }

            /// @note If the types are the same, we can just copy the value or error.
            if(m_hasValue == t_other.m_hasValue)
            {                
                if(m_hasValue)
                {
                    m_value = t_other.m_value;
                }
                else
                {
                    m_error = t_other.m_error;
                }
            }
            /// @note If the types are different, we need to destroy the current value or error and copy the new one.
            else
            {                
                destroy();
                _copy(t_other);
            }

            return *this;
        };

        /// @brief Move-assigns the value or error from _t_other_.
        /// @param t_other The instance to move from. After the call it holds a default-constructed error.
        /// @return A reference to this instance.
        Expected& operator=(Expected &&t_other) noexcept
        {
            /// @note This is a self-assignment check.
            if(this == &t_other)
            {
                return *this;
            }

            /// @note If the types are the same, we can just move the value or error.
            if(m_hasValue == t_other.m_hasValue)
            {
                if(m_hasValue)
                {
                    m_value = std::move(t_other.m_value);
                }
                else
                {
                    m_error = std::move(t_other.m_error);
                }
            }
            /// @note If the types are different, we need to destroy the current value or error and move the new one.
            else
            {
                destroy();
                _move(t_other);
            }

            return *this;
        };

        /// @brief Destroys the stored value or error.
        ~Expected()
        {
            destroy();
        };

        /// @brief Indicates whether this instance holds a value.
        /// @return true if a value is stored; false if an error is stored.
        bool hasValue() const
        {
            return m_hasValue;
        };

        /// @brief Converts the instance to a boolean.
        /// @return true if a value is stored; false if an error is stored.
        explicit operator bool() const
        {
            return hasValue();
        };

        /// @brief Returns a reference to the stored value.
        /// @return The success value.
        /// @pre hasValue() is true.
        T& getValue()
        {
            return m_value;
        };

        T& operator*()
        {
            return getValue();
        };

        /// @brief Returns a const reference to the stored value.
        /// @return The success value.
        /// @pre hasValue() is true.
        const T& getValue() const
        {
            return m_value;
        };

        const T& operator*() const
        {
            return getValue();
        };

        /// @brief Returns a reference to the stored error.
        /// @return The error.
        /// @pre hasValue() is false.
        E& getError()
        {
            return m_error;
        };

        /// @brief Returns a const reference to the stored error.
        /// @return The error.
        /// @pre hasValue() is false.
        const E& getError() const
        {
            return m_error;
        };
};

#endif
