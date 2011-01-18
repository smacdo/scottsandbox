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
#include <string>
#include <cassert>
#include <iostream>

#include "logging.h"
#include "markov.h"
#include "markovdata.h"
#include "elementnode.h"
#include "markovfactory.h"

MarkovData MarkovFactory::createFromLetters( 
        size_t depth,
        const std::string& input )
{
    MarkovData  data( depth-1 );        // bug :(
    MarkovChain currentChain;

    // pre-size our chain
    currentChain.resize( depth );

    // walk through the string, and create as many chain-pairs as possible
    for ( size_t i = 0; i <= (input.size()-depth); ++i )
    {
        std::cout << "START: ";

        // create a chain
        for ( size_t c = 0; c < depth; ++c )
        {
            currentChain[c] = input[i + c];
            std::cout << currentChain[c] << " ";
        }

        std::cout << std::endl;

        // add it
        data.insert( currentChain );
    }

    return data;
}
