// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file errors.h
/// @brief Defines the Error struct and the ErrorCategory class.
/// @note Compare categories by identity. Use one static instance per category.
#ifndef _HC_LIB_ERROR_H_
#define _HC_LIB_ERROR_H_

#include <stdint.h>

#if defined(ARDUINO)
    #include <Arduino.h>
    /// @brief Stores a string literal in flash on Arduino.
    #define ERROR_STR(str) F(str)
    /// @brief Pointer to an error message. On Arduino the text is kept in flash.
    using StringPtr = const __FlashStringHelper*;
#else
    /// @brief Stores a string literal in the binary on native builds.
    #define ERROR_STR(str) str
    /// @brief Pointer to an error message. On native builds this is a C string.
    using StringPtr = const char*;
#endif

/// @file error.h
/// Defines an error code plus category, similar to std::error_code.

/// @brief Maps an error code to a human-readable message.
/// @note Compare categories by identity. Use one static instance per category.
class ErrorCategory
{
    public:
    #if !defined(ARDUINO)
        /// @brief Destructor for native builds.
        virtual ~ErrorCategory() = default;
    #endif

        /// @brief Returns the message for _t_error_ in this category.
        /// @param t_error The category-specific error code.
        /// @return A platform string pointer (flash on Arduino, _const char*_ on native).
        virtual StringPtr message(uint8_t t_error) const = 0;
};

/// @brief An error code together with the category that interprets it.
/// @note Code 0 means no error (_operator bool()_ is false).
struct Error
{
    /// @brief Category-specific error code. 0 means no error.
    uint8_t error;
    /// @brief Category that owns _error_. Compared by pointer identity; must not be null when calling message().
    const ErrorCategory* category;

    /// @brief Equal when both the code and the category instance match.
    bool operator==(const Error& t_other) const
    {
        return error == t_other.error && category == t_other.category;
    }

    /// @brief true when _error_ is not 0.
    explicit operator bool() const
    {
        return error != 0;
    }

    /// @brief Returns the message for this error from _category_.
    /// @return An empty string when _category_ is null; otherwise _category->message(error)_.
    StringPtr message() const
    {
        if(!category)
        {
            return ERROR_STR("");
        }
        return category->message(error);
    }
};

#endif
