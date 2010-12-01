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

#include <stdio.h>
#include <stdint.h>

const int CRC_TABLE_SIZE = 256;

void dump_table( uint32_t polynomial )
{
    const int ELEMENT_SIZE = 12;
    const int PER_ROW = 80 / ELEMENT_SIZE;

    for ( int i = 0; i < CRC_TABLE_SIZE; ++i )
    {
        uint32_t crc = i;

        for ( int j = 0; j < 8; ++j )
        {
            if ( crc & 1 )
            {
                crc = (crc >> 1) ^ polynomial;
            }
            else
            {
                crc >>= 1;
            }
        }

        printf(" %#010x", crc);

        // end of line?
        if ( i != CRC_TABLE_SIZE - 1 )
        {
            printf(",");
        }

        if ( ((i+1) % PER_ROW == 0) || (i == CRC_TABLE_SIZE - 1) )
        {
            printf("\n");
        }
    }
}


int main( int , char** )
{
    dump_table( 0xEDB88320 );
}
