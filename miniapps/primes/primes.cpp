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
#include <cmath>
#include <memory.h>
#include <cstring>

bool isPrime( long );
void findPrimesUpTo( long );

int main( int , char** )
{
    findPrimesUpTo( 100000000 );
//    std::cout << ( isPrime( 29 ) ? "yes" : "no" ) << std::endl;
}

void findPrimesUpTo( long value )
{
    char * primesfield = new char[ value ];

    //
    // Set all but 0 and 1 to prime status
    //
    memset( primesfield, 1, sizeof(char) * value );
    primesfield[0] = 0;
    primesfield[1] = 1;

    long max = value + 1;
    long inc = 0;

    std::cout << " --- Generating table of primes" << std::endl;

    for ( long current = 2; current * current < max; current += inc )
    {
        //
        // Mark multiples
        //
        for ( long itr = (current * 2); itr < max; itr += current )
        {
            primesfield[itr] = 0;
        }

        //
        // Find distance to next prime, this is how much we'll increment
        // by
        //
        inc = 1;

        for ( long itr = current+1; itr < max; ++itr )
        {
            if ( primesfield[itr] == 1 )
            {
                break;
            }
            else
            {
                inc++;
            }
        }
    }

    //
    // Search for how many primes where generated
    //
    std::cout << " --- Generating results" << std::endl;

    long primesFound     = 0;
    long compositesFound = 0;
    long maxPrimeFound   = 0;

    for ( int i = 1; i < max; ++i )
    {
        if ( primesfield[i] == 1 )
        {
            // found a prime
            primesFound++;
            maxPrimeFound = i;
//            std::cout << i << ", ";
        }
        else
        {
            compositesFound++;
        }
    }

    std::cout << std::endl;

    //
    // All done
    //
    std::cout << "Found " << primesFound << " primes and "
              << compositesFound << " composites. Largest prime was "
              << maxPrimeFound
              << std::endl;
}

bool isPrime( long value )
{
    //
    // eliminate common values
    //
    if ( value % 2 == 0 || value % 3 == 0 || value % 5 == 0 )
    {
        return false;
    }

    //
    // Search all possible candidates to see if they are prime
    //
    bool prime = true;
    long max   = ceil( sqrt( value ) );

    for ( int i = 7; i < max; i += 2 )
    {
        if ( value % i == 0 )
        {
            prime = false;
            break;
        }
    }

    return prime;
}
