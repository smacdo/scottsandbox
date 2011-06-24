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

#include <iostream>

const int N = 624;
const int M = 397;
const int MATRIX_A = 0x9908b0df;    // constant vector A
const int UPPER_MASK = 0x80000000;  // most significant w-r bits
const int LOWER_MASK = 0x7fffffff;  // least significant r bits

static unsigned long mt[N];         // state vectory array
static int mti = N + 1;             // mti == N+1 means mt[N] not init'd

void init_genrand( unsigned long seed )
{
    mt[0] = seed & 0xffffffffUL;

    for ( mti = 1; mti < N; ++mti )
    {
        mt[mti]  = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        mt[mti] &= 0xffffffffUL;    // for 64 bit
    }
}

void init_by_array( unsigned long init_key[], int key_length )
{
    int k = ( N > key_length ? N : key_length );
    int i = 0;
    int j = 0;

    init_genrand( 19650218UL );

    // temp to kill warnings
    k = i = j;
    init_key[0] = 0;
}
