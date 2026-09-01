// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_I_CLONEABLE_H_
#define _HC_LIB_I_CLONEABLE_H_

/// @file ICloneable.h
/// Defines a generic cloneable abstraction.

/// @brief Supports creating a copy of an instance.
/// @tparam T The type produced by clone(), usually the most derived interface or class.
template <class T>
class ICloneable
{
    public:
        ICloneable() = default;
        virtual ~ICloneable() = default;

        /// @brief Creates a new copy of this instance.
        /// @return A pointer to the cloned object. The caller owns the pointer and must delete it.
        virtual T* clone() const = 0;
};

#endif
