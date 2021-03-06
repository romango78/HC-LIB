// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// This file is part of the Logger Library v1.0.1

#ifndef _I_LOG_PERSISTER_H_
#define _I_LOG_PERSISTER_H_

class ILogPersister 
{
    public:        
        virtual ~ILogPersister() = default;
        virtual void write(const char t_character) = 0;
};

#endif