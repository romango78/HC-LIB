// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DeviceErrors.h"

class DeviceErrorCategory : public ErrorCategory
{
    public:
    StringPtr message(uint8_t t_error) const override {
        switch(static_cast<DeviceError>(t_error)) {
            case DeviceError::TimerIsNotInitialized:
                return DEVICE_ERROR_TIMER_IS_NOT_INITIALIZED;
            case DeviceError::SensorIsNotInitialized:
                return DEVICE_ERROR_SENSOR_IS_NOT_INITIALIZED;
            default:
                return DEVICE_ERROR_UNKNOWN_ERROR;
        }
    }
};

/// @brief The instance of the DeviceErrorCategory.
static const DeviceErrorCategory deviceErrorCategoryInstance;

/// @brief Converts a DeviceError to an Error.
/// @param t_error The DeviceError to convert.
/// @return The converted Error.
Error to_error(DeviceError t_error) {
    return Error{ static_cast<uint8_t>(t_error), &deviceErrorCategoryInstance };
}

/// @brief Compares an Error with a DeviceError.
/// @param t_error The Error to compare.
/// @param t_error_value The DeviceError to compare.
/// @return True if the Error is equal to the DeviceError, false otherwise.
bool operator==(const Error& t_error, DeviceError t_error_value) {
    return (t_error.category == &deviceErrorCategoryInstance)
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}

/// @brief Compares a DeviceError with an Error.
/// @param t_error_value The DeviceError to compare.
/// @param t_error The Error to compare.
/// @return True if the DeviceError is equal to the Error, false otherwise.
bool operator==(DeviceError t_error_value, const Error& t_error) {
    return (t_error.category == &deviceErrorCategoryInstance)
        && (t_error.error == static_cast<uint8_t>(t_error_value));
}
