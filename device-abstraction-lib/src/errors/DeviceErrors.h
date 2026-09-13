// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file DeviceErrors.h
/// @brief Defines the DeviceError enum and the to_error function.
/// @note Compare categories by identity. Use one static instance per category.
#ifndef _HC_LIB_DEVICE_ERRORS_H_
#define _HC_LIB_DEVICE_ERRORS_H_

#include "errors/GenericErrors.h"

#define DEVICE_ERROR_TIMER_IS_NOT_INITIALIZED    ERROR_STR("Timer is not initialized")
#define DEVICE_ERROR_SENSOR_IS_NOT_INITIALIZED   ERROR_STR("Sensor is not initialized")
#define DEVICE_ERROR_UNKNOWN_ERROR               ERROR_STR("Unknown Device Error")

/// @brief Device-specific error codes. Use GenericError for shared codes.
enum class DeviceError : uint8_t
{
    TimerIsNotInitialized = 1,
    SensorIsNotInitialized
};

/// @brief Converts a DeviceError to an Error.
/// @param t_error The DeviceError to convert.
/// @return The converted Error in the device category.
Error to_error(DeviceError t_error);

/// @brief Wraps a DeviceError for return from Expected{T, Error}.
/// @param t_error The device error code.
/// @return An unexpected _Error_ in the device category.
inline Unexpected<Error> make_error(DeviceError t_error)
{
    return Unexpected<Error>(to_error(t_error));
}

/// @brief Compares an Error with a DeviceError.
/// @param t_error The Error to compare.
/// @param t_error_value The DeviceError to compare.
/// @return True if the Error is equal to the DeviceError, false otherwise.
bool operator==(const Error& t_error, DeviceError t_error_value);

/// @brief Compares a DeviceError with an Error.
/// @param t_error_value The DeviceError to compare.
/// @param t_error The Error to compare.
/// @return True if the DeviceError is equal to the Error, false otherwise.
bool operator==(DeviceError t_error_value, const Error& t_error);

#endif
