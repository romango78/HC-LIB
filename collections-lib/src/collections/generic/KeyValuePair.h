// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _KEY_VALUE_PAIR_H_
#define _KEY_VALUE_PAIR_H_

/// @brief A key and a value. Does not own pointed-to data.
/// @tparam TKey Key type. Copied into the pair.
/// @tparam TValue Value type. Copied into the pair.
template<typename TKey, typename TValue>
class KeyValuePair
{
    private:
        TKey m_key;
        TValue m_value;
    public:        
        /// @brief Constructs a key-value pair.
        /// @param t_key The key.
        /// @param t_value The value.
        KeyValuePair(const TKey &t_key, const TValue &t_value)
            : m_key(t_key), m_value(t_value)
        {}

        /// @brief Returns the key.
        /// @return The key.
        const TKey& getKey() const
        {
            return m_key;
        }

        /// @brief Returns the value.
        /// @return The value.
        const TValue& getValue() const
        {
            return m_value;
        };
};

#endif