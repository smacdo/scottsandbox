#include <common/time.h>
#include <cassert>

const double Time::TimeDelta = 0.000001;

Time::Time()
    : m_seconds( 0.0 )
{
}

Time::Time( const Time& t )
    : m_seconds( t.m_seconds )
{
}

Time::Time( int seconds )
    : m_seconds( seconds )
{
    assert( seconds >= 0 && "Time cannot consist of negative seconds" );
}

Time::Time( float seconds )
    : m_seconds( static_cast<double>(seconds) )
{
    assert( seconds >= 0.0f && "Time cannot consist of negative seconds" );
}

Time::Time( double seconds )
    : m_seconds( seconds )
{
    assert( seconds >= 0.0f && "Time cannot consist of negative seconds" );
}

Time& Time::operator = ( const Time& rhs )
{
    m_seconds = rhs.m_seconds;
}

bool Time::operator == ( const Time& rhs ) const
{
    return ( m_seconds + TimeDelta >= rhs.m_seconds &&
             m_seconds - TimeDelta <= rhs.m_seconds );
}

bool Time::operator != ( const Time& rhs ) const
{
    return !( *this == rhs );
}

Time Time::operator + ( const Time& rhs ) const
{
    return Time( m_seconds + rhs.m_seconds );
}

Time& Time::operator += ( const Time& rhs )
{
    m_seconds += rhs.m_seconds;
    return *this;
}

Time Time::operator - ( const Time& rhs ) const
{
    assert( m_seconds >= rhs.m_seconds );
    return Time( m_seconds - rhs.m_seconds );
}

Time& Time::operator -= ( const Time& rhs )
{
    assert( m_seconds >= rhs.m_seconds );
    
    m_seconds -= rhs.m_seconds;
    return *this;
}
