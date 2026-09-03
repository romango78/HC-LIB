// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_SRD05VDCSL_TESTS_H_
#define _HC_LIB_SRD05VDCSL_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "lib-utility.h"
#include "FakeStream.h"
#include "devices/SRD05VDCSL.h"
#include "devices/controllers/RelayDeviceController.h"

SRD05VDCSLDevice createSRD05VDCSL()
{
    SRD05VDCSLDevice result{5, new FakeDigitalStream()};
    return result;
}

SRD05VDCSLDevice createSRD05VDCSL2()
{
    SRD05VDCSLDevice result{15, new FakeDigitalStream()};
    return result;
}

void ShouldConvert_SRD05VDCSLToSRD05VDCSL()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();

    SRD05VDCSLDevice sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The cloned stream should be a new instance.");
}

void ShouldConvert_SRD05VDCSLToRelayDevice()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();

    RelayDevice sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_SRD05VDCSLToDigitalDevice()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();

    DigitalDevice sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_SRD05VDCSLToIDevice()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();

    IDevice sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_SRD05VDCSLToSRD05VDCSL()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();
    auto sourceStreamAddr = reinterpret_cast<uintptr_t>(source.stream);

    SRD05VDCSLDevice sut(std::move(source));

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(sourceStreamAddr, reinterpret_cast<uintptr_t>(sut.stream), "The 'stream' should be same as in origin object.");
}

void ShouldAssignCopy_SRD05VDCSL()
{
    SRD05VDCSLDevice source = createSRD05VDCSL();
    SRD05VDCSLDevice sut = createSRD05VDCSL2();

    sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The 'stream' should be cloned.");
}

void ShouldSelfAssign_SRD05VDCSL()
{
    SRD05VDCSLDevice sut = createSRD05VDCSL();
    const uint8_t pin = sut.pin;
    DigitalStream* const stream = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL_MESSAGE(pin, sut.pin, "Self-assignment should keep the pin.");
    TEST_ASSERT_EQUAL_MESSAGE(stream, sut.stream, "Self-assignment should keep the stream.");
}

void ShouldConstruct_SRD05VDCSL_WithNullStream()
{
    SRD05VDCSLDevice sut(7, nullptr);

    TEST_ASSERT_EQUAL(RELAY_DEVICE_TYPE, sut.type);
    TEST_ASSERT_EQUAL(DeviceCategory::Digital, sut.category);
    TEST_ASSERT_EQUAL(7, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
    TEST_ASSERT_TRUE(device::is_relay(sut));
}

void ShouldClassify_SRD05VDCSLAsRelay()
{
    SRD05VDCSLDevice sut = createSRD05VDCSL();

    TEST_ASSERT_TRUE(device::is_digital(sut));
    TEST_ASSERT_TRUE(device::is_relay(sut));
    TEST_ASSERT_FALSE(device::is_analog(sut));
}

void ShouldSwitch_SRD05VDCSL_OnAndOff()
{
    SRD05VDCSLDevice sut = createSRD05VDCSL();
    RelayDeviceController controller;

    Error offError = controller.off(sut);
    TEST_ASSERT_FALSE(offError);
    Expected<RelayState, Error> offState = controller.getState(sut);
    TEST_ASSERT_TRUE(offState.hasValue());
    TEST_ASSERT_TRUE(offState.getValue() == RelayState::Off);

    Error onError = controller.on(sut);
    TEST_ASSERT_FALSE(onError);
    Expected<RelayState, Error> onState = controller.getState(sut);
    TEST_ASSERT_TRUE(onState.hasValue());
    TEST_ASSERT_TRUE(onState.getValue() == RelayState::On);
}

void Should_RaiseError_WhenStreamIsNull()
{
    SRD05VDCSLDevice sut(3, nullptr);
    RelayDeviceController controller;

    TEST_ASSERT_TRUE(controller.on(sut) == IoError::StreamNotCreated);
    TEST_ASSERT_TRUE(controller.off(sut) == IoError::StreamNotCreated);

    Expected<RelayState, Error> state = controller.getState(sut);
    TEST_ASSERT_FALSE(state.hasValue());
    TEST_ASSERT_TRUE(state.getError() == IoError::StreamNotCreated);
}

#endif
#endif
