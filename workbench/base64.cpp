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
#ifndef SCOTT_WORKBENCH_BASE64_H
#define SCOTT_WORKBENCH_BASE64_H
#define BASE64_VERSION 2

#include <string>
#include <stdint.h>

int base64_decode_len( size_t inputlen );

/**
 * Takes a three byte memory block, and spits out 4 8-bit ASCII encoded
 * bytes. Length is the actual number of used bytes in the input block
 */
void base64_encode_block( uint8_t *in, uint8_t *out, int len );

/**
 * Decode 4 '6-bit' characters into 3 8-bit binary bytes
 */
void base64_decode_block( const uint8_t *in, uint8_t *out );

void base64_decode( const uint8_t *in,  size_t in_len,
                          uint8_t *out, size_t out_len );

void base64_decode( const std::string& source );

std::string base64_encode( uint8_t * bytes, size_t length );

std::string base64_encode( const std::string& result );

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <cassert>
#include <stdint.h>

inline int base64_decode_len( size_t inputlen )
{
    return 3 * inputlen / 4;
}

void base64_encode_block( uint8_t *in, uint8_t *out, int len )
{
   static const char * T = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    if ( len > 2 )
    {
        out[0] = T[(in[0] >> 2 )];
        out[1] = T[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
        out[2] = T[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)];
        out[3] = T[(in[2] & 0x3f)];
    }
    else if ( len > 1 )
    {
        out[0] = T[(in[0] >> 2 )];
        out[1] = T[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
        out[2] = T[((in[1] & 0x0f) << 2)];
        out[3] = '=';
    }
    else 
    {
        out[0] = T[(in[0] >> 2 )];
        out[1] = T[((in[0] & 0x03) << 4)];
        out[2] = '=';
        out[3] = '=';
    }
}

void base64_decode_block( const uint8_t *in, uint8_t *out )
{
    out[0] = ( in[0] << 2            | in[1] >> 4 );
    out[1] = ( in[1] << 4            | in[2] >> 2 );
    out[2] = ( ((in[2] << 6) & 0xc0) | in[3] );
}

void base64_decode( const uint8_t *in,  size_t in_len,
                          uint8_t *out, size_t out_len )
{
    assert( in  != 0 );
    assert( in_len > 0 && in_len % 4 == 0 );

    assert( out != 0 );
    assert( out_len > 0 );
    assert( out_len/3 >= in_len / 4 );

    for ( size_t i = 0, j = 0; i < in_len; i += 4, j += 3 )
    {
        base64_decode_block( in + i, out + j );

//        std::cout << i+0 << ": " << (int) in[i+0] << " --> " << (int) out[j+0] << std::endl
//                  << i+1 << ": " << (int) in[i+1] << " --> " << (int) out[j+1] << std::endl
//                  << i+2 << ": " << (int) in[i+2] << " --> " << (int) out[j+2] << std::endl
//                  << i+3 << ": " << (int) in[i+3] << std::endl << std::endl;
      
    }
}

/*void base64_decode( const std::string& source, uint8_t *out, size_t len )
{
    assert( sizeof(source[0]) == sizeof(uint8_t) );

    base64_decode( reinterpret_cast<const uint8_t*>(source.c_str()),
                   source.size(),
                   out,
                   len
    );
}*/

void base64_decode( const std::string& )
{
}

std::string base64_encode( uint8_t * bytes, size_t length )
{
    // Sanity checks
    assert( bytes != 0 );
    assert( length > 0 );

    // Allocate a char buffer to avoid dealing with STL string sizing
    size_t   bufflen = (length + 2 - ((length + 2) % 3)) * 4 / 3 + 1;
    uint8_t * output = new uint8_t[bufflen];

    output[bufflen-1] = 0;

    // Iterate through the string, and convert each character to its
    // base 64 equivilant.
    //
    // Normally we'd just use the c_str() pointer to avoid copying
    // from in/out to the string, but we cannot always be sure that
    // STL string is 8 characters.
    for ( size_t i = 0,j=0; i < length; i += 3, j += 4 )
    {
        base64_encode_block( bytes + i, output + j, length - i );

        std::cout << i+0 << ": " << (int) bytes[i+0] << " --> " << (int) output[j+0] << std::endl
                  << i+1 << ": " << (int) bytes[i+1] << " --> " << (int) output[j+1] << std::endl
                  << i+2 << ": " << (int) bytes[i+2] << " --> " << (int) output[j+2] << std::endl
                  << " " << "     "                  << " --> " << (int) output[j+3] << std::endl
                  << std::endl;
    }

    // All done, return the base64 encoded string
    const char * temp  = reinterpret_cast<char*>( output );
    std::string result = std::string( temp );

    delete[] output;
    return result;
}


std::string base64_encode( const std::string& result )
{
    assert( sizeof(char) == sizeof(uint8_t) );

    return base64_encode( (uint8_t*) result.c_str(), result.size() );
}

/*int main ( int argc, char* argv[] )
{
    //
    // Decode it
    //
    size_t    len    = base64_decode_len( output.size() );
    uint8_t * buffer = new uint8_t[ len ];
    base64_decode( (const uint8_t*) &output[0], output.size(), buffer, len );

    std::string decoded( (const char*) buffer );

    std::cout << "'" << output << "' ==> '" << decoded << "'" << std::endl;
    delete buffer;
}*/

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(Base64,EncodeHelloWorld)
{
    std::string input  = "hello world";
    std::string output = base64_encode( input );

    EXPECT_EQ( "aGVsbG8gd29ybGQ=", output );
}

TEST(Base64,WikipediaExample)
{
    std::string input = "Man is distinguished, not only by his reason, "
        "but by this singular passion from other animals, which is a lust "
        "of the mind, that by a perseverance of delight in the continued "
        "and indefatigable generation of knowledge, exceeds the short "
        "vehemence of any carnal pleasure.";

    std::string output = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpc"
        "yByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciB"
        "hbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZ"
        "XJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF"
        "0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9yd"
        "CB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4K";

    EXPECT_EQ( output, base64_encode( input ) );
}

TEST(Base64,DecodeHelloWorld)
{
//    std::string input  = "aGVsbG8gd29ybGQ=";
//    std::string output = base64_decode( input );

//    EXPECT_EQ( "hello world", output );
}
//#include <googletest/googletest.h>
