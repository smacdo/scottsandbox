// Copyright (c) 2010 Scott MacDonald.
// Code released into public domain with no license attached. Use as you wish

/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_NULLSTREAM_H
#define SCOTT_WORKBENCH_NULLSTREAM_H
#define NULLSTREAM_VERSION 2

#include <ostream>

struct NullBuffer : std::streambuf
{
    int overflow( int c );
};

struct NullStream : std::ostream
{
    NullBuffer m_sbuf;
    NullStream();
};
#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
int NullBuffer::overflow( int c )
{
    return traits_type::not_eof(c);
}

NullStream::NullStream()
    : std::ios( &m_sbuf ),
      std::ostream( &m_sbuf )
{
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(NullStream,DoesItWorkTest)
{
    NullStream ns;
    ns << "oh hai";
}
