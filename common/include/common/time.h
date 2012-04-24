#ifndef SCOTT_COMMON_TIME
#define SCOTT_COMMON_TIME

#include <cstddef>      // size_t

/**
 * Class used to measure and keep track of arbitrary amounts of time.
 * The time class measures time in terms of seconds, and has no pre-defined
 * start time.
 *
 * (eg, 5 seconds does not mean 5 seconds since epoch. It is just
 *  "five seconds").
 */
class Time
{
public:
    Time();
    Time( const Time& t );
    explicit Time( int seconds );
    explicit Time( float seconds );
    explicit Time( double seconds );

    static Time fromNanoSeconds( size_t time );
    static Time fromMilliSeconds( size_t time );
    static Time fromMicroSeconds( size_t time );

    Time& operator = ( const Time& rhs );

    bool operator == ( const Time& rhs ) const;
    bool operator != ( const Time& rhs ) const;
    bool operator <  ( const Time& rhs ) const;
    bool operator <= ( const Time& rhs ) const;
    bool operator >  ( const Time& rhs ) const;

    Time  operator +  ( const Time& rhs ) const;
    Time& operator += ( const Time& rhs );

    Time  operator -  ( const Time& rhs ) const;
    Time& operator -= ( const Time& rhs );

    const static double TimeDelta;
private:
    double m_seconds;
};

#endif
