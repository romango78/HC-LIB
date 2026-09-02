// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
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
#include "devices/AnalogDevices.h"
#include "devices/DigitalDevices.h"
#include "FakeStream.h"
#include "lib-utility.h"

AnalogDevice createAnalogDevice()
{
    AnalogDevice result{10, 5, (AnalogStream*)new FakeAnalogStream()};
    return result;
}

AnalogDevice createAnalogDevice2()
{
    AnalogDevice result{12, 10, (AnalogStream*)new FakeAnalogStream()};
    return result;
}

DigitalDevice createDigitalDevice()
{
    DigitalDevice result{10, 5, new FakeDigitalStream()};
    return result;
}

DigitalDevice createDigitalDevice2()
{
    DigitalDevice result{12, 10, new FakeDigitalStream()};
    return result;
}

RelayDevice createRelayDevice()
{
    RelayDevice result{5, new FakeDigitalStream()};
    return result;
}

RelayDevice createRelayDevice2()
{
    RelayDevice result{15, new FakeDigitalStream()};
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

void ShouldClassify_AnalogDeviceAsAnalog()
{
    AnalogDevice sut = createAnalogDevice();

    TEST_ASSERT_TRUE_MESSAGE(device::is_analog(sut), "AnalogDevice should be analog.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_digital(sut), "AnalogDevice should not be digital.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_relay(sut), "AnalogDevice should not be a relay.");
}

void ShouldClassify_DigitalDeviceAsDigital()
{
    DigitalDevice sut = createDigitalDevice();

    TEST_ASSERT_TRUE_MESSAGE(device::is_digital(sut), "DigitalDevice should be digital.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_analog(sut), "DigitalDevice should not be analog.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_relay(sut), "A generic digital device should not be a relay.");
}

void ShouldClassify_RelayDeviceAsRelay()
{
    RelayDevice sut = createRelayDevice();

    TEST_ASSERT_TRUE_MESSAGE(device::is_digital(sut), "RelayDevice should be digital.");
    TEST_ASSERT_TRUE_MESSAGE(device::is_relay(sut), "RelayDevice should be a relay.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_analog(sut), "RelayDevice should not be analog.");
    TEST_ASSERT_EQUAL_MESSAGE(RELAY_DEVICE_TYPE, sut.type, "RelayDevice type should be RELAY_DEVICE_TYPE.");
}

void ShouldConstruct_AnalogDevice_WithNullStream()
{
    AnalogDevice sut{10, 5, nullptr};

    TEST_ASSERT_EQUAL(10, sut.type);
    TEST_ASSERT_TRUE(sut.category == DeviceCategory::Analog);
    TEST_ASSERT_EQUAL(5, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldConstruct_DigitalDevice_WithNullStream()
{
    DigitalDevice sut{10, 5, nullptr};

    TEST_ASSERT_EQUAL(10, sut.type);
    TEST_ASSERT_TRUE(sut.category == DeviceCategory::Digital);
    TEST_ASSERT_EQUAL(5, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldConstruct_RelayDevice_WithNullStream()
{
    RelayDevice sut{7, nullptr};

    TEST_ASSERT_EQUAL(RELAY_DEVICE_TYPE, sut.type);
    TEST_ASSERT_TRUE(sut.category == DeviceCategory::Digital);
    TEST_ASSERT_EQUAL(7, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldCopy_AnalogDevice_WithNullStream()
{
    AnalogDevice source{10, 5, nullptr};

    AnalogDevice sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldCopy_DigitalDevice_WithNullStream()
{
    DigitalDevice source{10, 5, nullptr};

    DigitalDevice sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldCopy_AnalogDevice_FromMovedSource()
{
    AnalogDevice source = createAnalogDevice();
    AnalogDevice owner(std::move(source));

    AnalogDevice sut(source);

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Copy of a moved-from device should not clone the stream.");
    TEST_ASSERT_NOT_NULL(owner.stream);
}

void ShouldCopy_DigitalDevice_FromMovedSource()
{
    DigitalDevice source = createDigitalDevice();
    DigitalDevice owner(std::move(source));

    DigitalDevice sut(source);

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Copy of a moved-from device should not clone the stream.");
    TEST_ASSERT_NOT_NULL(owner.stream);
}

void ShouldCopy_RelayDevice_FromMovedSource()
{
    RelayDevice source = createRelayDevice();
    RelayDevice owner(std::move(source));

    RelayDevice sut(source);

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Copy of a moved-from device should not clone the stream.");
    TEST_ASSERT_NOT_NULL(owner.stream);
}

void ShouldAssign_AnalogDevice_FromNullStream()
{
    AnalogDevice source{10, 5, nullptr};
    AnalogDevice sut = createAnalogDevice();

    sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Assignment from a null-stream device should clear the stream.");
}

void ShouldAssign_DigitalDevice_FromNullStream()
{
    DigitalDevice source{10, 5, nullptr};
    DigitalDevice sut = createDigitalDevice();

    sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Assignment from a null-stream device should clear the stream.");
}

void ShouldSelfAssign_AnalogDevice()
{
    AnalogDevice sut = createAnalogDevice();
    AnalogStream* streamBefore = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL_MESSAGE(streamBefore, sut.stream, "Self-assignment must leave the stream pointer unchanged.");
    TEST_ASSERT_NOT_NULL(sut.stream);
}

void ShouldSelfAssign_DigitalDevice()
{
    DigitalDevice sut = createDigitalDevice();
    DigitalStream* streamBefore = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL_MESSAGE(streamBefore, sut.stream, "Self-assignment must leave the stream pointer unchanged.");
    TEST_ASSERT_NOT_NULL(sut.stream);
}

void ShouldSelfAssign_RelayDevice()
{
    RelayDevice sut = createRelayDevice();
    DigitalStream* streamBefore = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL_MESSAGE(streamBefore, sut.stream, "Self-assignment must leave the stream pointer unchanged.");
    TEST_ASSERT_NOT_NULL(sut.stream);
}

#endif
#endif