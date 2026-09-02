// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IoErrors.h
/// @brief Defines the IoError enum and the to_error function.
/// @note Compare categories by identity. Use one static instance per category.
#ifndef _HC_LIB_IO_ERRORS_H_
#define _HC_LIB_IO_ERRORS_H_

#include "errors/GenericErrors.h"

#define IO_ERROR_STREAM_NOT_CREATED ERROR_STR("Stream is not created")
#define IO_ERROR_STREAM_CLOSED      ERROR_STR("Stream is closed")
#define IO_ERROR_UNKNOWN_ERROR      ERROR_STR("Unknown IO error")

/// @brief IO-specific error codes. Use GenericError for shared codes.
enum class IoError : uint8_t
{
    StreamNotCreated = 1,
    StreamClosed
};

/// @brief Converts an IoError to an Error.
/// @param t_error The IoError to convert.
/// @return The converted Error in the IO category.
Error to_error(IoError t_error);

/// @brief Wraps an IoError for return from Expected{T, Error}.
/// @param t_error The IO error code.
/// @return An unexpected _Error_ in the IO category.
inline Unexpected<Error> make_error(IoError t_error)
{
    return Unexpected<Error>(to_error(t_error));
}

/// @brief Compares an Error with an IoError.
/// @param t_error The Error to compare.
/// @param t_error_value The IoError to compare.
/// @return True if the Error is equal to the IoError, false otherwise.
bool operator==(const Error& t_error, IoError t_error_value);

/// @brief Compares an IoError with an Error.
/// @param t_error_value The IoError to compare.
/// @param t_error The Error to compare.
/// @return True if the IoError is equal to the Error, false otherwise.
bool operator==(IoError t_error_value, const Error& t_error);

#endif
