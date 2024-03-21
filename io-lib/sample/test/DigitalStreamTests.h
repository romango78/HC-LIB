// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
#ifndef _DIGITAL_STREAM_TESTS_H_
#define _DIGITAL_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakePortAdapter.h"
#include "stream/DigitalStream.h"

void DigitalStream_ShouldReadData_WhenStreamIsOpenForRead()
{
    // Arrange
    uint8_t expectedValue = 125;
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();
    ((FakePortAdapter *)adapter)->setData(expectedValue);

    IStream<uint8_t>* sut = new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    uint8_t actualValue = sut->read();
    sut->end();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, actualValue,"The read value is not equal the expected value.");

    delete sut;
}

void DigitalStream_ShouldBeInReadMode_WhenStreamIsOpenForRead()
{
    // Arrange
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();    

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_EQUAL_MESSAGE(true, canRead,"The stream is not set in read mode.");
    TEST_ASSERT_EQUAL_MESSAGE(false, canWrite,"The stream is not set in read mode.");

    delete sut;
}

void DigitalStream_ShouldBeInUndefinedMode_WhenStreamIsClosed()
{
    // Arrange
    bool expectedValue = false;
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();    

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    sut->end();
    bool actualValue = sut->canRead() | sut->canWrite();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(expectedValue, actualValue,"The closed stream is set in incorrect mode.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead()
{
    // Arrange
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();
    ((FakePortAdapter *)adapter)->setData(125);

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act    
    uint8_t actualValue = sut->read();
    int error = sut->getLastError();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(NO_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is set to some mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(NO_DATA, actualValue,"Some data is read.");
    TEST_ASSERT_EQUAL_MESSAGE(true, sut->hasError(),"Some error is expected.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(STREAM_CLOSED_IO_ERROR, error,"The wrong error is set.");

    delete sut;
}

void DigitalStream_ShouldWriteData_WhenStreamIsOpenForWrite()
{
    // Arrange
    uint8_t expectedValue = 125;
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();    

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Write);
    sut->write(expectedValue);
    sut->end();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(expectedValue, ((FakePortAdapter *)adapter)->getData(),"The write value is not equal the expected value.");

    delete sut;
}

void DigitalStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite()
{
    // Arrange    
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();    

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Write);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(OUTPUT_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is not set in OUTPUT mode.");
    TEST_ASSERT_EQUAL_MESSAGE(true, canWrite,"The stream is not set in write mode.");
    TEST_ASSERT_EQUAL_MESSAGE(false, canRead,"The stream is not set in write mode.");

    delete sut;
}

void DigitalStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes()
{
    // Arrange    
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();    

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    sut->end();
    sut->begin(StreamMode::Write);
    sut->end();
    sut->begin(StreamMode::Read);
    bool canRead = sut->canRead();
    bool canWrite = sut->canWrite();
    sut->end();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(INPUT_MODE, ((FakePortAdapter *)adapter)->getMode(), "The port is not set in INPUT mode.");
    TEST_ASSERT_EQUAL_MESSAGE(false, canWrite,"The stream is not set in specific mode.");
    TEST_ASSERT_EQUAL_MESSAGE(true, canRead,"The stream is not set in specific mode.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite()
{
    // Arrange
    IPortAdapter<uint8_t>* adapter = (IPortAdapter<uint8_t> *)new FakePortAdapter();

    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    sut->write(125);
    int error = sut->getLastError();    

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut->hasError(),"Some error is expected.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(STREAM_CLOSED_IO_ERROR, error,"The wrong error is set.");

    delete sut;
}

void DigitalStream_ShouldRaiseError_WhenAdaptorIsNotSet()
{
    // Arrange
    IStream<uint8_t>* sut = (IStream<uint8_t> *)new DigitalStream(nullptr);

    // Act
    sut->begin(StreamMode::Read);
    sut->write(125);
    int error = sut->getLastError();

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut->hasError(),"Some error is expected.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(STREAM_CLOSED_IO_ERROR, error,"The wrong error is set.");

    delete sut;
}

#endif
#endif