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
#include "devices/Device.h"

void Device_IsDigital_ShouldBeTrue_WhenCategoryIsDigital()
{
    IDevice sut(UNDEFINED_DEVICE_TYPE, DeviceCategory::Digital);
    TEST_ASSERT_TRUE(device::is_digital(sut));
    TEST_ASSERT_FALSE(device::is_analog(sut));
}

void Device_IsAnalog_ShouldBeTrue_WhenCategoryIsAnalog()
{
    IDevice sut(UNDEFINED_DEVICE_TYPE, DeviceCategory::Analog);
    TEST_ASSERT_TRUE(device::is_analog(sut));
    TEST_ASSERT_FALSE(device::is_digital(sut));
}

void Device_IsRelay_ShouldBeTrue_WhenDigitalRelayType()
{
    IDevice sut(RELAY_DEVICE_TYPE, DeviceCategory::Digital);
    TEST_ASSERT_TRUE(device::is_relay(sut));
}

void Device_IsRelay_ShouldBeFalse_WhenAnalog()
{
    IDevice sut(RELAY_DEVICE_TYPE, DeviceCategory::Analog);
    TEST_ASSERT_FALSE(device::is_relay(sut));
}

#endif
#endif
