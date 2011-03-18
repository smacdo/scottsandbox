/**
 * math utils and whatnot
 */
#ifndef SCOTT_COMMON_MATH_UTIL
#define SCOTT_COMMON_MATH_UTIL

#include <cstdlib>
#include <cmath>
#include <limits>

#include <math/constants.h>

namespace Math
{
    static const float  FloatEpsilon  = 0.00001f;
    static const double DoubleEpsilon = 0.000000001;

    /**
     * Checks if two values of type T are equal, account for a small amount
     * of numeric inaccuracy. (Uses the floating point epsilon as the acceptable
     * range of inaccuracy)
     */
    template<typename T>
    inline bool equals_close( T a, T b )
    {
        return a == b;
    }

    /**
     * Checks if two floating point values are equal, accounting for a small
     * amount of numerical inaccuracy
     */
    template<>
    inline bool equals_close<float>( float a, float b )
    {
        return ( std::fabs( a - b ) < FloatEpsilon );
    }

    /**
     * Checks if two double values are equal, accounting for a small amount
     * of numeric inaccuracy
     */
    template<>
    inline bool equals_close<double>( double a, double b )
    {
        return ( std::fabs( a - b ) < DoubleEpsilon );
    }

    /**
     * Checks if a value is close to being zero, and accounting for a small
     * amount of numerical inaccuracy. This uses the floating point epsilon
     * as the acceptable range of inaccuracy
     */
    template<typename T>
    inline bool is_zero( T a )
    {
        return ( a == 0 );
    }

    template<>
    inline bool is_zero<float>( float a )
    {
        return ( a <= Math::FloatEpsilon && a >= -Math::FloatEpsilon );
    }

    template<>
    inline bool is_zero<double>( double a )
    {
        return ( a <= Math::DoubleEpsilon && a >= -Math::DoubleEpsilon );
    }

    /**
     * Checks if value is greater than zero. Uses floating point epsilon to allow
     * for an acceptable range of inaccuracy
     */
    template<typename T>
    inline bool greater_zero( T a )
    {
        return a > 0;
    }

    template<>
    inline bool greater_zero<float>( float a )
    {
        return ( a > Math::FloatEpsilon );
    }

    template<>
    inline bool greater_zero<double>( double a )
    {
        return ( a > Math::DoubleEpsilon );
    }

    /**
     * Takes a value, and clamps the value to be in the range [min,max].
     * If it falls out of this range it will be set to the closest value
     * in that range.
     */
    template<typename T>
    T clamp( const T& v, const T& min, const T& max ) 
    { 
        return ( v < max ? ( v > min ? v : min ) : max );
    }

    /**
     * Wraps a value to be within the range [0,max]. If the value is
     * larger than max, it will wrap to zero and use the remainder.
     * eg wrap( 34, 32 ) would return 2.
     */
    template<typename T>
    T wrap( const T& val, const T& max )
    {
        math_assert( val >= 0 && "Value to be wrapped must be positive" );
        return val % max;
    }

    /**
     * Performs a linear interpolation on the range [x,y] using s as the
     * linear interpolation value.
     */
    template<typename T,typename U>
    T lerp( const T& x, const T& y, const U& s )
    {
        return x + ( ( y - x ) * s);
    }

    /**
     * Performs a hermite linear interpolation on the range [a,b] using s as
     * the interpolvation percent (0 = a, 1=b).
     *
     * This is similiar to a linear interpolation, but the interpolation is
     * eased near the limits
     */
    template<typename T, typename U>
    T hermite( const T& a, const T& b, const U& s )
    {
        return lerp( a, b, s * s * ( 3.0f - 2.0f * s ) );
    }

    /**
     * Performs a sinusoidal interpolation on the range [a,b] using s as the
     * interpolation percent (0 = a, 1=b).
     *
     * The graph formed from [a,b] is similiar to a linear interpolatino, but
     * the end limit is eased in
    */
    template<typename T, typename U>
    T sinerp( const T& a, const T& b, const U& s )
    {
        return lerp( a, b, sin( s * Math::Pi * 0.5f ) );
    }

