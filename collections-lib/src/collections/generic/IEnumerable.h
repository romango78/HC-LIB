// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_I_ENUMERABLE_H_
#define _HC_LIB_I_ENUMERABLE_H_

#include "IEnumerator.h"
#include <memory>

/// @brief Exposes an enumerator, which supports a simple iteration over a generic collection.
/// @tparam T Item type stored in the collection.
template <typename T>
class IEnumerable
{
public:
    /// @brief Returns an enumerator that iterates through the collection.
    /// @return An enumerator that can be used to iterate through the collection.
    /// @note The caller is responsible for deleting the enumerator.
    virtual std::unique_ptr<IEnumerator<T>> getEnumerator() const = 0;

    /// @brief Returns an enumerator that iterates through the collection.
    /// @return An enumerator that can be used to iterate through the collection.
    /// @note The caller is responsible for deleting the enumerator.
    virtual IEnumerator<T> getEnumerator() const = 0;
};

#endif