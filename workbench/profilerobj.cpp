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
#ifndef SCOTT_WORKBENCH_PROFILER_H
#define SCOTT_WORKBENCH_PROFILER_H
#define PROFILER_VERSION 2

#define assert(x) if(!(x)){throwAssertion(#x,__FILE__,__FUNCTION__,__LINE__);}

#define PROFILE_FUNC ProfilerCallObj counter##__COUNTER__(__FILE__,__FUNCTION__,__LINE__);
#define PROFILE(x) ProfilerCallObj counter##__COUNTER__((x),__FILE__,__FUNCTION__,__LINE__);
#define warn() std::cout

class ProfilerCallObj
{
public:
    ProfilerCallObj();

    ProfilerCallObj( const char* file, const char* func, int line );

    ProfilerCallObj( const char* name,
                     const char* file,
                     const char* func,
                     int line );

    ~ProfilerCallObj();
private:
    void enter();

    void leave();

    static ProfilerCallObj * S_ParentObj;   // auto_ptr!
    static ProfilerCallObj   S_RootObj;
    static int               S_Depth;
    static bool              S_InitOnce;

    const char * m_name;
    const char * m_file;
    const char * m_func;
    int m_line;
};

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdlib.h>

ProfilerCallObj * ProfilerCallObj::S_ParentObj = new ProfilerCallObj();
int ProfilerCallObj::S_Depth                   = 0;

ProfilerCallObj::ProfilerCallObj()
    : m_name( NULL ),
      m_file( NULL ),
      m_func( NULL ),
      m_line( -1 )
{
    // don't do anything
}

ProfilerCallObj::ProfilerCallObj( const char* file,
                                  const char* func,
                                  int line )
    : m_name( func ),
      m_file( file ),
      m_func( func ),
      m_line( line )
{
    enter();
}

ProfilerCallObj::ProfilerCallObj( const char* name,
                                  const char* file,
                                  const char* func,
                                  int line )
    : m_name(name),
      m_file(file),
      m_func(func),
      m_line(line)
{
    enter();
}

ProfilerCallObj::~ProfilerCallObj()
{
    leave();
}

void ProfilerCallObj::enter()
{
    for ( int i = 0; i < S_Depth; ++i )
    {
        std::cout << "\t";
    }

    std::cout << "Entering: " << m_name << std::endl;
    S_Depth++;
}

void ProfilerCallObj::leave()
{
    S_Depth--;
    
    for ( int i = 0; i < S_Depth; ++i )
    {
        std::cout << "\t";
    }
    std::cout << "Leaving: " << m_name << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

#include <unistd.h>

void delay( int usec )
{
    usleep( usec );
}

TEST(Profiler,FirstTest)
{
    assert( false == true );
}

