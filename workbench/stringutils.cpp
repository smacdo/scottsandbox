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

typedef std::vector<std::string> StringList;

/**
 * Checks if the input string has the given prefix. Will return true if
 * it does, false otherwise
 */
bool hasPrefix( const std::string& input, const std::string& prefix );

/**
 * Checks in the input string has the given suffix. Wil lreturn true if
 * it does, false otherwise
 */
bool hasSuffix( const std::string& input, const std::string& suffix );

/**
 * Returns a string iterator pointing to the first non-whitespace character
 * in the given string
 */
std::string::const_iterator skipStartingWhitespace(
                               std::string::const_iterator begin,
                               std::string::const_iterator end );

/**
 * Converts the path to a correct path for the system
 *  (Shouldn't this be in fileutils?)
 */
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
 * Returns the length (number of characters) in the given string. This
 * is a wrapper around string::length()
 */
size_t length( const std::string& input );

/**
 * Returns the length (number of characters) in the given cstring
 */
size_t length( const char * input );

/**
 * Ensures that the given input string conforms to a maximum number of
 * characters per line. This method will insert newlines whenever the
 * number of characters exceeds maxLineLen. (Default is 80)
 */
std::string wordWrap( const std::string& input, int maxLineLen=80 );

/**
 * Returns the number of words found in the input string. A word is
 * defined to be a consecutive run of letters [A-Za-z_]+.
 */
size_t wordCount( const std::string& input );

std::string pad( const std::string& text, size_t len, char pad=' ' );

/**
 * Returns the input string reversed, so that each character is in the
 * inverse position of the input.
 */
std::string reverse( const std::string& input );

/**
 * Trims whitespace and newlines from the beginning and end of the input
 * string
 */
std::string trim( const std::string& input, bool left=true, bool right=true );

/**
 * Trims whitespace and newlines from the beginning of the input string
 */
std::string ltrim( const std::string& input );

/**
 * Trims whitespace and newlines frmo the end of the input string
 */
std::string rtrim( const std::string& input );

/**
 * Converts the input string to have all uppercase characters. Does not
 * modify non-alphabetic characters
 */
std::string toUpper( const std::string& input );

/**
 * Converts the input string to have all lowercase characters. Does not
 * modify non-alphabetic characters
 */
std::string toLower( const std::string& input );

/**
 * Replace searches the input string for all occurences of 'findWhat',
 * and will replace each one with 'replaceWith'. It will do this up to
 * 'limit' times.
 */
std::string replace( const std::string& input,
                     const std::string& findWhat,
                     const std::string& replaceWith,
                           size_t limit=0 );

/**
 * Erase will erase all occurences of 'findWhat' that occur in 'input',
 * up to a maximum of 'limit' times.
 */
std::string erase( const std::string& input,
                   const std::string& findWhat,
                         size_t limit=0 );

/**
 * Returns an STL string containing the stringified version of the
 * input value
 */
template<typename T>
std::string toString( const T& input );

/**
 * Returns an array of strings that result in splitting the string along
 * the given delimitter characters
 */
StringList split( const std::string& input,
                   const std::string& delims );

/**
 * Splits a string into multiple pieces depending on the given delimter
 * chars
 */
size_t split( const std::string& input,
                    std::vector<std::string>& output,
              const std::string& delims );

/**
 * Returns a rot13 "encrypted" version of the string
 */
std::string rot13( const std::string& input );

/**
 * Parses an input string and attempts to convert it to the target output
 * type. Function will return false if it false if it fails to do so, true
 * otherwise
 */
template<typename T>
bool parseString( const std::string& input, T& output );

/**
 * Returns the correct suffix when using the given value. For example,
 * it will return 'rd' for 3 and 'nd' for 22.
 */
const char* nth( long number );

/**
 * Returns a string of zeros and ones that represent the binary layout
 * of the given array of bytes
 */
std::string bin2str( uint8_t* data, size_t len );

/**
 * Returns a string of zeros and ones that represent the binary layout
 * of the given object
 */
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
#include <cstring>

#include <algorithm>

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

size_t length( const std::string& input )
{
    return input.size();
}

