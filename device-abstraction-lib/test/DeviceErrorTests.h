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
#include "errors/DeviceErrors.h"

void DeviceError_ToError_ShouldMatchTimerCode()
{
    Error sut = to_error(DeviceError::TimerIsNotInitialized);
    TEST_ASSERT_TRUE(sut);
    TEST_ASSERT_TRUE(sut == DeviceError::TimerIsNotInitialized);
    TEST_ASSERT_TRUE(DeviceError::TimerIsNotInitialized == sut);
    TEST_ASSERT_FALSE(sut == DeviceError::SensorIsNotInitialized);
}

void DeviceError_ToError_ShouldMatchSensorCode()
{
    Error sut = to_error(DeviceError::SensorIsNotInitialized);
    TEST_ASSERT_TRUE(sut);
    TEST_ASSERT_TRUE(sut == DeviceError::SensorIsNotInitialized);
}

void DeviceError_MakeError_ShouldWrapDeviceError()
{
    Expected<uint8_t, Error> sut = make_error(DeviceError::TimerIsNotInitialized);
    TEST_ASSERT_FALSE(sut.hasValue());
    TEST_ASSERT_TRUE(sut.getError() == DeviceError::TimerIsNotInitialized);
}

#endif
#endif
