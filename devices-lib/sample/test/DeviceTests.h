// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DEVICE_TESTS_H_
#define _DEVICE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "devices/AnalogDevices.h"
#include "devices/DigitalDevices.h"
#include "FakeStream.h"
#include "move.h"

AnalogDevice createAnalogDevice()
{
    AnalogDevice result{10, 5, (IStream<uint16_t>*)new FakeStream()};
    return result;
}

AnalogDevice createAnalogDevice2()
{
    AnalogDevice result{12, 10, (IStream<uint16_t>*)new FakeStream()};
    return result;
}

DigitalDevice createDigitalDevice()
{
    DigitalDevice result{10, 5, new FakeStream()};
    return result;
}

DigitalDevice createDigitalDevice2()
{
    DigitalDevice result{12, 10, new FakeStream()};
    return result;
}

RelayDevice createRelayDevice()
{
    RelayDevice result{5, new FakeStream()};
    return result;
}

RelayDevice createRelayDevice2()
{
    RelayDevice result{15, new FakeStream()};
    return result;
}

void ShouldConvert_AnalogDeviceToAnalogDevice()
{
    // Arrange
    AnalogDevice source = createAnalogDevice();

    // Act
    AnalogDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_AnalogDeviceToIDevice()
{
    // Arrange
    AnalogDevice source = createAnalogDevice();

    // Act
    IDevice sut = (IDevice)source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_AnalogDeviceToAnalogDevice()
{
    // Arrange
    AnalogDevice source = createAnalogDevice();
    auto sourceStreamAddr = reinterpret_cast<uintptr_t>(source.stream);

    // Act
    AnalogDevice sut(std::move(source));

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(sourceStreamAddr, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldAssignCopy_AnalogDevice()
{
    // Arrange
    AnalogDevice source = createAnalogDevice();
    AnalogDevice sut = createAnalogDevice2();

    // Act
    sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_DigitalDeviceToDigitalDevice()
{
    // Arrange
    DigitalDevice source = createDigitalDevice();

    // Act
    DigitalDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_DigitalDeviceToIDevice()
{
    // Arrange
    DigitalDevice source = createDigitalDevice();

    // Act
    IDevice sut = (IDevice)source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_DigitalDeviceToDigitalDevice()
{
    // Arrange
    DigitalDevice source = createDigitalDevice();
    auto sourceStreamAddr = reinterpret_cast<uintptr_t>(source.stream);

    // Act
    DigitalDevice sut(std::move(source));

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(sourceStreamAddr, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldAssignCopy_DigitalDevice()
{
    // Arrange
    DigitalDevice source = createDigitalDevice();
    DigitalDevice sut = createDigitalDevice2();

    // Act
    sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_RelayDeviceToRelayDevice()
{
    // Arrange
    RelayDevice source = createRelayDevice();

    // Act
    RelayDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_RelayDeviceToDigitalDevice()
{
    // Arrange
    RelayDevice source = createRelayDevice();

    // Act
    DigitalDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_RelayDeviceToIDevice()
{
    // Arrange
    RelayDevice source = createRelayDevice();

    // Act
    IDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_RelayDeviceToRelayDevice()
{
    // Arrange
    RelayDevice source = createRelayDevice();
    auto sourceStreamAddr = reinterpret_cast<uintptr_t>(source.stream);

    // Act
    RelayDevice sut(std::move(source));

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(sourceStreamAddr, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldAssignCopy_RelayDevice()
{
    // Arrange
    RelayDevice source = createRelayDevice();
    RelayDevice sut = createRelayDevice2();

    // Act
    sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The 'stream' should be cloned.");
}

#endif
#endif