size_t length( const char * input )
{
    return strlen( input );
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

std::string wordWrap( const std::string& input, int maxlen )
{
    maxlen = maxlen;        // shut compiler up
    return input;
}

size_t wordCount( const std::string& input )
{
    return input.size();
}

std::string pad( const std::string& text, size_t len, char pad )
{
    len = len; pad = pad;
    return text;
}

std::string reverse( const std::string& input )
{
    return std::string( input.rbegin(), input.rend() );
}

char rot13( char input )
{
    char  cap = input & 32;
    
    input &= ~cap;
    input = ((input >= 'A' && (input <= 'Z')) ?
                ((input - 'A' + 13) % 26 + 'A') : input );

    return (input | cap);
}

std::string rot13( const std::string& input )
{
    std::string output( input );
    size_t len = input.size();

    for ( size_t i = 0; i < len; ++i )
    {
        output[i] = input[i];
    }

    return output;
}

std::vector<std::string> split( const std::string& input,
                                const std::string& delims )
{
    std::vector<std::string> output;
    split( input, output, delims );

    return output;
}

size_t split( const std::string& input,
                    StringList&  output,
              const std::string& delims )
{
    std::string::size_type pos  = 0, lastPos = 0;
    std::string::size_type last = input.size();
    size_t startCount           = output.size();
    const bool trimEmpty        = true;

    while ( pos < last )
    {
        pos = input.find_first_of( delims, lastPos );

        if ( pos == std::string::npos )
        {
            // no more occurrences
            pos = input.length();       // set position to end of string
        }

        if ( pos != lastPos || !trimEmpty )
        {
            // So long as there is something to capture, or if the user
            // wants to capture _everything_ then grab the last capture
            // group formed by [lastPos,pos).
            output.push_back( std::string(input, lastPos, pos-lastPos) );
        }

        lastPos = pos + 1;
    }

    return ( output.size() - startCount );
}

/**
 * A powerful genericized split
 */
template<typename ContainerT>
size_t split( const std::string& input,
                    ContainerT&  output,
              const std::string& delims )
{
    std::string::size_type pos  = 0, lastPos = 0;
    std::string::size_type last = input.size();
    size_t startCount           = output.size();
    const bool trimEmpty        = true;

    while ( pos < last )
    {
        pos = input.find_first_of( delims, lastPos );

        if ( pos == std::string::npos )
        {
            // no more occurrences
            pos = input.length();       // set position to end of string
        }

        if ( pos != lastPos || !trimEmpty )
        {
            // So long as there is something to capture, or if the user
            // wants to capture _everything_ then grab the last capture
            // group formed by [lastPos,pos).
            output.push_back(
                ContainerT::value_type( input, lastPos, pos-lastPos )
            );
        }

        lastPos = pos + 1;
    }

    return ( output.size() - startCount );
}

std::string trim( const std::string& input, bool ltrim, bool rtrim )
{
    std::string::size_type left  = 0;
    std::string::size_type right = input.size();
    
    if ( ltrim )
    {
        std::string::size_type t = input.find_first_not_of(" \t\r\n");
        left = ( t != std::string::npos ? t : input.size() );
    }

    if ( rtrim )
    {
        std::string::size_type t = input.find_last_not_of(" \t\r\n");
        right = ( t != std::string::npos ? t : 0 );
    }

    return std::string( input, left, (right-left+1) );
}

std::string ltrim( const std::string& input )
{
    return trim( input, true, false );
}

std::string rtrim( const std::string& input )
{
    return trim( input, false, true );
}

std::string::value_type internalToUpper( std::string::value_type v )
{
    return toupper( static_cast<char>(v) );
}

std::string::value_type internalToLower( std::string::value_type v )
{
    return tolower( static_cast<char>(v) );
}

std::string toUpper( const std::string& input )
{
    std::string output( input ); // presize?
    std::transform( input.begin(), input.end(), output.begin(),
                    internalToUpper );

    return output;
}

std::string toLower( const std::string& input )
{
    std::string output( input );
    std::transform( input.begin(), input.end(), output.begin(),
                    internalToLower );

    return output;
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

// reverse
TEST(StringUtils,ReverseAString)
{
    EXPECT_EQ( "ftwseonho", reverse(std::string("ohnoeswtf")) );
}

TEST(StringUtils,ReverseOneCharDoesNothing)
{
    EXPECT_EQ( "a", reverse("a") );
}

TEST(StringUtils,ReverseEmptyDoesNothing)
{
    EXPECT_EQ( "", reverse("") );
}

TEST(StringUtils,ReversePalindromIsSame)
{
    EXPECT_EQ( "racecar", reverse("racecar") );
}

// trim
TEST(StringUtils,TrimEmptyIsEmpty)
{
    EXPECT_EQ( "", trim("") );
}

TEST(StringUtils,TrimAllWhitespaceIsEmpty)
{
    EXPECT_EQ( "", trim(" \t \r    \t\n\r \r\n   \n") );
}

TEST(StringUtils,TrimWhitespaceAtEnds)
{
    EXPECT_EQ( "hai", trim("    hai       ") );
}

TEST(StringUtils,TrimWithNothingNeededToTrim)
{
    EXPECT_EQ( "hai", trim("hai") );
}

TEST(StringUtils,TrimNothing)
{
    EXPECT_EQ("   hahaha   ", trim("   hahaha   ", false, false) );
}

TEST(StringUtils,TrimWhitespaceOnlyAtEnds)
{
    EXPECT_EQ( "ha i", trim("ha i") );
    EXPECT_EQ( "ha i", trim("  ha i") );
    EXPECT_EQ( "ha i", trim("ha i  ") );
    EXPECT_EQ( "ha i", trim("  ha i ") );
}

TEST(StringUtils,TrimOnlyLeftWhitespace)
{
    EXPECT_EQ( "te s  t  ", trim("   te s  t  ", true, false) );
}

TEST(StringUtils,TrimOnlyRightWhitespace)
{
    EXPECT_EQ( "   te s  t", trim("   te s  t  ", false, true) );
}

// To upper
TEST(StringUtils,ToUpper)
{
    EXPECT_EQ("OH HELLO THERE!!1", toUpper("oH hELlo there!!1"));
}

// To lower
TEST(StringUtils,ToLower)
{
    EXPECT_EQ("oh hello there!!1", toLower("OH hELlo there!!1"));
}

// length
TEST(StringsUtils,LengthEmptyStringIsZero)
{
    EXPECT_EQ( (size_t)0, length(std::string("")) );
    EXPECT_EQ( (size_t)0, length("") );
}

TEST(StringUtils,LengthnormalString)
{
    EXPECT_EQ( (size_t)11, length(std::string("hello world")) );
    EXPECT_EQ( (size_t)11, length("hello world") );
}

// wordwrap
TEST(StringUtils,WordWrap)
{
}

// wordcount
TEST(StringUtils,WordCount)
{
    /*
    EXPECT_EQ( (size_t)0, std::string("") );
    EXPECT_EQ( (size_t)1, std::string("        ONE") );
    EXPECT_EQ( (size_t)5, std::string("one two three four five") );
    EXPECT_EQ( (size_t)5, std::string("  one . two ^.X three four five."));
    EXPECT_EQ( (size_t)3, std::string("   three one     two    ") );
    */
}
