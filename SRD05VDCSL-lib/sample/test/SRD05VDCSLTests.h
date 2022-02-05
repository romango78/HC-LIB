// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SRD05VDCSL_TESTS_H_
#define _SRD05VDCSL_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "move.h"
#include "FakeStream.h"
#include "devices/SRD05VDCSL.h"

SRD05VDCSLDevice createSRD05VDCSL()
{
    IStream<uint8_t>* stream = new FakeStream();
    SRD05VDCSLDevice result{5, stream};
    return result;
}

void ShouldConvert_SRD05VDCSLToSRD05VDCSL()
{
    // Arrange
    SRD05VDCSLDevice source = createSRD05VDCSL();

    // Act
    SRD05VDCSLDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.stream, sut.stream, "The 'stream' should be same as in origin object.");
}


void ShouldConvert_SRD05VDCSLToRelayDevice()
{
    // Arrange
    SRD05VDCSLDevice source = createSRD05VDCSL();

    // Act
    RelayDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.stream, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldConvert_SRD05VDCSLToDigitalDevice()
{
    // Arrange
    SRD05VDCSLDevice source = createSRD05VDCSL();

    // Act
    DigitalDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.stream, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldConvert_SRD05VDCSLToIDevice()
{
    // Arrange
    SRD05VDCSLDevice source = createSRD05VDCSL();

    // Act
    IDevice sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_SRD05VDCSLToSRD05VDCSL()
{
    // Arrange
    SRD05VDCSLDevice source = createSRD05VDCSL();

    // Act
    SRD05VDCSLDevice sut(std::move(source));

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.stream, sut.stream, "The 'stream' should be same as in origin object.");
}

#endif
#endif