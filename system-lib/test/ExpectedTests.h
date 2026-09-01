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
#include "errors/GenericErrors.h"

class FakeTest
{
    public:
        uint8_t data;
        const char* string; 
};

Expected<uint8_t, Error> getValueTypeTestMethod(const int value)
{
    if(value < 0 || value > 255)
    {
        return make_error(GenericError::OutOfRange);
    }
    return (uint8_t)value;
};

Expected<FakeTest*, Error> getPointerTestMethod(FakeTest* value)
{
    if(!value)
    {
        return make_error(GenericError::ArgumentIsNull);
    }
    return value;
};

Expected<FakeTest, Error> getReferenceTypeTestMethod(FakeTest &value)
{
    if(&value == nullptr)
    {
        return make_error(GenericError::ArgumentIsNull);
    }
    return value;
};

Expected<void*, Error> getVoidTestMethod(const int value)
{
    if(value < 0 || value > 255)
    {
        return make_error(GenericError::OutOfRange);
    }
    return nullptr;
}

Expected<uint8_t, Error> getExpectedTestMethod(const int value)
{
    return getValueTypeTestMethod(value);
}

void ExpectedValueType_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    Expected<uint8_t, Error> sut = getValueTypeTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.getValue(),"Expected<T> contains value.");
};

void ExpectedValueType_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    Expected<uint8_t, Error> sut = getValueTypeTestMethod(-10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::OutOfRange, "Expected<T> should contain OutOfRange error.");
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
    delete p;
};

void ExpectedPointer_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    auto sut = getPointerTestMethod(nullptr);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::ArgumentIsNull, "Expected<T> should contain ArgumentIsNull error.");
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
};

void ExpectedReferenceValue_ShouldContainError_WhenSomeErrorOccurred()
{
    // Arrange
    FakeTest* p = nullptr;

    // Act    
    auto sut = getReferenceTypeTestMethod(*p);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::ArgumentIsNull, "Expected<T> should contain ArgumentIsNull error.");
};

void ExpectedVoid_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    auto sut = getVoidTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_NULL_MESSAGE(sut.getValue(),"Expected<T> contains value.");
};

void ExpectedVoid_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act    
    auto sut = getVoidTestMethod(300);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::OutOfRange, "Expected<T> should contain OutOfRange error.");
};

void ExpectedExpectedType_ShouldNotContainError_WhenSomeErrorNotOccurred()
{
    // Act
    Expected<uint8_t, Error> sut = getExpectedTestMethod(10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should not contain an error.");
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.getValue(),"Expected<T> contains value.");
};

void ExpectedExpected_ShouldContainError_WhenSomeErrorOccurred()
{
    // Act
    Expected<uint8_t, Error> sut = getValueTypeTestMethod(-10);

    // Assert
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::OutOfRange, "Expected<T> should contain OutOfRange error.");
};

struct DtorCounter
{
    static int constructed;
    static int destroyed;
    int id;

    DtorCounter(int t_id = 0) noexcept
        : id(t_id)
    {
        constructed++;
    }

    DtorCounter(const DtorCounter &t_other)
        : id(t_other.id)
    {
        constructed++;
    }

    DtorCounter(DtorCounter &&t_other) noexcept
        : id(t_other.id)
    {
        constructed++;
        // Mark the moved-from object as destroyed
        t_other.id = -1;
    }

    DtorCounter& operator=(const DtorCounter &t_other)
    {
        // Prevent self-assignment
        if (this != &t_other)
        {
            id = t_other.id;
        }

        return *this;
    }

    DtorCounter& operator=(DtorCounter &&t_other) noexcept
    {
        // Prevent self-assignment
        if (this != &t_other)
        {
            id = t_other.id;
            // Mark the moved-from object as destroyed
            t_other.id = -1;
        }

        return *this;
    }

    ~DtorCounter()
    {
        destroyed++;
    }
};

int DtorCounter::constructed = 0;
int DtorCounter::destroyed = 0;

void Expected_ValueCtor_ShouldStoreValue_WhenConstructedFromConstLvalue()
{
    const DtorCounter value(4);

    Expected<DtorCounter, Error> sut(value);

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(4, sut.getValue().id, "Expected<T> should copy the lvalue.");
    TEST_ASSERT_EQUAL_MESSAGE(4, value.id, "Source lvalue should be unchanged.");
};

