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
    TEST_ASSERT_FALSE(sut.canRead());
    TEST_ASSERT_FALSE(sut.canWrite());
    TEST_ASSERT_FALSE(sut.hasError());
    TEST_ASSERT_TRUE(sut.getLastError() == GenericError::NoError);
}

void BaseStream_Begin_ShouldAllowRead_WhenOpenedForRead()
{
    TestStream sut;
    sut.begin(StreamMode::Read);
    TEST_ASSERT_TRUE(sut.canRead());
    TEST_ASSERT_FALSE(sut.canWrite());
    TEST_ASSERT_FALSE(sut.hasError());
}

void BaseStream_Begin_ShouldAllowWrite_WhenOpenedForWrite()
{
    TestStream sut;
    sut.begin(StreamMode::Write);
    TEST_ASSERT_TRUE(sut.canWrite());
    TEST_ASSERT_FALSE(sut.canRead());
    TEST_ASSERT_FALSE(sut.hasError());
}

void BaseStream_End_ShouldCloseStream()
{
    TestStream sut;
    sut.begin(StreamMode::Read);
    sut.end();
    TEST_ASSERT_FALSE(sut.canRead());
    TEST_ASSERT_FALSE(sut.canWrite());
}

void BaseStream_SetLastError_ShouldStoreIoError()
{
    TestStream sut;
    sut.setLastError(to_error(IoError::StreamClosed));
    TEST_ASSERT_TRUE(sut.hasError());
    TEST_ASSERT_TRUE(sut.getLastError() == IoError::StreamClosed);
}

#endif
#endif