    /**
     * Performs a cosinusoidal interpolation on the range [a,b] using s as the
     * interpolation percent (0=a, 1=b).
     *
     * The graph formed from [a,b] is similiar to a linear interpolation, but
     * the start limit is eased in
     */
    template<typename T, typename U>
    T coserp( const T& a, const T& b, const U& s )
    {
        return lerp( a, b, cos( s * Math::Pi * 0.5f ) );
    }

    /**
     * Performs a "boing!" interpolation on the range [a,b] using s as the
     * interpolation percent (0=a, 1=b).
     *
     * This interpolation will cause the value to overshoot the upper limit,
     * and then converge.
     */
    template<typename T, typename U>
    T berp( const T& a, const T& b, const U& s )
    {
        T v = clamp( s, 0, 1 );
        v = sin(v * Math::Pi * (0.2f + 2.5f * s*s*s)) *
            pow(1.0f-s,2.2f);
    /*
    *    value = Mathf.Clamp01(value);
    *    value = (Mathf.Sin(value * Mathf.PI * (0.2f + 2.5f * value * value * value)) * Mathf.Pow(1f - value, 2.2f) + value) * (1f + (1.2f * (1f - value)));
    *    return start + (end - start) * value;
    */
    }

    /**
     * Snaps a value to the specified grid size. If the value is not
     * a multiple of grid, then value will be rounded to the nearest multiple
     * of grid.
     */
    template<typename T>
    T snap( const T& val, const T& grid )
    {
        T base = val % grid;
        
        if ( base < ( grid / 2 ) )
        {
            return val - base;
        }
        else
        {
            return ( grid - base ) + val;
        }
    }

    /**
     * Returns the passed value squared (x*x)
     */
    template<typename T>
    inline T square( const T& x )
    {
        return x * x;
    }

    /**
     * Returns the passed value cubed (x*x*x)
     */
    template<typename T>
    inline T cube( const T& x )
    {
        return x * x * x;
    }

    /**
     * Casts the passed value to the requested numeric type, and clamps
     * the passed value to be within the bounds of the cast type.
     *
     * ex: castAntClamp<int8_t>( 1000 ) == 255
     */
    template<typename T, typename U>
    U castAndClamp( const T& base )
    {
        if ( base > std::numeric_limits<U>::max() )
        {
            return std::numeric_limits<U>::max();
        }
        else if ( base < std::numeric_limits<U>::min() )
        {
            return std::numeric_limits<U>::min();
        }
        else
        {
            return static_cast<U>( base );
        }
    }

    /**
     * Generates a random integer and returns it. Completly at the whim
     * of the standard library's random number generator
     *
     * TODO Don't use the built in standard library. Be more creative
     */
    template<typename T>
    T random()
    {
        return static_cast<T>( rand() );
    }

    /**
     * Generates a random floating point number between 0 and 1
     * TODO: Verify range. Does it include or exclude boundry?
     */
    template<>
    float random<float>()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    /**
     * Generates a random double precision number between 0 and 1
     * TODO: Verify ranges. Does it include or exlude boundry?
     */
    template<>
    double random<double>()
    {
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }

    /**
     * Returns a random number in the range [min,max]
     */
    int random( int min, int max )
    {
        return min + ( random<int>() %  (max - min + 1) );
    }

    /**
     * Converts degrees into radians
     */
    static inline float RadiansToDegrees( float radians )
    {
        return radians * 180 / Math::Pi;
    }

    /**
     * Converts radian value into degrees
     */
    static inline float DegreesToRadians( float degrees )
    {
        return degrees * Math::Pi / 180;
    }

    /**
     * Clamps an angle given in degrees to its natural 0-360 range.
     */
    static inline float ClampAngle( float degrees )
    {
        float angle = degrees;

        if ( angle > 360.0f )
        {
            angle -= 360.0f * static_cast<float>(
                                    static_cast<int>( angle / 360.0f ) );
        }
        else
        {
            angle += 360.0f * static_cast<float>(
                                    static_cast<int>(fabs(angle)/360.0f));
        }

        return angle;
    }
}

#endif