void Expected_ValueCtor_ShouldMoveValue_WhenConstructedFromRvalue()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    DtorCounter value(4);
    Expected<DtorCounter, Error> sut(std::move(value));

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Expected<T> should hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(4, sut.getValue().id, "Expected<T> should move the rvalue.");
    TEST_ASSERT_EQUAL_MESSAGE(-1, value.id, "Source rvalue should be moved-from.");
};

void Expected_ErrorCtor_ShouldStoreError_WhenConstructedFromUnexpectedLvalue()
{
    const Unexpected<Error> unexpected = make_error(GenericError::OutOfRange);

    Expected<uint8_t, Error> sut(unexpected);

    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should hold an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::OutOfRange, "Expected<T> should copy the unexpected error.");
};

void Expected_ErrorCtor_ShouldStoreError_WhenConstructedFromUnexpectedRvalue()
{
    Expected<uint8_t, Error> sut(make_error(GenericError::ArgumentIsNull));

    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should hold an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::ArgumentIsNull, "Expected<T> should move the unexpected error.");
};

void Expected_ErrorCtor_ShouldConvertError_WhenUnexpectedTypeDiffers()
{
    Unexpected<short> unexpected(static_cast<short>(42));

    Expected<uint8_t, int> sut(unexpected);

    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Expected<T> should hold a converted error.");
    TEST_ASSERT_EQUAL_MESSAGE(42, sut.getError(), "Expected<T> should construct E from Unexpected<U>.");
};

void Expected_CopyCtor_ShouldCopyValue_WhenSourceHasValue()
{
    Expected<DtorCounter, Error> original = DtorCounter(7);

    Expected<DtorCounter, Error> sut(original);

    TEST_ASSERT_EQUAL_MESSAGE(true, original.hasValue(), "Copy source should still hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Copy destination should hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(7, original.getValue().id, "Copy source value should be unchanged.");
    TEST_ASSERT_EQUAL_MESSAGE(7, sut.getValue().id, "Copy destination should contain the same value.");
};

void Expected_CopyCtor_ShouldCopyError_WhenSourceHasError()
{
    Expected<uint8_t, Error> original = make_error(GenericError::OutOfRange);

    Expected<uint8_t, Error> sut(original);

    TEST_ASSERT_EQUAL_MESSAGE(false, original.hasValue(), "Copy source should still hold an error.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.hasValue(), "Copy destination should hold an error.");
    TEST_ASSERT_TRUE_MESSAGE(sut.getError() == GenericError::OutOfRange, "Copy destination should contain the same error.");
};

void Expected_CopyAssign_ShouldCopyValue_WhenBothHaveValue()
{
    Expected<DtorCounter, Error> target = DtorCounter(1);
    Expected<DtorCounter, Error> source = DtorCounter(2);

    target = source;

    TEST_ASSERT_EQUAL_MESSAGE(true, target.hasValue(), "Copy assignment destination should hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(2, target.getValue().id, "Copy assignment should replace the value.");
    TEST_ASSERT_EQUAL_MESSAGE(true, source.hasValue(), "Copy assignment source should still hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(2, source.getValue().id, "Copy assignment source should be unchanged.");
};

void Expected_CopyAssign_ShouldCopyError_WhenBothHaveError()
{
    Expected<uint8_t, Error> target = make_error(GenericError::ArgumentIsNull);
    Expected<uint8_t, Error> source = make_error(GenericError::OutOfRange);

    target = source;

    TEST_ASSERT_EQUAL_MESSAGE(false, target.hasValue(), "Copy assignment destination should hold an error.");
    TEST_ASSERT_TRUE_MESSAGE(target.getError() == GenericError::OutOfRange, "Copy assignment should replace the error.");
    TEST_ASSERT_EQUAL_MESSAGE(false, source.hasValue(), "Copy assignment source should still hold an error.");
};

void Expected_CopyAssign_ShouldReplaceValueWithError_WhenSourceHasError()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> target = DtorCounter(1);
        Expected<DtorCounter, Error> source = make_error(GenericError::OutOfRange);

        target = source;

        TEST_ASSERT_EQUAL_MESSAGE(false, target.hasValue(), "Copy assignment should replace a value with an error.");
        TEST_ASSERT_TRUE_MESSAGE(target.getError() == GenericError::OutOfRange, "Copy assignment destination should contain the source error.");
        TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "Replacing a value with an error should destroy T.");
    }
};

void Expected_CopyAssign_ShouldReplaceErrorWithValue_WhenSourceHasValue()
{
    Expected<DtorCounter, Error> target = make_error(GenericError::OutOfRange);
    Expected<DtorCounter, Error> source = DtorCounter(2);

    target = source;

    TEST_ASSERT_EQUAL_MESSAGE(true, target.hasValue(), "Copy assignment should replace an error with a value.");
    TEST_ASSERT_EQUAL_MESSAGE(2, target.getValue().id, "Copy assignment destination should contain the source value.");
    TEST_ASSERT_EQUAL_MESSAGE(true, source.hasValue(), "Copy assignment source should still hold a value.");
    TEST_ASSERT_EQUAL_MESSAGE(2, source.getValue().id, "Copy assignment source should be unchanged.");
};

void Expected_CopyAssign_ShouldKeepValue_WhenSelfAssigned()
{
    Expected<uint8_t, Error> sut((uint8_t)5);
    Expected<uint8_t, Error> &same = sut;

    same = sut;

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Self copy-assignment should keep the value.");
    TEST_ASSERT_EQUAL_MESSAGE(5, sut.getValue(), "Self copy-assignment should not change the value.");
};

void Expected_MoveCtor_ShouldPreserveValue_WhenSourceHasValue()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> original = DtorCounter(7);
        Expected<DtorCounter, Error> moved = std::move(original);

        TEST_ASSERT_EQUAL_MESSAGE(true, moved.hasValue(), "Move destination should contain a value.");
        TEST_ASSERT_EQUAL_MESSAGE(7, moved.getValue().id, "Moved value should be preserved.");
        TEST_ASSERT_EQUAL_MESSAGE(true, original.hasValue(), "Moved-from Expected should stay engaged.");
        TEST_ASSERT_EQUAL_MESSAGE(-1, original.getValue().id, "Moved-from T should be in a moved-from state.");
    }

    TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "T destructor should run once per constructor.");
};

