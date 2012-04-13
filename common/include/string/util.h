#ifndef SCOTT_COMMON_STRING_UTIL_H
#define SCOTT_COMMON_STRING_UTIL_H

#include <string>
#include <ostream>

#include <stdint.h>

namespace String
{
    inline std::string toUpper( const std::string& );
}

/**
 * Appends a numeric value to the end of a string. Really use for
 * creating sequences of nameN.
 */
std::string makeSuffix( const std::string& base, long suffix );

/**
 * Takes an input string, and replaces each occurrence of 'findStr' with
 * 'replaceStr'. Returns the result of this replacement
 */
std::string replace( const std::string& input,
                     const std::string& findStr,
                     const std::string& replaceStr );

/**
 * Print the memory pointed to in hexadecimal form
 */
std::ostream& printHex( std::ostream& stream,
                        const uint8_t *ptr,
                        size_t len );

/**
 * Print the memory pointed to in binary form
 */
std::ostream& printBinary( std::ostream& stream,
                           const uint8_t *ptr,
                           size_t len );

/**
 * Print the given object's data in hexadecimal form
 */
template<typename T>
std::ostream& printHex( std::ostream& stream, const T& obj )
{
    const uint8_t *base = reinterpret_cast<const uint8_t*>(&obj);
    printHex( stream, base, sizeof(obj) );

    return stream;
}

/**
 * Print the given object's data in binary form
 */
template<typename T>
std::ostream& printBinary( std::ostream& stream, const T& obj )
{
    const uint8_t *base = reinterpret_cast<const uint8_t*>(&obj);
    printBinary( stream, base, sizeof(obj) );

    return stream;
}

bool loadfile( const std::string& filename, std::string& output );

#endif
