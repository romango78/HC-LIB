// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _MOVE_TESTS_H_
#define _MOVE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "lib-utility.h"

template<typename T, typename U>
struct MoveTestSame
{
    enum { value = 0 };
};

template<typename T>
struct MoveTestSame<T, T>
{
    enum { value = 1 };
};

struct MoveProbe
{
    static int copied;
    static int moved;
    int id;

    MoveProbe(int t_id = 0)
        : id(t_id)
    {
    }

    MoveProbe(const MoveProbe &t_other)
        : id(t_other.id)
    {
        copied++;
    }

    MoveProbe(MoveProbe &&t_other)
        : id(t_other.id)
    {
        moved++;
        t_other.id = -1;
    }

    MoveProbe& operator=(const MoveProbe &t_other)
    {
        id = t_other.id;
        copied++;
        return *this;
    }

    MoveProbe& operator=(MoveProbe &&t_other)
    {
        id = t_other.id;
        t_other.id = -1;
        moved++;
        return *this;
    }
};

int MoveProbe::copied = 0;
int MoveProbe::moved = 0;

template<typename T>
MoveProbe forwardIntoProbe(T &&t_value)
{
    return MoveProbe(std::forward<T>(t_value));
}

void Move_ShouldNotMutateSource_WhenResultIsNotConsumed()
{
    int value = 42;

    int &&alias = std::move(value);

    TEST_ASSERT_EQUAL_MESSAGE(42, value, "std::move should not change the source by itself.");
    TEST_ASSERT_EQUAL_MESSAGE(42, alias, "std::move should refer to the same value.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&value, &alias, "std::move should be a cast to an rvalue reference.");
};

void Move_ShouldSelectMoveConstructor_WhenInitializingFromMovedLvalue()
{
    MoveProbe::copied = 0;
    MoveProbe::moved = 0;
    MoveProbe source(7);

    MoveProbe sut(std::move(source));

    TEST_ASSERT_EQUAL_MESSAGE(0, MoveProbe::copied, "std::move should not select the copy constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(1, MoveProbe::moved, "std::move should select the move constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(7, sut.id, "Moved value should be preserved.");
    TEST_ASSERT_EQUAL_MESSAGE(-1, source.id, "Source should be left in a moved-from state.");
};

void Move_ShouldSelectMoveAssignment_WhenAssigningFromMovedLvalue()
{
    MoveProbe::copied = 0;
    MoveProbe::moved = 0;
    MoveProbe source(3);
    MoveProbe sut(1);

    sut = std::move(source);

    TEST_ASSERT_EQUAL_MESSAGE(0, MoveProbe::copied, "std::move should not select copy assignment.");
    TEST_ASSERT_EQUAL_MESSAGE(1, MoveProbe::moved, "std::move should select move assignment.");
    TEST_ASSERT_EQUAL_MESSAGE(3, sut.id, "Move assignment should transfer the value.");
    TEST_ASSERT_EQUAL_MESSAGE(-1, source.id, "Source should be left in a moved-from state.");
};

void Move_ShouldSelectCopyConstructor_WhenSourceIsConst()
{
    MoveProbe::copied = 0;
    MoveProbe::moved = 0;
    const MoveProbe source(9);

    MoveProbe sut(std::move(source));

    TEST_ASSERT_EQUAL_MESSAGE(1, MoveProbe::copied, "std::move of a const lvalue should select the copy constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(0, MoveProbe::moved, "std::move of a const lvalue should not select the move constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(9, sut.id, "Copied value should be preserved.");
    TEST_ASSERT_EQUAL_MESSAGE(9, source.id, "Const source should be unchanged.");
};

void Forward_ShouldSelectCopyConstructor_WhenForwardingLvalue()
{
    MoveProbe::copied = 0;
    MoveProbe::moved = 0;
    MoveProbe source(4);

    MoveProbe sut = forwardIntoProbe(source);

    TEST_ASSERT_EQUAL_MESSAGE(1, MoveProbe::copied, "std::forward of an lvalue should select the copy constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(0, MoveProbe::moved, "std::forward of an lvalue should not select the move constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(4, sut.id, "Forwarded lvalue should be copied.");
    TEST_ASSERT_EQUAL_MESSAGE(4, source.id, "Forwarded lvalue source should be unchanged.");
};

void Forward_ShouldSelectMoveConstructor_WhenForwardingRvalue()
{
    MoveProbe::copied = 0;
    MoveProbe::moved = 0;
    MoveProbe source(5);

    MoveProbe sut = forwardIntoProbe(std::move(source));

    TEST_ASSERT_EQUAL_MESSAGE(0, MoveProbe::copied, "std::forward of an rvalue should not select the copy constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(1, MoveProbe::moved, "std::forward of an rvalue should select the move constructor.");
    TEST_ASSERT_EQUAL_MESSAGE(5, sut.id, "Forwarded rvalue should be moved.");
    TEST_ASSERT_EQUAL_MESSAGE(-1, source.id, "Forwarded rvalue source should be left in a moved-from state.");
};

void Forward_ShouldPreserveAddress_WhenForwardingLvalueReference()
{
    int value = 11;

    int &forwarded = std::forward<int&>(value);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&value, &forwarded, "std::forward of an lvalue should refer to the same object.");
    TEST_ASSERT_EQUAL_MESSAGE(11, forwarded, "std::forward of an lvalue should keep the value.");
};

void RemoveReference_ShouldYieldValueType_WhenGivenValueOrReference()
{
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::remove_reference<int>::type, int>::value),
        "remove_reference<int> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::remove_reference<int&>::type, int>::value),
        "remove_reference<int&> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::remove_reference<int&&>::type, int>::value),
        "remove_reference<int&&> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::remove_reference<const int&>::type, const int>::value),
        "remove_reference<const int&> should keep const.");
};

void Decay_ShouldStripReferenceAndCv_WhenGivenQualifiedType()
{
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::decay<int>::type, int>::value),
        "decay<int> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::decay<int&>::type, int>::value),
        "decay<int&> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::decay<int&&>::type, int>::value),
        "decay<int&&> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::decay<const int&>::type, int>::value),
        "decay<const int&> should be int.");
    TEST_ASSERT_TRUE_MESSAGE(
        (MoveTestSame<std::decay<const volatile int&&>::type, int>::value),
        "decay<const volatile int&&> should be int.");
};

#endif
#endif
