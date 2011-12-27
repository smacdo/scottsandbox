/**
 * Copyright 2011 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_COMMON_CRC32_H
#define SCOTT_COMMON_CRC32_H

#include <stdint.h>
#include <string>

uint32_t crc32( const uint8_t * pInput, size_t length );
uint32_t crc32( const std::string& input );

/**
 * Calculates the CRC32 checksum value for an arbitrary object. This method
 * will convert the object into a byte array as a way of finding the CRC32
 * value.
 *
 * \param  object  The object to checksum
 * \return         The CRC32 checksum value
 */
template<typename T>
uint32_t crc32( const T& object )
{
    const uint8_t * ptr = reinterpret_cast<const uint8_t*>( &object );
    return crc32( ptr, sizeof(object) );
}

#endif
