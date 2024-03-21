// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_CLONEABLE_H_
#define _I_CLONEABLE_H_

template <class T>
class ICloneable
{
    public:
        ICloneable() = default;
        virtual ~ICloneable() = default;

        virtual T* clone() const = 0;
};

#endif