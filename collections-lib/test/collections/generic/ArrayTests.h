// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ARRAY_TESTS_H_
#define _HC_LIB_ARRAY_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "collections/generic/Array.h"

/// @file ArrayTests.h
/// @brief Unity tests for Array{T, LENGTH}.

/// @brief A new array reports LENGTH and every slot is T().
void Array_ShouldReportLength_AndDefaultElements()
{
    Array<uint8_t, 4> sut;

    TEST_ASSERT_EQUAL_UINT32(4u, sut.getCount());
    TEST_ASSERT_EQUAL_UINT8(0u, sut[0]);
    TEST_ASSERT_EQUAL_UINT8(0u, sut[3]);
}

/// @brief setValue() stores an element. operator[] and getValue() return that element.
void Array_ShouldSetAndReadElement()
{
    Array<uint8_t, 3> sut;

    Expected<bool, Error> result = sut.setValue(42, 1);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_EQUAL_UINT8(42u, sut[1]);
    TEST_ASSERT_EQUAL_UINT8(0u, sut[0]);

    Expected<uint8_t, Error> value = sut.getValue(1);
    TEST_ASSERT_TRUE(value.hasValue());
    TEST_ASSERT_EQUAL_UINT8(42u, value.getValue());
}

/// @brief operator[] returns a reference. Assigning through it stores the element.
void Array_ShouldAssignElement_ThroughOperator()
{
    Array<uint8_t, 3> sut;

    sut[1] = 42;

    TEST_ASSERT_EQUAL_UINT8(42u, sut[1]);
    TEST_ASSERT_EQUAL_UINT8(0u, sut[0]);
    Expected<uint8_t, Error> value = sut.getValue(1);
    TEST_ASSERT_TRUE(value.hasValue());
    TEST_ASSERT_EQUAL_UINT8(42u, value.getValue());
}

/// @brief operator[] past the end does not change a stored element.
void Array_ShouldNotStoreElement_WhenOperatorIndexPastEnd()
{
    Array<uint8_t, 2> sut;
    sut[0] = 9;
    sut[1] = 8;

    sut[2] = 99;

    TEST_ASSERT_EQUAL_UINT8(9u, sut[0]);
    TEST_ASSERT_EQUAL_UINT8(8u, sut[1]);
    TEST_ASSERT_EQUAL_UINT32(2u, sut.getCount());
    Expected<uint8_t, Error> value = sut.getValue(2);
    TEST_ASSERT_FALSE(value.hasValue());
    TEST_ASSERT_TRUE(value.getError() == GenericError::OutOfRange);
}

/// @brief getValue() returns GenericError::OutOfRange when the index is past the end.
void Array_ShouldReturnOutOfRange_WhenIndexPastEnd()
{
    Array<uint8_t, 2> sut;

    Expected<uint8_t, Error> result = sut.getValue(2);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == GenericError::OutOfRange);
}

/// @brief setValue() returns GenericError::OutOfRange when the index is past the end.
void Array_ShouldReturnOutOfRange_WhenSetPastEnd()
{
    Array<uint8_t, 2> sut;

    Expected<bool, Error> result = sut.setValue(7, 2);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == GenericError::OutOfRange);
}

/// @brief clear() writes T() into every slot and leaves the length unchanged.
void Array_ShouldClearElements()
{
    Array<uint8_t, 2> sut;
    sut.setValue(9, 0);
    sut.setValue(8, 1);

    sut.clear();

    TEST_ASSERT_EQUAL_UINT8(0u, sut[0]);
    TEST_ASSERT_EQUAL_UINT8(0u, sut[1]);
    TEST_ASSERT_EQUAL_UINT32(2u, sut.getCount());
}

/// @brief getEnumerator() yields elements from index 0 through LENGTH - 1, then moveNext() stays false.
void Array_ShouldEnumerateInIndexOrder()
{
    Array<uint8_t, 3> sut;
    sut.setValue(10, 0);
    sut.setValue(20, 1);
    sut.setValue(30, 2);
    std::unique_ptr<IEnumerator<uint8_t>> enumerator = sut.getEnumerator();
    uint8_t expected[] = {10, 20, 30};
    uint8_t index = 0;

    while (enumerator->moveNext())
    {
        Expected<uint8_t, Error> current = enumerator->getCurrent();
        TEST_ASSERT_TRUE(current.hasValue());
        TEST_ASSERT_EQUAL_UINT8(expected[index], current.getValue());
        ++index;
    }

    TEST_ASSERT_EQUAL_UINT8(3u, index);
    TEST_ASSERT_FALSE(enumerator->moveNext());
}

#endif
#endif
