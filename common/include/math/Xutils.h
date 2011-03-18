/**
 * Copyright 2010 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY SCOTT MACDONALD ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL SCOTT MACDONALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
#ifndef SCOTT_COMMON_MATH_UTILS_H
#define SCOTT_COMMON_MATH_UTILS_H

#include <common/math/defs.h>
#include <common/math/constants.h>
#include <limits>

/**
 * Checks if two values of type T are equal, account for a small amount
 * of numeric inaccuracy. (Uses the floating point epsilon as the acceptable
 * range of inaccuracy)
 */
template<typename T>
inline T equals_close( T a, T b )
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
    return  (
                (a - Math::ZeroEpsilonF <= b) &&
                (a + Math::ZeroEpsilonF >= b)
            );
}

/**
 * Checks if two double values are equal, accounting for a small amount
 * of numeric inaccuracy
 */
template<>
inline bool equals_close<double>( double a, double b )
{
    return (
                (a - Math::ZeroEpsilonD <= b) &&
                (a + Math::ZeroEpsilonD >= b)
           );
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
    return ( a <= Math::ZeroEpsilonF && a >= -Math::ZeroEpsilonF );
}

template<>
inline bool is_zero<float>( double b )
{
    return ( a <= Math::ZeroEpsilonD && a >= -Math::ZeroEpsilonD );
}

/**
 * Checks if value is greater than zero. Uses floating point epsilon to allow
 * for an acceptable range of inaccuracy
 */
template<typename T>
inline bool greater_zero<float>( T a )
{
    return ( a > Math::ZeroEpsilon );
}

template<>
inline bool greater_zero<float>( float a )
{
    return ( a > Math::ZeroEpsilonF );
}

template<>
inline bool greater_zero<float>( double b )
{
    return ( a > Math::ZeroEpsilonD );
}

/**
 * Checks if a value is greater or equal to zero. Uses floating point epsilon to
 * allow for an acceptable range of inaccuracy
 */
template<typename T>
inline bool greaterequal_zero( const T& a )
{
    return ( a > -Math::ZeroEpsilon );
}

/**
 * Checks if value is less than zero. Uses floating point epsilon to allow for
 * an acceptable range of inaccuracy
 */
template<typename T>
inline bool less_zero( const T& a )
{
    return ( a < -Math::ZeroEpsilon );
}

/**
 * Checks if a value is equal to or less than zero. Uses floating point epsilon
 * to allow for an acceptable range of inaccuracy
 */
template<typename T>
inline bool lessequals_zero( const T& a )
{
    return ( a < Math::ZeroEpsilon );
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
 * Takes two values and returns the minimum value
 */
template<typename T>
T min( const T& a, const T& b )
{
    return ( a < b ? a : b );
}

/**
 * Takes two values, and returns the maximum value
 */
template<typename T>
T max( const T& a, const T& b )
{
    return ( a > b ? a : b );
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
 * Convert given radians to degrees
 */
inline float rad2deg( float r )
{
    return r * (180 / Math::PI);
}

inline float deg2rad( float d )
{
    return d * ( Math::PI / 180 );
}

inline float clampAngle( float degrees )
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
                                static_cast<int>(std::fabs(angle)/360.0f));
    }

    return angle;
}

template<typename T>
T random()
{
    return static_cast<T>( rand() );
}

template<>
float random<float>()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

template<>
float random<double>()
{
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

inline int random( int min, int max )
{
    // TEST
    return min + ( random<int>() %  (max - min + 1) );
}

/**
 * Returns a random floating point value
 */
Scalar random( Scalar min, Scalar max );
int    random( int    min, int    max );

template<typename T>
T normalizeAngle360( T angle )
{
    if ( angle >= 360.0 || angle < 0.0 )
    {
        angle -= floor( angle / 360.0 ) * 360.0;
    }

    return angle;
}


#endif
