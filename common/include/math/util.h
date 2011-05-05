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
    static const float  ZeroEpsilonF  = 0.00001f;
    static const double ZeroEpsilonD = 0.000000001;

    /**
     * Checks if two numeric values of the same type have identical values.
     * For imprecise floating point values, this method will check if they
     * are within an acceptable distance of being equal to each other. 
     */
    template<typename T>
    inline bool equalsClose( T a, T b )
    {
        return a == b;
    }

    /**
     * Checks if the two single precision floating point values are within
     * an acceptable distance of each other.
     */
    template<>
    inline bool equalsClose<float>( float a, float b )
    {
        return ( std::fabs( a - b ) < ZeroEpsilonF );
    }

    /**
     * Checks if the two double precision floating point values are within
     * an acceptable distance of each other
     */
    template<>
    inline bool equalsClose<double>( double a, double b )
    {
        return ( std::fabs( a - b ) < ZeroEpsilonD );
    }

    /**
     * Checks if the value is zero. In the case of floating point values,
     * this method will check if the value is precisely zero or within an
     * acceptable distance (epsilon) of zero.
     */
    template<typename T>
    inline bool isZero( T a )
    {
        return ( a == 0 );
    }

    /**
     * Checks if the single precision floating point value is zero or
     * within an acceptable distance of zero.
     */
    template<>
    inline bool isZero<float>( float a )
    {
        return ( std::fabs( a - a ) < ZeroEpsilonF );
    }

    /**
     * Checks if the double precision floating point value is zero or
     * within an acceptable distance of zero
     */
    template<>
    inline bool isZero<double>( double a )
    {
        return ( std::fabs( a - a ) < ZeroEpsilonD );
    }

    /**
     * Checks if value is not zero. Floating point values are checked
     * for near equality
     */
    template<typename T>
    inline bool isNotZero( T a )
    {
        return ( a != 0 );
    }

    /**
     * Checks if single precision floating point value is not zero
     */
    template<>
    inline bool isNotZero<float>( float a )
    {
        return a > ZeroEpsilonF || a < -ZeroEpsilonF;
    }

    /**
     * Checks if double precision floating point value is not zero
     */
    template<>
    inline bool isNotZero<double>( double a )
    {
        return a > ZeroEpsilonD || a < -ZeroEpsilonD;
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
     * Casts the provided value to the requested type. The value casted
     * will be clamped to be within the acceptable bounds of the new
     * type.
     *
     * ex: castAntClamp<int8_t>( 1000 ) == 255
     */
    template<typename T, typename U>
    U clampedCast( const T& base )
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
     * Converts degrees into radians
     */
    static inline float rad2deg( float radians )
    {
        return radians * 180 / Math::Pi;
    }

    /**
     * Converts radian value into degrees
     */
    static inline float deg2rad( float degrees )
    {
        return degrees * Math::Pi / 180;
    }

    /**
     * Clamps an angle given in degrees to its natural 0-360 range.
     */
    template<typename T>
    inline T clampAngle( T deg )
    {
        return deg % 360;
    }

    /**
     * Returns a clamped double precision angle in [0, 360.0]
     */
    template<>
    inline double clampAngle<double>( double degrees )
    {
        double angle = degrees;

        if ( angle > 360.0 )
        {
            angle -= 360.0 * static_cast<double>(
                                static_cast<int>( angle / 360.0 ));
        }
        else
        {
            angle += 360.0 * static_cast<double>(
                                static_cast<int>(fabs(angle)/360.0));
        }

        return angle;
    }

    /**
     * Returns clamped single precision angle in [0, 360.0]
     */
    template<>
    inline float clampAngle<float>( float degrees )
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

    /**
     * Returns the next highest power of two value for the given
     * argument v.
     *
     * TODO Verify this works correctly on 64bit platforms for values
     * larger than a 32bit int
     */
    static inline int nextPowerOfTwo( int v )
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }

    inline float fastSqrt( float v );
}

#endif
