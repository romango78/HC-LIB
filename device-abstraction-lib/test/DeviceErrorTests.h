// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_DEVICE_ERROR_TESTS_H_
#define _HC_LIB_DEVICE_ERROR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "errors/DeviceErrors.h"

void DeviceError_ToError_ShouldMatchTimerCode()
{
    Error sut = to_error(DeviceError::TimerIsNotInitialized);
    TEST_ASSERT_TRUE_MESSAGE(sut, F("Timer error should be truthy."));
    TEST_ASSERT_TRUE_MESSAGE(sut == DeviceError::TimerIsNotInitialized, F("Error should match TimerIsNotInitialized."));
    TEST_ASSERT_TRUE_MESSAGE(DeviceError::TimerIsNotInitialized == sut, F("TimerIsNotInitialized should match Error."));
    TEST_ASSERT_FALSE_MESSAGE(sut == DeviceError::SensorIsNotInitialized, F("Timer error should not match SensorIsNotInitialized."));
}

void DeviceError_ToError_ShouldMatchSensorCode()
{
    Error sut = to_error(DeviceError::SensorIsNotInitialized);
    TEST_ASSERT_TRUE_MESSAGE(sut, F("Sensor error should be truthy."));
    TEST_ASSERT_TRUE_MESSAGE(sut == DeviceError::SensorIsNotInitialized, F("Error should match SensorIsNotInitialized."));
}

void DeviceError_MakeError_ShouldWrapDeviceError()
{
    Expected<uint8_t, Error> sut = make_error(DeviceError::TimerIsNotInitialized);
    TEST_ASSERT_FALSE_MESSAGE(sut.hasValue(), F("make_error should produce an error Expected."));
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == DeviceError::TimerIsNotInitialized, F("Expected should hold TimerIsNotInitialized."));
}

#endif
#endif
