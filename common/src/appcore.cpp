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
#include "appcore.h"

#include <iostream>
#include <cstdlib>

void raiseFatalError( const char * message,
                      const char * reason,
                      const char * data,
                      const char * file,
                      const char * func,
                      int line )
{
    std::cout << "---------------------------------- FATAL ERROR --------------"
                 "--------------------" << std::endl;
    std::cout << message << std::endl;
    std::cout << std::endl;
    std::cout << " Reason    : " << reason << std::endl;
    std::cout << " Expression: " << expr   << std::endl;
    std::cout << " Location  : " << func
              << " (line: "      << line
              << ", "            << file
              << ")"  << std::endl;
    std::cout << "-------------------------------------------------------------"
                 "--------------------" << std::endl;

    exit(-1);
}


void throwAssertion( const char * expr,
                     const char * file,
                     const char * func,
                     int line )
{
    raiseFatalError( "The running application has thrown a fatal uncaught "
                     "exception, and will be terminated.",
                     "Uncaught exception",
                     expr,
                     file,
                     func,
                     line );
}

