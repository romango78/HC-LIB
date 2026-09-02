// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IDateTimeProvider.h
/// @brief Local date/time source for log timestamps.
#ifndef _HC_LIB_I_DATETIME_PROVIDER_H_
#define _HC_LIB_I_DATETIME_PROVIDER_H_

/// @brief Calendar date and time used in log prefixes.
struct DateTime
{
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    int seconds;
};

/// @brief Supplies the current local date and time.
class IDateTimeProvider
{
    public:
        virtual ~IDateTimeProvider() = default;

        /// @brief Returns the current local date and time.
        virtual DateTime getLocalDatetime() const = 0;
};

#endif
