// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DATETIME_PROVIDER_H_
#define _DATETIME_PROVIDER_H_

#include "providers/IDateTimeProvider.h"

#if defined(ARDUINO)
#include <Arduino.h>
#include <time.h>
#else
#include <ctime>
#endif

/// @brief Local wall clock for log timestamps.
/// @note Native uses the OS local time.
/// @note The Nano has no RTC. AVR libc time() also stays frozen unless
///       system_tick() is called at 1 Hz, which Arduino does not do. This
///       provider seeds from __DATE__/__TIME__ (the sketch compile instant)
///       and advances with millis().
class DateTimeProvider : public IDateTimeProvider
{
    public:
        DateTimeProvider() = default;

        DateTime getLocalDatetime() const override
        {
#if defined(ARDUINO)
            static const time_t compiled = compileUnixTime();
            const time_t now = compiled + static_cast<time_t>(millis() / 1000UL);
            tm broken;
            gmtime_r(&now, &broken);
            return fromTm(broken);
#else
            const std::time_t now = std::time(nullptr);
            std::tm broken = {};
#if defined(_WIN32)
            localtime_s(&broken, &now);
#else
            localtime_r(&now, &broken);
#endif
            return fromTm(broken);
#endif
        };

    private:
#if defined(ARDUINO)
        using BrokenTime = tm;
#else
        using BrokenTime = std::tm;
#endif

        static DateTime fromTm(const BrokenTime& t_broken)
        {
            DateTime dt;
            dt.year = t_broken.tm_year + 1900;
            dt.month = t_broken.tm_mon + 1;
            dt.day = t_broken.tm_mday;
            dt.hours = t_broken.tm_hour;
            dt.minutes = t_broken.tm_min;
            dt.seconds = t_broken.tm_sec;
            return dt;
        };

#if defined(ARDUINO)
        /// @brief Month 1-12 from the "Mmm" prefix of __DATE__.
        static int monthFromAbbrev(const char *t_date)
        {
            switch(t_date[0])
            {
                case 'J': return t_date[1] == 'a' ? 1 : (t_date[2] == 'n' ? 6 : 7);
                case 'F': return 2;
                case 'M': return t_date[2] == 'r' ? 3 : 5;
                case 'A': return t_date[1] == 'p' ? 4 : 8;
                case 'S': return 9;
                case 'O': return 10;
                case 'N': return 11;
                case 'D': return 12;
                default: return 1;
            }
        };

        /// @brief Unix time of __DATE__ and __TIME__, treated as UTC so gmtime_r
        ///        prints the compiler's local clock without a timezone offset.
        static time_t compileUnixTime()
        {
            const char *date = __DATE__;
            const char *timeOfDay = __TIME__;

            tm compiled = {};
            compiled.tm_year = (date[7] - '0') * 1000 + (date[8] - '0') * 100
                + (date[9] - '0') * 10 + (date[10] - '0') - 1900;
            compiled.tm_mon = monthFromAbbrev(date) - 1;
            compiled.tm_mday = (date[4] == ' ' ? 0 : (date[4] - '0') * 10) + (date[5] - '0');
            compiled.tm_hour = (timeOfDay[0] - '0') * 10 + (timeOfDay[1] - '0');
            compiled.tm_min = (timeOfDay[3] - '0') * 10 + (timeOfDay[4] - '0');
            compiled.tm_sec = (timeOfDay[6] - '0') * 10 + (timeOfDay[7] - '0');
            return mk_gmtime(&compiled);
        };
#endif
};

#endif
