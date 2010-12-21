#include <iostream>
#include <string>

#include <time.h>
#include <cassert>

struct TimeT
{
    TimeT( const time_t& rawtime, const tm* ti )
        : rawtime( rawtime ), timeinfo( ti )
    {
        assert( rawtime  >= 0 );
        assert( timeinfo != NULL );
    }

    const time_t rawtime;
    const tm*    timeinfo;
};

enum TimeFormat
{
    TF_Relative,
    TF_Short,
    TF_Long,
    TF_DateOnly,
    TF_TimeOnly
};

TimeT appGetLocaltime()
{
    time_t rawtime;
    tm *   timeinfo;

    // Get the raw time, convert it to local time and return the result
    time( &rawtime );
    timeinfo = localtime( &rawtime );

    return TimeT( rawtime, timeinfo );
}

TimeT appGetGMTTime()
{
    time_t rawtime;
    tm *   timeinfo;

    // Get the raw time, convert it to GMT time and return the result
    time( &rawtime );
    timeinfo = gmtime( &rawtime );

    return TimeT( rawtime, timeinfo );
}

std::string formatTimeString( const TimeT& thetime, const char * format )
{
    assert( format  != NULL );

    const int buffer_length = 128;
    char buffer[buffer_length];

    // Format the time to the c-string buffer
    int len = strftime( buffer, buffer_length, format, thetime.timeinfo );
    assert( len < buffer_length );      // sanity, even though never happens

    // Return the c-string buffer as a STL string
    return std::string( buffer );
}

std::string formatTimeString( const tm *time, TimeFormat format )
{
    assert( time != NULL );

//    const int buffer_length = 128;
//    char buffer[buffer_length];

    switch ( format )
    {
    /*    case TF_RELATIVE:
            break;

        case TF_Short:
            break;

        case TF_Long:
            break;

        case TF_DateOnly:
            break;

        case TF_TimeOnly:
            break;*/

        default:
            assert(false && "Invalid time format specified");
    }
}
