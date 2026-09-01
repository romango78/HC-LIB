// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file GenericErrors.h
/// @brief Defines the GenericError enum and the to_error function.
/// @note Compare categories by identity. Use one static instance per category.
#ifndef _HC_LIB_GENERIC_ERRORS_H_
#define _HC_LIB_GENERIC_ERRORS_H_

#include "errors.h"
#include "Expected.h"

#define GENERIC_ERROR_NO_ERROR                ERROR_STR("No error")
#define GENERIC_ERROR_ARGUMENT_IS_NULL        ERROR_STR("Argument is null")
#define GENERIC_ERROR_OUT_OF_RANGE            ERROR_STR("Out of range")
#define GENERIC_ERROR_OUT_OF_MEMORY           ERROR_STR("Out of memory")
#define GENERIC_ERROR_NOT_SUPPORTED_OPERATION ERROR_STR("Not supported operation")
#define GENERIC_ERROR_INVALID_OPERATION       ERROR_STR("Invalid operation")
#define GENERIC_ERROR_UNKNOWN_ERROR           ERROR_STR("Unknown error")

/// @brief Generic error codes.
enum class GenericError : uint8_t
{
    NoError = 0,
    ArgumentIsNull,
    OutOfRange,
    OutOfMemory,
    NotSupportedOperation,
    InvalidOperation,
    UnknownError
};

/// @brief Converts a GenericError to an Error.
/// @param t_error The GenericError to convert.
/// @return The converted Error.
Error to_error(GenericError t_error);

/// @brief Wraps a GenericError for return from Expected{T, Error}.
/// @param t_error The generic error code.
/// @return An unexpected _Error_ in the generic category.
inline Unexpected<Error> make_error(GenericError t_error)
{
    return Unexpected<Error>(to_error(t_error));
}

/// @brief Compares an Error with a GenericError.
/// @param t_error The Error to compare.
/// @param t_error_value The GenericError to compare.
/// @return True if the Error is equal to the GenericError, false otherwise.
bool operator==(const Error& t_error, GenericError t_error_value);

/// @brief Compares a GenericError with an Error.
/// @param t_error_value The GenericError to compare.
/// @param t_error The Error to compare.
/// @return True if the GenericError is equal to the Error, false otherwise.
bool operator==(GenericError t_error_value, const Error& t_error);

#endif