// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_STREAM_H_
#define _I_STREAM_H_

#define IO_ERROR_STREAM_NOTCREATED 0x0100

template<typename T>
class IStream
{
    public:
        virtual ~IStream() = default;

        virtual void startRead() {};
        virtual T read() 
        { 
            return T();
        };
        virtual void endRead() {};

        virtual void startWrite() {};
        virtual void write(T data) {};
        virtual void endWrite() {};

        virtual bool canRead() 
        {
            return false;
        };
        virtual bool canWrite() 
        {
            return false;
        };

};

#endif