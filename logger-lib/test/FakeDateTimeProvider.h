// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_FAKE_DATETIME_PROVIDER_H_
#define _HC_LIB_FAKE_DATETIME_PROVIDER_H_

#include "providers/IDateTimeProvider.h"

class FakeDateTimeProvider : public IDateTimeProvider
{
    public:
        FakeDateTimeProvider() = default;

        DateTime getLocalDatetime() const override
        {
            DateTime dt;
            dt.year = 1900;
            dt.month = 1;
            dt.day = 1;
            dt.hours = 0;
            dt.minutes = 0;
            dt.seconds = 0;
            return dt;
        };
};

#endif
