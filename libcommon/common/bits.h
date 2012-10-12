/*
 * Copyright 2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_COMMON_BITS_H
#define SCOTT_COMMON_BITS_H

/**
 * Collection of utility functions to help when manipulating individual
 * bits in a value
 */
namespace BitUtils
{
    /**
     * Set a bit to one
     */
    template<typename T>
    inline void set( T& value, size_t pos )
    {
        value |= 1 << pos;
    }

    /**
     * Set a bit to zero
     */
    template<typename T>
    inline void clear( T& value, size_t pos )
    {
        value &= ~( 1 << pos );
    }

    /**
     * Flip a bit (Change zero to one, or one to zero)
     */
    template<typename T>
    inline void flip( T& value, size_t pos )
    {
        value ^= ( 1 << pos );
    }

    /**
     * Get a bit
     */
    template<typename T>
    inline bool test( T value, size_t pos )
    {
        return static_cast<bool>( value & ( 1 << pos ) );
    }
}

#endif
