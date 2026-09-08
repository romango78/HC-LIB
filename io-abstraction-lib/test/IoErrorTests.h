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
#include "unity_extensions.h"
#include "errors/IoErrors.h"

void IoError_ToError_ShouldMatchStreamNotCreated()
{
    Error sut = to_error(IoError::StreamNotCreated);
    TEST_ASSERT_TRUE_MESSAGE(sut, F("StreamNotCreated error should be truthy."));
    TEST_ASSERT_TRUE_MESSAGE(sut == IoError::StreamNotCreated, F("Error should match StreamNotCreated."));
    TEST_ASSERT_TRUE_MESSAGE(IoError::StreamNotCreated == sut, F("StreamNotCreated should match Error."));
    TEST_ASSERT_FALSE_MESSAGE(sut == IoError::StreamClosed, F("StreamNotCreated should not match StreamClosed."));
}

void IoError_ToError_ShouldMatchStreamClosed()
{
    Error sut = to_error(IoError::StreamClosed);
    TEST_ASSERT_TRUE_MESSAGE(sut, F("StreamClosed error should be truthy."));
    TEST_ASSERT_TRUE_MESSAGE(sut == IoError::StreamClosed, F("Error should match StreamClosed."));
}

void IoError_MakeError_ShouldWrapIoError()
{
    Expected<uint8_t, Error> sut = make_error(IoError::StreamClosed);
    TEST_ASSERT_FALSE_MESSAGE(sut.hasValue(), F("make_error should produce an error Expected."));
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == IoError::StreamClosed, F("Expected should hold StreamClosed."));
}

#endif
#endif
