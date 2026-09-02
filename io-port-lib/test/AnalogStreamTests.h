// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ANALOG_STREAM_TESTS_H_
#define _HC_LIB_ANALOG_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakePortAdapter.h"
#include "stream/AnalogStream.h"

void AnalogStream_ShouldReadData_WhenStreamIsOpenForRead()
{
    int expectedValue = 125;
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    adapter->setData(expectedValue);
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Read);
    uint16_t actualValue = sut->read();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, adapter->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, actualValue, "The read value is not equal the expected value.");

    delete sut;
}

void AnalogStream_ShouldBeInReadMode_WhenStreamIsOpenForRead()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Read);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, adapter->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_TRUE_MESSAGE(canRead, "The stream is not set in read mode.");
    TEST_ASSERT_FALSE_MESSAGE(canWrite, "The stream is not set in read mode.");

    delete sut;
}

void AnalogStream_ShouldBeInUndefinedMode_WhenStreamIsClosed()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Read);
    sut->end();
    bool actualValue = sut->canRead() | sut->canWrite();

    TEST_ASSERT_FALSE_MESSAGE(actualValue, "The closed stream is set in incorrect mode.");

    delete sut;
}

void AnalogStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    adapter->setData(125);
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    uint16_t actualValue = sut->read();

    TEST_ASSERT_EQUAL_MESSAGE(NO_MODE, adapter->getMode(), "The port is set to some mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(NO_DATA, actualValue, "Some data is read.");
    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

void AnalogStream_ShouldWriteData_WhenStreamIsOpenForWrite()
{
    uint16_t expectedValue = 125;
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Write);
    sut->write(expectedValue);
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, adapter->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, adapter->getData(), "The write value is not equal the expected value.");

    delete sut;
}

void AnalogStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Write);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, adapter->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_TRUE_MESSAGE(canWrite, "The stream is not set in write mode.");
    TEST_ASSERT_FALSE_MESSAGE(canRead, "The stream is not set in write mode.");

    delete sut;
}

void AnalogStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Read);
    sut->end();
    sut->begin(StreamMode::Write);
    sut->end();
    sut->begin(StreamMode::Read);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, adapter->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_FALSE_MESSAGE(canWrite, "The stream is not set in specific mode.");
    TEST_ASSERT_TRUE_MESSAGE(canRead, "The stream is not set in specific mode.");

    delete sut;
}

void AnalogStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IStream<uint16_t>* sut = new AnalogStream(adapter);

    sut->begin(StreamMode::Read);
    sut->write(125);

    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

void AnalogStream_ShouldRaiseError_WhenAdaptorIsNotSet()
{
    IStream<uint16_t>* sut = new AnalogStream(nullptr);

    sut->begin(StreamMode::Read);
    sut->write(125);

    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

#endif
#endif
