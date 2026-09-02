// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_DIGITAL_STREAM_TESTS_H_
#define _HC_LIB_DIGITAL_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakePortAdapter.h"
#include "stream/DigitalStream.h"

void DigitalStream_ShouldReadData_WhenStreamIsOpenForRead()
{
    uint8_t expectedValue = 125;
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    adapter->setData(expectedValue);
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Read);
    uint8_t actualValue = sut->read();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, adapter->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, actualValue, "The read value is not equal the expected value.");

    delete sut;
}

void DigitalStream_ShouldBeInReadMode_WhenStreamIsOpenForRead()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Read);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, adapter->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_TRUE_MESSAGE(canRead, "The stream is not set in read mode.");
    TEST_ASSERT_FALSE_MESSAGE(canWrite, "The stream is not set in read mode.");

    delete sut;
}

void DigitalStream_ShouldBeInUndefinedMode_WhenStreamIsClosed()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Read);
    sut->end();
    bool actualValue = sut->canRead() | sut->canWrite();

    TEST_ASSERT_FALSE_MESSAGE(actualValue, "The closed stream is set in incorrect mode.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    adapter->setData(125);
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    uint8_t actualValue = sut->read();

    TEST_ASSERT_EQUAL_MESSAGE(NO_MODE, adapter->getMode(), "The port is set to some mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(NO_DATA, actualValue, "Some data is read.");
    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

void DigitalStream_ShouldWriteData_WhenStreamIsOpenForWrite()
{
    uint8_t expectedValue = 125;
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Write);
    sut->write(expectedValue);
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, adapter->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, adapter->getData(), "The write value is not equal the expected value.");

    delete sut;
}

void DigitalStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Write);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, adapter->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_TRUE_MESSAGE(canWrite, "The stream is not set in write mode.");
    TEST_ASSERT_FALSE_MESSAGE(canRead, "The stream is not set in write mode.");

    delete sut;
}

void DigitalStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

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

void DigitalStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    IStream<uint8_t>* sut = new DigitalStream(adapter);

    sut->begin(StreamMode::Read);
    sut->write(125);

    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenAdaptorIsNotSet()
{
    IStream<uint8_t>* sut = new DigitalStream(nullptr);

    sut->begin(StreamMode::Read);
    sut->write(125);

    TEST_ASSERT_TRUE_MESSAGE(sut->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut->getLastError() == IoError::StreamClosed, "The wrong error is set.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenBegin_AndAdaptorIsNotSet()
{
    DigitalStream sut(nullptr);

    sut.begin(StreamMode::Read);

    TEST_ASSERT_TRUE_MESSAGE(sut.hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getLastError() == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
    TEST_ASSERT_FALSE_MESSAGE(sut.canRead(), "The stream should stay closed when the adapter is missing.");
}

void DigitalStream_ShouldGetState_WhenAdaptorIsSet()
{
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    adapter->setState(1);
    DigitalStream sut(adapter);

    uint8_t actualValue = sut.getState();

    TEST_ASSERT_EQUAL_MESSAGE(1, actualValue, "The adapter state is expected.");
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), "No errors expected.");
}

void DigitalStream_ShouldRaiseError_WhenGetState_AndAdaptorIsNotSet()
{
    DigitalStream sut(nullptr);

    uint8_t actualValue = sut.getState();

    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, actualValue, "NO_DATA is expected when the adapter is missing.");
    TEST_ASSERT_TRUE_MESSAGE(sut.hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getLastError() == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");
}

void DigitalStream_ShouldClone_WhenAdaptorIsSet()
{
    uint8_t expectedValue = 1;
    FakePortAdapter<uint8_t>* adapter = new FakePortAdapter<uint8_t>();
    adapter->setData(expectedValue);
    DigitalStream sut(adapter);
    sut.begin(StreamMode::Read);

    IStream<uint8_t>* clone = sut.clone();

    TEST_ASSERT_NOT_NULL_MESSAGE(clone, "A clone is expected.");
    TEST_ASSERT_TRUE_MESSAGE(clone->canRead(), "The clone should keep the source mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, clone->read(), "The clone should read the cloned adapter data.");
    TEST_ASSERT_FALSE_MESSAGE(clone->hasError(), "No errors expected on the clone.");

    delete clone;
}

void DigitalStream_ShouldClone_WhenAdaptorIsNotSet()
{
    DigitalStream sut(nullptr);

    IStream<uint8_t>* clone = sut.clone();

    TEST_ASSERT_NOT_NULL_MESSAGE(clone, "A clone is expected.");
    clone->begin(StreamMode::Read);
    TEST_ASSERT_TRUE_MESSAGE(clone->hasError(), "Some error is expected.");
    TEST_ASSERT_TRUE_MESSAGE(clone->getLastError() == IoError::StreamNotCreated, "IoError::StreamNotCreated is expected.");

    delete clone;
}

#endif
#endif
