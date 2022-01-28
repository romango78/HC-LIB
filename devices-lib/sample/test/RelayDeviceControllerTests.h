// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _RELAY_DEVICE_CONTROLLER_TESTS_H_
#define _RELAY_DEVICE_CONTROLLER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakeStream.h"
#include "devices/controllers/RelayDeviceController.h"

void ShouldSwitchRepayInOnStatus()
{
    // Arrange
    IStream<uint8_t>* stream = new FakeStream();
    RelayDevice device {1, stream};

    RelayDeviceController sut {};

    // Act
    err_t result = sut.on(device);

    // Assert

    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, result, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(RelayState::on, ((FakeStream*)stream)->getWrittenValue(), "The relay should be ON.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_WhenTryOnRelay_AndStreamIsNotSet()
{
    // Arrange
    RelayDevice device {1, nullptr};

    RelayDeviceController sut {};
    
    // Act
    err_t result = sut.on(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result, "The STREAM_NOTCREATED_IO_ERROR error is expected.");
}

void ShouldSwitchRepayInOffStatus()
{
    // Arrange
    IStream<uint8_t>* stream = new FakeStream();
    RelayDevice device {1, stream};

    RelayDeviceController sut {};

    // Act
    err_t result = sut.off(device);

    // Assert

    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, result, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(RelayState::off, ((FakeStream*)stream)->getWrittenValue(), "The relay should be OFF.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected in Stream.");
}

void Should_RaiseError_WhenTryOffRelay_AndStreamIsNotSet()
{
    // Arrange
    RelayDevice device {1, nullptr};

    RelayDeviceController sut {};
    
    // Act
    err_t result = sut.off(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result, "The STREAM_NOTCREATED_IO_ERROR error is expected.");
}

void ShouldGetRelayState()
{
    // Arrange
    IStream<uint8_t>* stream = new FakeStream(RelayState::off);
    RelayDevice device {1, stream};

    RelayDeviceController sut {};

    // Act
    Expected<RelayState> result = sut.getState(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(true, result.hasValue(), "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(RelayState::off, result.getValue(), "TThe relay should be OFF.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected in Stream.");    

}

void Should_RaiseError_WhenTryGetRelayState_AndStreamIsNotSet()
{
    // Arrange
    RelayDevice device {1, nullptr};

    RelayDeviceController sut {};

    // Act
    auto result = sut.getState(device);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), "The error is expected.");
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result.getError(), "The STREAM_NOTCREATED_IO_ERROR error is expected.");       
}

#endif
#endif