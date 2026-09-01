// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "GenericErrors.h"

class GenericCategory : public ErrorCategory
{
    public:
    StringPtr message(uint8_t t_error) const override {
        switch(static_cast<GenericError>(t_error)) {
            case GenericError::NoError:           
                return GENERIC_ERROR_NO_ERROR;
            case GenericError::ArgumentIsNull:
                return GENERIC_ERROR_ARGUMENT_IS_NULL;
            case GenericError::OutOfRange:
                return GENERIC_ERROR_OUT_OF_RANGE;
            case GenericError::OutOfMemory:
                return GENERIC_ERROR_OUT_OF_MEMORY;
            case GenericError::NotSupportedOperation:
                return GENERIC_ERROR_NOT_SUPPORTED_OPERATION;
            case GenericError::InvalidOperation:
                return GENERIC_ERROR_INVALID_OPERATION;
            default:
                return GENERIC_ERROR_UNKNOWN_ERROR;
        }        
    }
};

/// @brief The instance of the GenericCategory.
static const GenericCategory genericCategoryInstance;

/// @brief Converts a GenericError to an Error.
/// @param t_error The GenericError to convert.
/// @return The converted Error.
Error to_error(GenericError t_error) {
    return Error{ static_cast<uint8_t>(t_error), &genericCategoryInstance };
}

/// @brief Compares an Error with a GenericError.
/// @param t_error The Error to compare.
/// @param t_error_value The GenericError to compare.
/// @return True if the Error is equal to the GenericError, false otherwise.
bool operator==(const Error& t_error, GenericError t_error_value) {
    return (t_error.category == &genericCategoryInstance) 
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}

/// @brief Compares a GenericError with an Error.
/// @param t_error_value The GenericError to compare.
/// @param t_error The Error to compare.
/// @return True if the GenericError is equal to the Error, false otherwise.
bool operator==(GenericError t_error_value, const Error& t_error) {
    return (t_error.category == &genericCategoryInstance) 
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}
