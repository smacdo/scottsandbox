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
#ifndef MARKOV_LOGGING_H
#define MARKOV_LOGGING_H
#include <iostream>

struct nullstream : std::ostream
{
    struct nullbuf : std::streambuf
    {
        int overflow( int c )
        {
            return traits_type::not_eof(c);
        }
    } m_sbuf;

    nullstream()
        : std::ios( &m_sbuf ), std::ostream( &m_sbuf )
    {
    }
};

struct LogEntry
{
    LogEntry( std::ostream& stream_ )
        : stream( stream_ )
    {
    }

    ~LogEntry()
    {
        stream << std::endl;
    }

    template<typename T>
    std::ostream& operator << ( const T& v )
    {
        stream << v;
        return stream;
    }

    std::ostream& stream;
};

LogEntry ErrorLog();
LogEntry DebugLog();
LogEntry Log();

#define ERRORLOG ErrorLog()
#define DEBUGLOG DebugLog()
#define TRACELOG Log()
#define ENDLOG   


#endif
