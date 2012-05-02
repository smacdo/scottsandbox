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
        return ( std::fabs( a ) < ZeroEpsilonF );
    }

    /**
     * Checks if the double precision floating point value is zero or
     * within an acceptable distance of zero
     */
    template<>
    inline bool isZero<double>( double a )
    {
        return ( std::fabs( a ) < ZeroEpsilonD );
    }

    /**
     * Checks if value is not zero. Floating point values are checked
     * for near equality
     */
    template<typename T>
    inline bool notZero( T a )
    {
        return ( a != 0 );
    }

    /**
     * Checks if single precision floating point value is not zero
     */
    template<>
    inline bool notZero<float>( float a )
    {
        return std::fabs( a ) > ZeroEpsilonF;
    }

    /**
     * Checks if double precision floating point value is not zero
     */
    template<>
    inline bool notZero<double>( double a )
    {
        return std::fabs( a ) > ZeroEpsilonD;
    }

    /**
     * Return the fractional portion of a value
     */
    template<typename T>
    inline T fraction( T a )
    {
        return static_cast<T>( 0 );
    }

    /**
     * Return the fractional portion of a value
     */
    template<>
    inline float fraction( float a )
    {
        return a - std::floor( a );
    }

    /**
     * Return the fractional portion of a value
     */
    template<>
    inline double fraction( double a )
    {
        return a - std::floor( a );
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

        // Account for negatives
        if ( base < 0 )
        {
            base += grid;
        }
        
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
     * Clamps an angle given in degrees to its natural 0-360 range.
     */
    template<typename T>
    inline T clampAngle( T angle )
    {
        angle = angle % 360;

        if ( angle < 0 )
        {
            angle += 360;
        }

        return angle;
    }

    /**
     * Returns a clamped double precision angle in [0, 360.0]
     */
    template<>
    inline double clampAngle<double>( double angle )
    {
        angle = std::fmod( angle, 360.0 );

        if ( angle < 0.0 )
        {
            angle += 360.0;
        }

        return angle;
    }

    /**
     * Returns clamped single precision angle in [0, 360.0]
     */
    template<>
    inline float clampAngle<float>( float angle )
    {
        angle = std::fmod( angle, 360.0f );
        
        if ( angle < 0.0f )
        {
            angle += 360.0f;
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
    template<typename T>
    T nextPowerOfTwo( T n )
    {
        static_assert( std::numeric_limits<T>::is_integer, "Cannot be float" );

        std::size_t k = 1;
        n -= 1;

        do
        {
            n  |= n >> k;
            k <<= 1;
        }
        while ( k < static_cast<size_t>(std::numeric_limits<T>::digits) );

        return ++n;
    }

    template<>
    inline unsigned int nextPowerOfTwo( unsigned int v )
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

    /**
     * Computes a 32 bit hash from a floating point value. This is used to
     * quickly cache floats into a hashmap, but care must be taken since two
     * seemingly identical floating point values can have different memory values
     * and hence different hash values
     */
    inline unsigned int hashfloat( float value );

    /**
     * Computes a 32 bit hash from a floating point value. This is used to
     * quickly cache floats into a hashmap, but care must be taken since two
     * seemingly identical floating point values can have different memory values
     * and hence different hash values
     */
    inline unsigned int hashfloat( const float * pArray, size_t arraySize );

    inline float fastSqrt( float v );
}

#endif
