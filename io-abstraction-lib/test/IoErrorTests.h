// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_IO_ERROR_TESTS_H_
#define _HC_LIB_IO_ERROR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "errors/IoErrors.h"

void IoError_ToError_ShouldMatchStreamNotCreated()
{
    Error sut = to_error(IoError::StreamNotCreated);
    TEST_ASSERT_TRUE(sut);
    TEST_ASSERT_TRUE(sut == IoError::StreamNotCreated);
    TEST_ASSERT_TRUE(IoError::StreamNotCreated == sut);
    TEST_ASSERT_FALSE(sut == IoError::StreamClosed);
}

void IoError_ToError_ShouldMatchStreamClosed()
{
    Error sut = to_error(IoError::StreamClosed);
    TEST_ASSERT_TRUE(sut);
    TEST_ASSERT_TRUE(sut == IoError::StreamClosed);
}

void IoError_MakeError_ShouldWrapIoError()
{
    Expected<uint8_t, Error> sut = make_error(IoError::StreamClosed);
    TEST_ASSERT_FALSE(sut.hasValue());
    TEST_ASSERT_TRUE(sut.getError() == IoError::StreamClosed);
}

#endif
#endif
