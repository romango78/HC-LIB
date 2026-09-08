// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_BASE_STREAM_TESTS_H_
#define _HC_LIB_BASE_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "stream/BaseStream.h"

class TestStream : public BaseStream<uint8_t>
{
    public:
        using BaseStream<uint8_t>::setLastError;

        IStream<uint8_t>* clone() const override
        {
            return new TestStream();
        }
};

void BaseStream_Ctor_ShouldBeClosed_WithoutError()
{
    TestStream sut;
    TEST_ASSERT_FALSE_MESSAGE(sut.canRead(), F("Closed stream should not be readable."));
    TEST_ASSERT_FALSE_MESSAGE(sut.canWrite(), F("Closed stream should not be writable."));
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), F("New stream should have no error."));
    TEST_ASSERT_TRUE_MESSAGE(sut.getLastError() == GenericError::NoError, F("Last error should be NoError."));
}

void BaseStream_Begin_ShouldAllowRead_WhenOpenedForRead()
{
    TestStream sut;
    sut.begin(StreamMode::Read);
    TEST_ASSERT_TRUE_MESSAGE(sut.canRead(), F("Read mode should allow read."));
    TEST_ASSERT_FALSE_MESSAGE(sut.canWrite(), F("Read mode should not allow write."));
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), F("begin(Read) should clear the last error."));
}

void BaseStream_Begin_ShouldAllowWrite_WhenOpenedForWrite()
{
    TestStream sut;
    sut.begin(StreamMode::Write);
    TEST_ASSERT_TRUE_MESSAGE(sut.canWrite(), F("Write mode should allow write."));
    TEST_ASSERT_FALSE_MESSAGE(sut.canRead(), F("Write mode should not allow read."));
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), F("begin(Write) should clear the last error."));
}

void BaseStream_End_ShouldCloseStream()
{
    TestStream sut;
    sut.begin(StreamMode::Read);
    sut.end();
    TEST_ASSERT_FALSE_MESSAGE(sut.canRead(), F("end() should clear read."));
    TEST_ASSERT_FALSE_MESSAGE(sut.canWrite(), F("end() should clear write."));
}

void BaseStream_SetLastError_ShouldStoreIoError()
{
    TestStream sut;
    sut.setLastError(to_error(IoError::StreamClosed));
    TEST_ASSERT_TRUE_MESSAGE(sut.hasError(), F("setLastError should set hasError."));
    TEST_ASSERT_TRUE_MESSAGE(sut.getLastError() == IoError::StreamClosed, F("Last error should be StreamClosed."));
}

#endif
#endif
