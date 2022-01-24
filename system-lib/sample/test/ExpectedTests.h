// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _EXPECTED_TESTS_H_
#define _EXPECTED_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "Expected.h"

class FakeTest
{
    public:
        uint8_t data;
        const char* string; 
};

Expected<uint8_t> getValueTypeTestMethod(const int value)
{
    if(value < 0 || value > 255)
    {
        return Expected<uint8_t>::fromError(OUT_OF_RANGE_ERROR);
    }
    return (uint8_t)value;
};

Expected<FakeTest*> getPointerTestMethod(FakeTest* value)
{
    if(!value)
    {
        return Expected<FakeTest*>::fromError(ARGUMENT_IS_NULL_ERROR);
    }
    return value;
};

Expected<FakeTest> getReferenceTypeTestMethod(FakeTest &value)
{
    if(&value == nullptr)
    {
        return Expected<FakeTest>::fromError(ARGUMENT_IS_NULL_ERROR);
    }
    return value;
};

Expected<void*> getVoidTestMethod(const int value)
{
    if(value < 0 || value > 255)
    {
        return Expected<void*>::fromError(OUT_OF_RANGE_ERROR);
    }
    return nullptr;
}

Expected<uint8_t> getExpectedTestMethod(const int value)
{
    return getValueTypeTestMethod(value);
}

void ExpectedValueType_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    Expected<uint8_t> sut = getValueTypeTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.getValue(),"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, sut.getError(),"Expected<T> doesn't not contain an error.");
};

void ExpectedValueType_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    Expected<uint8_t> sut = getValueTypeTestMethod(-10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(OUT_OF_RANGE_ERROR, sut.getError(),"Expected<T> should contain OUT_OF_RANGE_ERROR error.");
};

void ExpectedPointer_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Arrange
    FakeTest* p = new FakeTest();
    p->data = 125;
    p->string = "Test String.";

    // Act
    auto sut = getPointerTestMethod(p);
    p->data = 50;

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(p->data, sut.getValue()->data,"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(p->string, sut.getValue()->string,"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, sut.getError(),"Expected<T> doesn't not contain an error.");
};

void ExpectedPointer_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    auto sut = getPointerTestMethod(nullptr);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(ARGUMENT_IS_NULL_ERROR, sut.getError(),"Expected<T> should contain OUT_OF_RANGE_ERROR error.");
};

void ExpectedReferenceValue_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Arrange
    FakeTest p = FakeTest();
    p.data = 125;
    p.string = "Test String.";

    // Act
    auto sut = getReferenceTypeTestMethod(p);
    p.data = 50;

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(125, sut.getValue().data,"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(p.string, sut.getValue().string,"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, sut.getError(),"Expected<T> doesn't not contain an error.");
};

void ExpectedReferenceValue_ShouldContainError_WhenSomeErrorOccurred()
{
    // Arrange
    FakeTest* p = nullptr;

    // Act    
    auto sut = getReferenceTypeTestMethod(*p);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(ARGUMENT_IS_NULL_ERROR, sut.getError(),"Expected<T> should contain OUT_OF_RANGE_ERROR error.");
};

void ExpectedVoid_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    auto sut = getVoidTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_NULL_MESSAGE(sut.getValue(),"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, sut.getError(),"Expected<T> doesn't not contain an error.");
};

void ExpectedVoid_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act    
    auto sut = getVoidTestMethod(300);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(OUT_OF_RANGE_ERROR, sut.getError(),"Expected<T> should contain OUT_OF_RANGE_ERROR error.");
};

void ExpectedExpectedType_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    Expected<uint8_t> sut = getExpectedTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.getValue(),"Expected<T> contains value.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_ERROR, sut.getError(),"Expected<T> doesn't not contain an error.");
};

void ExpectedExpected_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    Expected<uint8_t> sut = getValueTypeTestMethod(-10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(OUT_OF_RANGE_ERROR, sut.getError(),"Expected<T> should contain OUT_OF_RANGE_ERROR error.");
};

#endif
#endif