// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file Q10.h
/// @brief Q10 library.
#ifndef _HC_LIB_Q10_H_
#define _HC_LIB_Q10_H_

#include <stdint.h>
#include <cmath>
#include "errors/GenericErrors.h"

#define FRACTIONAL_BITS 10
#define SCALE_FACTOR (1 << FRACTIONAL_BITS)

/// @brief Q10 class.
/// @note The value is stored as a 32-bit integer with the fractional part in the lower 10 bits.
class Q10
{
private:
    /// @brief The raw integer storing the fixed-point value.
    int32_t m_value;

    /// @brief Internal constructor to create a Q10 directly from raw bits.
    constexpr explicit Q10(int32_t t_value, bool) : m_value(t_value) {}
public:
    /// @brief Constructors.
    constexpr Q10() : m_value(0) {};
    constexpr Q10(int32_t t_value) : m_value(t_value << FRACTIONAL_BITS) {};
    constexpr Q10(float t_value) : m_value(static_cast<int32_t>(std::round(t_value * SCALE_FACTOR))) {};
    constexpr Q10(double t_value) : m_value(static_cast<int32_t>(std::round(t_value * SCALE_FACTOR))) {};

    /// @brief Convert the Q10 to a float.
    constexpr float toFloat() const { return static_cast<float>(m_value) / SCALE_FACTOR; }
    /// @brief Convert the Q10 to a double.
    constexpr double toDouble() const { return static_cast<double>(m_value) / SCALE_FACTOR; }
    /// @brief Convert the Q10 to a int32_t.
    constexpr int32_t toInt32() const { return m_value >> FRACTIONAL_BITS; }

    /// @brief Square root, still in Q10.
    /// @return The root when the value is zero or positive. GenericError::InvalidOperation when the value is negative.
    /// @note sqrt(raw / 1024) * 1024 = isqrt(raw << 10).
    Expected<Q10, Error> sqrt() const
    {
        if(m_value < 0)
        {
            return make_error(GenericError::InvalidOperation);
        }
        if(m_value == 0)
        {
            return Q10(0, true);
        }

        uint64_t remainder = static_cast<uint64_t>(m_value) << FRACTIONAL_BITS;
        uint64_t result = 0;
        uint64_t bit = 1ULL << 62;
        while(bit > remainder)
        {
            bit >>= 2;
        }
        while(bit != 0)
        {
            const uint64_t candidate = result + bit;
            if(remainder >= candidate)
            {
                remainder -= candidate;
                result = (result >> 1) + bit;
            }
            else
            {
                result >>= 1;
            }
            bit >>= 2;
        }
        if(remainder > result)
        {
            result++;
        }
        return Q10(static_cast<int32_t>(result), true);
    }
    
    /// @brief Addition operator (no scaling adjustment needed).
    constexpr Q10 operator+(const Q10& other) const {
        return Q10(m_value + other.m_value, true);
    }
    /// @brief Subtraction operator (no scaling adjustment needed).
    constexpr Q10 operator-(const Q10& other) const {
        return Q10(m_value - other.m_value, true);
    }
    /// @brief Multiplication operator.
    /// @note Multiplication: (A * B) >> 10
    constexpr Q10 operator*(const Q10& other) const {        
        return Q10(static_cast<int32_t>((static_cast<int64_t>(m_value) * other.m_value) >> FRACTIONAL_BITS), true);
    }
    /// @brief Division operator.
    /// @note Division: (A << 10) / B. The shift must be parenthesized; / binds tighter than <<.
    constexpr Q10 operator/(const Q10& other) const {
        return Q10(static_cast<int32_t>((static_cast<int64_t>(m_value) << FRACTIONAL_BITS) / other.m_value), true);
    }
    
    /// @brief Addition assignment operator.
    Q10& operator+=(const Q10& other) { 
        m_value += other.m_value; 
        return *this; 
    }
    /// @brief Subtraction assignment operator.
    Q10& operator-=(const Q10& other) { 
        m_value -= other.m_value; 
        return *this; 
    }
    /// @brief Multiplication assignment operator.
    Q10& operator*=(const Q10& other) { 
        *this = *this * other; 
        return *this; 
    }
    /// @brief Division assignment operator.
    Q10& operator/=(const Q10& other) { 
        *this = *this / other; 
        return *this; 
    }

    /// @brief Equal when both raw steps match.
    constexpr bool operator==(const Q10& other) const { return m_value == other.m_value; }
    /// @brief Not equal when the raw steps differ.
    constexpr bool operator!=(const Q10& other) const { return m_value != other.m_value; }
    /// @brief Less than, using the signed raw steps.
    constexpr bool operator<(const Q10& other) const { return m_value < other.m_value; }
    /// @brief Greater than, using the signed raw steps.
    constexpr bool operator>(const Q10& other) const { return m_value > other.m_value; }
    /// @brief Less than or equal, using the signed raw steps.
    constexpr bool operator<=(const Q10& other) const { return m_value <= other.m_value; }
    /// @brief Greater than or equal, using the signed raw steps.
    constexpr bool operator>=(const Q10& other) const { return m_value >= other.m_value; }
};

#endif