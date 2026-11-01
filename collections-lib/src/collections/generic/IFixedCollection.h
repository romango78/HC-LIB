// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_I_FIXED_COLLECTION_H_
#define _HC_LIB_I_FIXED_COLLECTION_H_

#include "IEnumerable.h"

/// @brief Defines a fixed-length generic collection.
/// @tparam T Item type stored in the collection.
template <typename T>
class IFixedCollection : public IEnumerable<T>
{
public:

    /// @brief Gets the number of elements contained in the collection.
    virtual uint32_t getCount() const = 0;

    /// @brief Gets the element at the specified index.
    /// @param t_index The zero-based index of the element to get. Must be less than getCount().
    /// @return A reference to the element at _t_index_.
    virtual T& operator[](const uint32_t t_index) = 0;

    /// @brief Removes every element from the collection.
    virtual void clear() = 0;
};

#endif
