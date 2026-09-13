// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_DEVICE_TESTS_H_
#define _HC_LIB_DEVICE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "devices/Device.h"

void Device_IsDigital_ShouldBeTrue_WhenCategoryIsDigital()
{
    IDevice sut(UNDEFINED_DEVICE_TYPE, DeviceCategory::Digital);
    TEST_ASSERT_TRUE_MESSAGE(device::is_digital(sut), F("Digital category should be digital."));
    TEST_ASSERT_FALSE_MESSAGE(device::is_analog(sut), F("Digital category should not be analog."));
}

void Device_IsAnalog_ShouldBeTrue_WhenCategoryIsAnalog()
{
    IDevice sut(UNDEFINED_DEVICE_TYPE, DeviceCategory::Analog);
    TEST_ASSERT_TRUE_MESSAGE(device::is_analog(sut), F("Analog category should be analog."));
    TEST_ASSERT_FALSE_MESSAGE(device::is_digital(sut), F("Analog category should not be digital."));
}

void Device_IsRelay_ShouldBeTrue_WhenDigitalRelayType()
{
    IDevice sut(RELAY_DEVICE_TYPE, DeviceCategory::Digital);
    TEST_ASSERT_TRUE_MESSAGE(device::is_relay(sut), F("Digital relay type should be a relay."));
}

void Device_IsRelay_ShouldBeFalse_WhenAnalog()
{
    IDevice sut(RELAY_DEVICE_TYPE, DeviceCategory::Analog);
    TEST_ASSERT_FALSE_MESSAGE(device::is_relay(sut), F("Analog device should not be a relay."));
}

#endif
#endif
