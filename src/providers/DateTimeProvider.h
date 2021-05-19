// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DATETIME_PROVIDER_H_
#define _DATETIME_PROVIDER_H_

#include <time.h>
#include "IDatetimeProvider.h"

class DateTimeProvider : IDateTimeProvider
{
    public:
        DateTimeProvider() {};
        DateTime* getLocalDatetime() override
        {
            DateTime *dt = new DateTime;
            
            time_t now = time(0);
            struct tm  tStruct = *localtime(&now);
            dt->year = 1900 + tStruct.tm_year;
            dt->month = tStruct.tm_mon;
            dt->day = tStruct.tm_mday;
            dt->hours = tStruct.tm_hour;
            dt->minutes = tStruct.tm_min;
            dt->seconds = tStruct.tm_sec;

            return dt;
        };        
};

#endif