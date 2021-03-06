// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_STREAM_H_
#define _I_STREAM_H_

#include "ioerrdef.h"
#include "ICloneable.h"

#define UNDEF_MODE 0
#define READ_MODE 1
#define WRITE_MODE 2

enum StreamMode
{    
    Read = READ_MODE,
    Write = WRITE_MODE
};

template<typename T>
class IStream : public ICloneable<IStream<T>>
{
    public:
        IStream() = default;
        virtual ~IStream() = default;

        virtual void begin(const StreamMode t_mode) = 0;
        virtual T read() = 0;          
        virtual void write(const T t_data) = 0;
        virtual void end() = 0;

        virtual uint8_t getState() = 0;

        virtual bool canRead() = 0;
        virtual bool canWrite() = 0;
        virtual bool hasError() = 0;

        virtual err_t getLastError() = 0;
};

#endif