void Expected_MoveCtor_ShouldMoveError_WhenSourceHasError()
{
    Expected<uint8_t, Error> original = make_error(GenericError::ArgumentIsNull);

    Expected<uint8_t, Error> moved = std::move(original);

    TEST_ASSERT_EQUAL_MESSAGE(false, moved.hasValue(), "Move destination should contain an error.");
    TEST_ASSERT_TRUE_MESSAGE(moved.getError() == GenericError::ArgumentIsNull, "Moved error should be preserved.");
    TEST_ASSERT_EQUAL_MESSAGE(false, original.hasValue(), "Moved-from Expected should still hold an error.");
};

void Expected_MoveAssign_ShouldReplaceValue_WhenBothHaveValue()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> target = DtorCounter(1);
        Expected<DtorCounter, Error> source = DtorCounter(2);
        target = std::move(source);

        TEST_ASSERT_EQUAL_MESSAGE(true, target.hasValue(), "Move assignment destination should contain a value.");
        TEST_ASSERT_EQUAL_MESSAGE(2, target.getValue().id, "Move assignment should replace the value.");
        TEST_ASSERT_EQUAL_MESSAGE(true, source.hasValue(), "Moved-from Expected should stay engaged.");
        TEST_ASSERT_EQUAL_MESSAGE(-1, source.getValue().id, "Moved-from T should be in a moved-from state.");
    }

    TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "T destructor should run once per constructor.");
};

void Expected_MoveAssign_ShouldMoveError_WhenBothHaveError()
{
    Expected<uint8_t, Error> target = make_error(GenericError::ArgumentIsNull);
    Expected<uint8_t, Error> source = make_error(GenericError::OutOfRange);

    target = std::move(source);

    TEST_ASSERT_EQUAL_MESSAGE(false, target.hasValue(), "Move assignment destination should hold an error.");
    TEST_ASSERT_TRUE_MESSAGE(target.getError() == GenericError::OutOfRange, "Move assignment should replace the error.");
    TEST_ASSERT_EQUAL_MESSAGE(false, source.hasValue(), "Moved-from Expected should still hold an error.");
};

