// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// This file is part of the Logger Library v1.0.1

#ifndef _I_DATETIME_PROVIDER_H_
#define _I_DATETIME_PROVIDER_H_

struct DateTime
{
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    int seconds;
};

class IDateTimeProvider 
{
    public:
        virtual DateTime* getLocalDatetime() 
        { 
            return new DateTime;
        };
        virtual ~IDateTimeProvider() = default;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif