// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_DIGITAL_DEVICE_CONTROLLER_TESTS_H_
#define _HC_LIB_DIGITAL_DEVICE_CONTROLLER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakeStream.h"
#include "devices/controllers/DigitalDeviceController.h"

void Should_SetState_WhenDeviceIsInitialized()
{
    DigitalStream* stream = new FakeDigitalStream();
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, stream};
    DigitalDeviceController sut {};

    Error result = sut.setState(device, 1);

    TEST_ASSERT_TRUE_MESSAGE(result == GenericError::NoError, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(1, ((FakeDigitalStream*)stream)->getWrittenValue(), "The wrong value was written to port.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_IfSetState_WhenStreamIsNotSet()
{
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, nullptr};
    DigitalDeviceController sut {};

    Error result = sut.setState(device, 1);

    TEST_ASSERT_TRUE_MESSAGE(result == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void Should_GetState_WhenDeviceIsInitialized()
{
    uint8_t expectedValue = 1;
    DigitalStream* stream = new FakeDigitalStream(expectedValue);
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, stream};
    DigitalDeviceController sut {};

    Expected<uint8_t, Error> result = sut.getState(device);

    TEST_ASSERT_TRUE_MESSAGE(result.hasValue(), "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(expectedValue, result.getValue(), "The Port state is expected.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_IfGetState_WhenStreamIsNotSet()
{
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, nullptr};
    DigitalDeviceController sut {};

    Expected<uint8_t, Error> result = sut.getState(device);

    TEST_ASSERT_FALSE_MESSAGE(result.hasValue(), "The error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(result.getError() == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void Should_SetState_WhenStreamAlreadyWritable()
{
    DigitalStream* stream = new FakeDigitalStream();
    DigitalDevice device {RELAY_DEVICE_TYPE, 1, stream};
    DigitalDeviceController sut {};

    Error first = sut.setState(device, 1);
    Error second = sut.setState(device, 0);

    TEST_ASSERT_TRUE_MESSAGE(first == GenericError::NoError, "No errors expected on first write.");
    TEST_ASSERT_TRUE_MESSAGE(second == GenericError::NoError, "No errors expected when the stream is already writable.");
    TEST_ASSERT_EQUAL_MESSAGE(0, ((FakeDigitalStream*)stream)->getWrittenValue(), "The last written value is expected.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

#endif
#endif
