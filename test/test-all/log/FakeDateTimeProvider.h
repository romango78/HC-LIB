// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _FAKE_DATETIME_PROVIDER_H_
#define _FAKE_DATETIME_PROVIDER_H_

#include "IDatetimeProvider.h"

class FakeDateTimeProvider : IDateTimeProvider
{
    public:
        FakeDateTimeProvider() {};
        DateTime* getLocalDatetime() override
        {
            DateTime *dt = new DateTime;
            dt->year = 1900;
            dt->month = 1;
            dt->day = 1;
            dt->hours = 0;
            dt->minutes = 0;
            dt->seconds = 0;
            return dt;
        };        
};

#endif