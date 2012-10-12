/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_QUICK_PROFILER_H
#define SCOTT_WORKBENCH_QUICK_PROFILER_H

class SimpleTracer
{
public:
    SimpleTracer( const std::string& name, std::ostream& out = std::cout );
    ~SimpleTracer();

private:
    std::string mName;
    std::ostream& mOut;
    timeval mStartTime;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////

SimpleTracer::SimpleTracer( const std::string& name, std::ostream& out )
    : mName( name ),
      mOut( out )
      mStartTime( 0 )
{
    gettimeofday( &mStartTime, 0 );
}

SimpleTracer::~SimpleTracer()
{
    timeval now;
    gettimeofday( &now, 0 );

    double elapsed = now.tv_sec - mStartTime.tv_sec +
                     now.tv_usec / 1e6 - mStartTime.tv_usec / 1e6;

    mOut << "[TRACE] "         << mName
         << ", elapsed time: " << elapsed
         << "\n"
         << std::flush;
}


/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(Template,DefaultTest)
{
    EXPECT_TRUE( true );
}
