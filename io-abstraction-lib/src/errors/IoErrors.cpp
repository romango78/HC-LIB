// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "IoErrors.h"

class IoErrorCategory : public ErrorCategory
{
    public:
    StringPtr message(uint8_t t_error) const override {
        switch(static_cast<IoError>(t_error)) {
            case IoError::StreamNotCreated:
                return IO_ERROR_STREAM_NOT_CREATED;
            case IoError::StreamClosed:
                return IO_ERROR_STREAM_CLOSED;
            default:
                return IO_ERROR_UNKNOWN_ERROR;
        }
    }
};

/// @brief The instance of the IoErrorCategory.
static const IoErrorCategory ioErrorCategoryInstance;

/// @brief Converts an IoError to an Error.
/// @param t_error The IoError to convert.
/// @return The converted Error.
Error to_error(IoError t_error) {
    return Error{ static_cast<uint8_t>(t_error), &ioErrorCategoryInstance };
}

/// @brief Compares an Error with an IoError.
/// @param t_error The Error to compare.
/// @param t_error_value The IoError to compare.
/// @return True if the Error is equal to the IoError, false otherwise.
bool operator==(const Error& t_error, IoError t_error_value) {
    return (t_error.category == &ioErrorCategoryInstance)
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}

/// @brief Compares an IoError with an Error.
/// @param t_error_value The IoError to compare.
/// @param t_error The Error to compare.
/// @return True if the IoError is equal to the Error, false otherwise.
bool operator==(IoError t_error_value, const Error& t_error) {
    return (t_error.category == &ioErrorCategoryInstance)
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}
