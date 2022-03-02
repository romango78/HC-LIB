// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DIGITAL_DEVICE_CONTROLLER_TESTS_H_
#define _DIGITAL_DEVICE_CONTROLLER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakeStream.h"
#include "devices/controllers/DigitalDeviceController.h"

void Should_SetState_WhenDeviceIsInitialized()
{
    // Arrange
    IStream<uint8_t>* stream = new FakeStream();
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, stream};

    DigitalDeviceController sut {};

    // Act
    err_t result = sut.setState(device, 1);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, result, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(1, ((FakeStream*)stream)->getWrittenValue(), "The wrong value was written to port.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_IfSetState_WhenStreamIsNotSet()
{
    // Arrange
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, nullptr};

    DigitalDeviceController sut {};
    
    // Act
    err_t result = sut.setState(device, 1);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result, "The STREAM_NOTCREATED_IO_ERROR error is expected.");
}

void Should_GetState_WhenDeviceIsInitialized()
{
    // Arrange
    uint8_t expectedValue = 1;
    IStream<uint8_t>* stream = new FakeStream(expectedValue);
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, stream};

    DigitalDeviceController sut {};

    // Act
    auto result = sut.getState(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(true, result.hasValue(), "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(expectedValue, result.getValue(), "The Port state is expected.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_IfGetState_WhenStreamIsNotSet()
{
    // Arrange
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, nullptr};

    DigitalDeviceController sut {};

    // Act
    auto result = sut.getState(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), "The error is expected.");
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result.getError(), "The STREAM_NOTCREATED_IO_ERROR error is expected.");       
}

#endif
#endif
