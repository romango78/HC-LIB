// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_RELAY_DEVICE_CONTROLLER_TESTS_H_
#define _HC_LIB_RELAY_DEVICE_CONTROLLER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakeStream.h"
#include "devices/controllers/RelayDeviceController.h"

void ShouldSwitchRepayInOnStatus()
{
    DigitalStream* stream = new FakeDigitalStream();
    RelayDevice device {1, stream};
    RelayDeviceController sut {};

    Error result = sut.on(device);

    TEST_ASSERT_TRUE_MESSAGE(result == GenericError::NoError, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(static_cast<uint8_t>(RelayState::On), ((FakeDigitalStream*)stream)->getWrittenValue(), "The relay should be ON.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_WhenTryOnRelay_AndStreamIsNotSet()
{
    RelayDevice device {1, nullptr};
    RelayDeviceController sut {};

    Error result = sut.on(device);

    TEST_ASSERT_TRUE_MESSAGE(result == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void ShouldSwitchRepayInOffStatus()
{
    DigitalStream* stream = new FakeDigitalStream();
    RelayDevice device {1, stream};
    RelayDeviceController sut {};

    Error result = sut.off(device);

    TEST_ASSERT_TRUE_MESSAGE(result == GenericError::NoError, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(static_cast<uint8_t>(RelayState::Off), ((FakeDigitalStream*)stream)->getWrittenValue(), "The relay should be OFF.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_WhenTryOffRelay_AndStreamIsNotSet()
{
    RelayDevice device {1, nullptr};
    RelayDeviceController sut {};

    Error result = sut.off(device);

    TEST_ASSERT_TRUE_MESSAGE(result == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void ShouldGetRelayState()
{
    DigitalStream* stream = new FakeDigitalStream(static_cast<uint8_t>(RelayState::Off));
    RelayDevice device {1, stream};
    RelayDeviceController sut {};

    Expected<RelayState, Error> result = sut.getState(device);

    TEST_ASSERT_TRUE_MESSAGE(result.hasValue(), "No errors expected.");
    TEST_ASSERT_TRUE_MESSAGE(result.getValue() == RelayState::Off, "The relay should be OFF.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_WhenTryGetRelayState_AndStreamIsNotSet()
{
    RelayDevice device {1, nullptr};
    RelayDeviceController sut {};

    Expected<RelayState, Error> result = sut.getState(device);

    TEST_ASSERT_FALSE_MESSAGE(result.hasValue(), "The error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(result.getError() == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void ShouldGetRelayState_WhenRelayIsOn()
{
    DigitalStream* stream = new FakeDigitalStream(static_cast<uint8_t>(RelayState::On));
    RelayDevice device {1, stream};
    RelayDeviceController sut {};

    Expected<RelayState, Error> result = sut.getState(device);

    TEST_ASSERT_TRUE_MESSAGE(result.hasValue(), "No errors expected.");
    TEST_ASSERT_TRUE_MESSAGE(result.getValue() == RelayState::On, "The relay should be ON.");
    TEST_ASSERT_FALSE_MESSAGE(stream->hasError(), "No errors expected in Stream.");
}

#endif
#endif