void Expected_MoveAssign_ShouldReplaceValueWithError_WhenSourceHasError()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> target = DtorCounter(1);
        Expected<DtorCounter, Error> source = make_error(GenericError::OutOfRange);

        target = std::move(source);

        TEST_ASSERT_EQUAL_MESSAGE(false, target.hasValue(), "Move assignment should replace a value with an error.");
        TEST_ASSERT_TRUE_MESSAGE(target.getError() == GenericError::OutOfRange, "Move assignment destination should contain the source error.");
        TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "Replacing a value with an error should destroy T.");
    }
};

void Expected_MoveAssign_ShouldReplaceErrorWithValue_WhenSourceHasValue()
{
    Expected<DtorCounter, Error> target = make_error(GenericError::OutOfRange);
    Expected<DtorCounter, Error> source = DtorCounter(2);

    target = std::move(source);

    TEST_ASSERT_EQUAL_MESSAGE(true, target.hasValue(), "Move assignment should replace an error with a value.");
    TEST_ASSERT_EQUAL_MESSAGE(2, target.getValue().id, "Move assignment destination should contain the moved value.");
    TEST_ASSERT_EQUAL_MESSAGE(true, source.hasValue(), "Moved-from Expected should stay engaged.");
    TEST_ASSERT_EQUAL_MESSAGE(-1, source.getValue().id, "Moved-from T should be in a moved-from state.");
};

void Expected_MoveAssign_ShouldKeepValue_WhenSelfAssigned()
{
    Expected<uint8_t, Error> sut((uint8_t)5);

    sut = std::move(sut);

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.hasValue(), "Self move-assignment should keep the value.");
    TEST_ASSERT_EQUAL_MESSAGE(5, sut.getValue(), "Self move-assignment should not change the value.");
};

void Expected_BoolConversion_ShouldBeTrue_WhenHasValue()
{
    Expected<uint8_t, Error> sut((uint8_t)10);

    TEST_ASSERT_EQUAL_MESSAGE(true, static_cast<bool>(sut), "Expected<T> should convert to true when it holds a value.");
};

void Expected_BoolConversion_ShouldBeFalse_WhenHasError()
{
    Expected<uint8_t, Error> sut = make_error(GenericError::OutOfRange);

    TEST_ASSERT_EQUAL_MESSAGE(false, static_cast<bool>(sut), "Expected<T> should convert to false when it holds an error.");
};

void Expected_Dereference_ShouldReturnValue_WhenHasValue()
{
    Expected<uint8_t, Error> sut((uint8_t)10);

    *sut = 20;

    TEST_ASSERT_EQUAL_MESSAGE(20, *sut, "operator* should return a mutable reference to the value.");
    TEST_ASSERT_EQUAL_MESSAGE(20, sut.getValue(), "getValue() should observe the mutation through operator*.");
};

void Expected_ConstAccessors_ShouldReturnValueAndError()
{
    const Expected<uint8_t, Error> value((uint8_t)10);
    const Expected<uint8_t, Error> error = make_error(GenericError::OutOfRange);

    TEST_ASSERT_EQUAL_MESSAGE(10, value.getValue(), "const getValue() should return the stored value.");
    TEST_ASSERT_EQUAL_MESSAGE(10, *value, "const operator* should return the stored value.");
    TEST_ASSERT_TRUE_MESSAGE(error.getError() == GenericError::OutOfRange, "const getError() should return the stored error.");
};

void Expected_Destroy_ShouldDestroyValue_WhenScopeEnds()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> sut = DtorCounter(1);
    }

    TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "Destroying Expected<T> should destroy the stored T.");
};

void Expected_Copy_ShouldDestroyEachCopy_WhenScopeEnds()
{
    DtorCounter::constructed = 0;
    DtorCounter::destroyed = 0;

    {
        Expected<DtorCounter, Error> original = DtorCounter(7);
        Expected<DtorCounter, Error> copy(original);
        TEST_ASSERT_EQUAL_MESSAGE(7, copy.getValue().id, "Copy should contain the same value.");
    }

    TEST_ASSERT_EQUAL_MESSAGE(DtorCounter::constructed, DtorCounter::destroyed, "Each copied T should be destroyed.");
};

#endif
#endif
