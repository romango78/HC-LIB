// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file lib-utility.h
/// @brief Provides std::move, std::forward, and the type traits they need on Arduino.
/// On native (non-Arduino) builds this header includes the C++ utility and type_traits headers.
#ifndef _HC_LIB_UTILITY_H_
#define _HC_LIB_UTILITY_H_

#ifdef ARDUINO

namespace std
{
    /// @brief Obtains the non-reference type of _T_.
    /// @tparam T The type to transform.
    template<typename T>
    struct remove_reference
    {
        /// The type _T_ without a reference qualifier.
        typedef T type;
    };

    /// @brief Specialization for lvalue references.
    /// @tparam T The type referred to by _T&_.
    template<typename T>
    struct remove_reference<T&>
    {
        /// The type _T_ without the lvalue reference.
        typedef T type;
    };

    /// @brief Specialization for rvalue references.
    /// @tparam T The type referred to by _T&&_.
    template<typename T>
    struct remove_reference<T&&>
    {
        /// The type _T_ without the rvalue reference.
        typedef T type;
    };

    /// @brief Obtains the type of _T_ without const or volatile.
    /// @tparam T The type to transform.
    template<typename T>
    struct remove_cv
    {
        /// The type _T_ without const or volatile.
        typedef T type;
    };

    /// @brief Specialization for const types.
    /// @tparam T The type referred to by _const T_.
    template<typename T>
    struct remove_cv<const T>
    {
        /// The type _T_ without const.
        typedef T type;
    };

    /// @brief Specialization for volatile types.
    /// @tparam T The type referred to by _volatile T_.
    template<typename T>
    struct remove_cv<volatile T>
    {
        /// The type _T_ without volatile.
        typedef T type;
    };

    /// @brief Specialization for const volatile types.
    /// @tparam T The type referred to by _const volatile T_.
    template<typename T>
    struct remove_cv<const volatile T>
    {
        /// The type _T_ without const or volatile.
        typedef T type;
    };

    /// @brief Obtains the decayed type of _T_ (no reference, no cv-qualifiers).
    /// @tparam T The type to transform.
    /// @note Array and function types are not converted to pointers.
    template<typename T>
    struct decay
    {
        /// The value type of _T_ after dropping references and cv-qualifiers.
        typedef typename remove_cv<typename remove_reference<T>::type>::type type;
    };

    /// @brief Casts _t_ to an rvalue so it can be moved from.
    /// @tparam T The type of the argument.
    /// @param t The object to treat as movable.
    /// @return An rvalue reference to _t_.
    /// @note This does not move any members by itself; it only changes value category.
    template<typename T>
    constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }

    /// @brief Forwards an lvalue as either an lvalue or an rvalue.
    /// @tparam T The type to forward as.
    /// @param t The lvalue to forward.
    /// @return A reference of type _T&&_.
    template<typename T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    /// @brief Forwards an rvalue as an rvalue.
    /// @tparam T The type to forward as.
    /// @param t The rvalue to forward.
    /// @return An rvalue reference of type _T&&_.
    template<typename T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
    {
        return static_cast<T&&>(t);
    }
}

#else
#include <utility>
#include <type_traits>
#endif

#endif
