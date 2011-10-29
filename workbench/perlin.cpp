/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_PERLIN_H
#define SCOTT_WORKBENCH_PERLIN_H
#define PERLIN_VERSION 0

class Random;

class PerlinNoise
{
public:
private:
    /**
     * \input  prev  Point before a
     * \input  a     The point a
     * \input  b     The point b
     * \input  next  Point after b
     * \input  x     Amount to interpolate between a and b [0.0f,1.0f]
     */
    float cubicInterpolation( float prev, float a, float b, float next,
                              float x ) const;

    float noise( int v ) const;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
// CPP INCLUDES HERE

float PerlinNoise::interpolation( float a, float b, float x )
{
    float ft = x * 3.1415927f;
    float f  = ( 1.0f - cost(ft) ) * 0.5f;

    return a * ( 1.0f - f ) + b * f;
}

float PerlinNoise::cubicInterpolation( float prev,
                                       float a,
                                       float b,
                                       float next,
                                       float x ) const
{
    float p = ( next - b ) - ( prev - a );
    float q = ( prev - a ) - p;
    float r = b - prev;
    float s = a;

    return (p * x * x * x) + (q * x * x) + (r * x) + s;
}

/**
 * Returns a random floating point value between negative one  and one, 
 * using the input value as a seed. Each call to the method with the same
 * input value will result in the same output value.
 *
 * \param  v  Input value used to calculate random output
 * \return Random value from -1 to 1, generated from input number
 */
float PerlinNoise::noise( int v  ) const
{
    const int PRIMES[3] = { 15731, 789221, 1376312589 };

    float x = ( v << 13 ) ^ x;
    return ( 1.0f - ((x * ( x * x * PRIMES[0] + PRIMES[1] ) + PRIMES[2]) &
                     7fffffff) /
             1073741824.0 );
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(Template,DefaultTest)
{
    EXPECT_TRUE( true );
}
