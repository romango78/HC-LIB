// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_Q10_TESTS_H_
#define _HC_LIB_Q10_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "Q10.h"

/// @brief Raw Q10 bits (value × 1024). Avoids float Unity asserts, which nano builds exclude.
/// @param t_value The Q10 value to read.
/// @return The stored step, rounded from toDouble().
int32_t q10Raw(const Q10& t_value)
{
    return static_cast<int32_t>(std::round(t_value.toDouble() * SCALE_FACTOR));
}

/// @brief Tests the default constructor.
/// @note A default Q10 is zero in both raw steps and toInt32().
void Q10_DefaultCtor_ShouldBeZero()
{
    Q10 sut;

    TEST_ASSERT_EQUAL_INT32(0, q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(0, sut.toInt32());
}

/// @brief Tests the int32_t constructor.
/// @note An integer is stored as value << 10, and toInt32() returns that integer.
void Q10_IntCtor_ShouldShiftByFractionalBits()
{
    Q10 sut(5);

    TEST_ASSERT_EQUAL_INT32(5 * SCALE_FACTOR, q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(5, sut.toInt32());
}

/// @brief Tests the float constructor.
/// @note 1.5 becomes 1536 steps. 0.275 × 1024 rounds to 282.
void Q10_FloatCtor_ShouldRoundToNearestStep()
{
    Q10 half(1.5f);
    Q10 gain(0.275f);

    TEST_ASSERT_EQUAL_INT32(1536, q10Raw(half));
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 1.5f, half.toFloat());
    TEST_ASSERT_EQUAL_INT32(282, q10Raw(gain));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.275f, gain.toFloat());
}

/// @brief Tests the double constructor.
/// @note 0.275 uses the same nearest-step rounding as the float constructor.
void Q10_DoubleCtor_ShouldMatchFloatCtor()
{
    Q10 sut(0.275);

    TEST_ASSERT_EQUAL_INT32(282, q10Raw(sut));
    //TEST_ASSERT_DOUBLE_WITHIN(0.001f, 0.275f, gain.toDouble());
}

/// @brief Tests a negative integer constructor.
/// @note The sign is kept in the raw steps and in toInt32().
void Q10_NegativeInt_ShouldKeepSign()
{
    Q10 sut(-3);

    TEST_ASSERT_EQUAL_INT32(-3 * SCALE_FACTOR, q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(-3, sut.toInt32());
}

/// @brief Tests addition.
/// @note 1.5 + 2.25 equals 3.75 with no extra scaling.
void Q10_Add_ShouldAddRawValues()
{
    Q10 sut = Q10(1.5) + Q10(2.25);

    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(3.75)), q10Raw(sut));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.75f, sut.toFloat());
}

/// @brief Tests subtraction of a larger value.
/// @note 1.5 − 2 is −512 steps. Arithmetic >> 10 of −512 yields −1 from toInt32().
void Q10_Subtract_ShouldKeepFractionalSign()
{
    Q10 sut = Q10(1.5) - Q10(2);

    TEST_ASSERT_EQUAL_INT32(-512, q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(-1, sut.toInt32());
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -0.5f, sut.toFloat());
}

/// @brief Tests multiplication.
/// @note (A × B) >> 10. Covers an integer product, a fraction, and a negative factor.
void Q10_Multiply_ShouldScaleBackBy1024()
{
    Q10 sut = Q10(1.5) * Q10(2);
    Q10 quarter = Q10(0.5) * Q10(0.5);
    Q10 negative = Q10(-2) * Q10(3);

    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(3)), q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(0.25)), q10Raw(quarter));
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(-6)), q10Raw(negative));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.0f, sut.toFloat());
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.25f, quarter.toFloat());
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -6.0f, negative.toFloat());
}

/// @brief Tests division.
/// @note 3 / 1.5 is 2. 1 / 3 truncates toward zero to 341 steps.
void Q10_Divide_ShouldTruncateTowardZero()
{
    Q10 sut = Q10(3) / Q10(1.5);
    Q10 third = Q10(1) / Q10(3);

    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(2)), q10Raw(sut));
    TEST_ASSERT_EQUAL_INT32(341, q10Raw(third));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, sut.toFloat());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.333f, third.toFloat());
}

/// @brief Tests +=, −=, *=, and /= on the same instance.
/// @note The sequence is 4 → 5.5 → 5 → 2.5 → 1.25.
void Q10_AssignmentOperators_ShouldUpdateInPlace()
{
    Q10 sut(4);

    sut += Q10(1.5);
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(5.5)), q10Raw(sut));

    sut -= Q10(0.5);
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(5)), q10Raw(sut));

    sut *= Q10(0.5);
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(2.5)), q10Raw(sut));

    sut /= Q10(2);
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(1.25)), q10Raw(sut));
}

/// @brief Tests sqrt() of zero.
/// @note sqrt(0) is the Q10 value 0.
void Q10_Sqrt_ShouldReturnZero_WhenValueIsZero()
{
    Expected<Q10, Error> sut = Q10(0).sqrt();

    TEST_ASSERT_TRUE(sut.hasValue());
    TEST_ASSERT_EQUAL_INT32(0, q10Raw(sut.getValue()));
}

/// @brief Tests sqrt() of a negative value.
/// @note A negative input is GenericError::InvalidOperation.
void Q10_Sqrt_ShouldReturnInvalidOperation_WhenValueIsNegative()
{
    Expected<Q10, Error> sut = Q10(-4).sqrt();

    TEST_ASSERT_FALSE(sut.hasValue());
    TEST_ASSERT_TRUE(sut.getError() == GenericError::InvalidOperation);
}

/// @brief Tests sqrt() when the result is an exact Q10 value.
/// @note sqrt(4) is 2 and sqrt(0.25) is 0.5.
void Q10_Sqrt_ShouldBeExact_WhenResultIsRepresentable()
{
    Expected<Q10, Error> four = Q10(4).sqrt();
    Expected<Q10, Error> quarter = Q10(0.25).sqrt();

    TEST_ASSERT_TRUE(four.hasValue());
    TEST_ASSERT_TRUE(quarter.hasValue());
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(2)), q10Raw(four.getValue()));
    TEST_ASSERT_EQUAL_INT32(q10Raw(Q10(0.5)), q10Raw(quarter.getValue()));
}

/// @brief Tests sqrt() when the result is not an exact Q10 step.
/// @note sqrt(2) × 1024 ≈ 1448.15, so the nearest step is 1448.
void Q10_Sqrt_ShouldRoundToNearestStep_WhenIrrational()
{
    Expected<Q10, Error> sut = Q10(2).sqrt();

    TEST_ASSERT_TRUE(sut.hasValue());
    TEST_ASSERT_EQUAL_INT32(1448, q10Raw(sut.getValue()));
}

#endif
#endif
