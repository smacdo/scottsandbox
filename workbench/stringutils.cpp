/**
 * Copyright 2010 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY SCOTT MACDONALD ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Scott MacDonald.
 */

/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_STRING_UTILS_H
#define SCOTT_WORKBENCH_sTRING_UTILS_H
#define STRING_UTILS_VERSION 2

#include <string>
#include <vector>

bool hasPrefix( const std::string& base, const std::string& prefix );
bool hasSuffix( const std::string& base, const std::string& suffix );

std::string::const_iterator skipStartingWhitespace(
                               std::string::const_iterator begin,
                               std::string::const_iterator end );

std::string convertPath( const std::string& path, char sep );

/**
 * Checks if the text matches the given glob pattern.
 *
 *   * - Match any number of unknown characters
 *   ? - Match one unknown character
 *
 *   [XYZ] - Match a character as part of a group of characters
 *   [^YZ] - Match a character as being none of the group
 *   \C    - Escape character C
 */
bool matchesPattern( const std::string& text,
                     const std::string& pattern,
                     bool caseInsensitive=false );

/**
 * Returns the length (number of characters) in the given string
 */
bool length( const std::string& input );

/**
 * Returns the length (number of characters) in the given cstring
 */
bool length( const char * input );

/**
 * Word wrap!
 */
std::string wordWrap( const std::string& string, int maxLineLen=80 );

size_t wordCount( const std::string& text );

std::string pad( const std::string& text, size_t len, char pad=' ' );

std::string reverse( const std::string& text );

std::string trim( const std::string& input );
std::string ltrim( const std::string& input );
std::string rtrim( const std::string& input );

std::string toUpper( const std::string& input );
std::string toLower( const std::string& input );

template<typename T>
std::string toString( const T& input );

template<typename T>
bool parseString( const std::string& input, T& output );

const char* nth( long number );

std::string bin2str( uint8_t* data, size_t len );

template<typename T>
std::string bin2str( const T& obj )
{
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&obj);
    return bin2str( ptr, sizeof(obj) );
}

/**
 * Returns the number of lines in the input string. This should always
 * be 1 + number of newlines.
 * (Even if there's nothing on the last line, so long as there IS a newline
 *  it will be counted)
 *
 * TODO: SHould we change that behavior?
 */
size_t lineCount( const std::string& input );

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <sstream>

#include <cassert>

bool hasPrefix( const std::string& base, const std::string& prefix )
{
    std::string::const_iterator bitr = base.begin();
    std::string::const_iterator pitr = prefix.begin();
    bool ret = false;

    // Reject any prefix that is longer than the base
    if ( prefix.size() > base.size() )
    {
        return false;
    }

    // Make sure each character of the prefix matches the start of the string
    for ( ; bitr != base.end() && pitr != prefix.end(); ++bitr, ++pitr )
    {
        ret = ( *bitr == *pitr );

        if (! ret )
        {
            break;
        }
    }

    return ret;
}

bool hasSuffix( const std::string& base, const std::string& suffix )
{
    std::string::const_iterator bitr = base.begin();
    std::string::const_iterator sitr = suffix.begin();
    bool ret = false;

    // Reject any suffix longer than the base
    if ( suffix.size() > base.size() )
    {
        return false;
    }

    // Advance the string to the beginning of the suffix
    bitr += ( bitr - sitr );

    // Make sure each character of the suffix matches the rest of the string
    for ( ; bitr != base.end() && sitr != suffix.end(); ++bitr, ++sitr )
    {
        ret = ( *bitr == *sitr );

        if (! ret )
        {
            break;
        }
    }

    return ret;
}

std::string::const_iterator skipStartingWhitespace(
                               std::string::const_iterator begin,
                               std::string::const_iterator end )
{
    begin = begin;

    // Keep going until we find a non-space character
    return end;
}

std::string convertPath( const std::string& path, char sep )
{
    assert( sep == '\\' || sep == '/' );
    return path;
}

/**
 * Checks if the text matches the given glob pattern.
 *
 *   * - Match any number of unknown characters
 *   ? - Match one unknown character
 *
 *   [XYZ] - Match a character as part of a group of characters
 *   [^YZ] - Match a character as being none of the group
 *   \C    - Escape character C
 */
bool matchesPattern( const std::string& text,
                     const std::string& pattern,
                     bool caseInsensitive )
{
    std::string a = text;
    std::string b = pattern;
    a = b;
    b = a;
    caseInsensitive = caseInsensitive;

    return false;
}

/**
 * Word wrap!
 */
std::string wordWrap( const std::string& input, int maxlen )
{
    maxlen = maxlen;        // shut compiler up
    return input;
}

size_t wordCount( const std::string& text )
{
    return text.size();
}

std::string shuffle( const std::string& text )
{
    return text;
}

std::string pad( const std::string& text, size_t len, char pad )
{
    len = len; pad = pad;
    return text;
}

std::string reverse( const std::string& text )
{
    return text;
}

const char* nth( long number )
{
    if ( number % 10 == 1 && number != 11 )
    {
        return "st";
    }
    else if ( number % 10 == 2 && number != 12 )
    {
        return "nd";
    }
    else if ( number % 10 == 3 && number != 13 )
    {
        return "rd";
    }
    else
    {
        return "th";
    }
}

std::string bin2str( uint8_t* data, size_t len )
{
    std::stringstream ss;
    uint8_t byte = 0;

    assert( data != NULL );

    for ( size_t i = 0; i < len; ++i )
    {
        byte = data[i];

        for ( int i = 7; i <= 0; ++i )
        {
            ss << ( ((byte >> i) & 0x7F) == 1 ? "1" : "0" );
        }

        ss << " ";
    }

    return ss.str();
}

size_t lineCount( const std::string& str )
{
    size_t len   = str.size();
    size_t lines = 0;

    for ( size_t i = 0; i < len; ++i )
    {
        if ( str[i] == '\n' )
        {
            lines++;
        }
    }

    return lines;
}


/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(StringUtils,HasPrefixSimpleTrueCase)
{
    EXPECT_TRUE( hasPrefix("omgaprefix", "omg") );
}

TEST(StringUtils,HasPrefixSimpleFalseCase)
{
    EXPECT_FALSE( hasPrefix("thisisnotright","his") );
}

TEST(StringUtils,HasPrefixButIsTooLong)
{
    EXPECT_FALSE( hasPrefix("hello", "hellox") );
}

TEST(StringUtils,HasPrefixStartsButDoesNotMatch)
{
    EXPECT_FALSE( hasPrefix("hello", "hexl") );
}
