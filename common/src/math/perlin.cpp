/*
 * Copyright 2010-2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_MATH_PERLIN_H
#define SCOTT_MATH_PERLIN_H

#include <stdlib>
#include <boost/random.hpp>

PerlinNoise::PerlinNoise( unsigned int seed )
    : pRandomLUT( new unsigned char[RANDOM_LUT_SIZE] )
{
    init( seed );
}

PerlinNoise::PerlinNoise( const PerlinNoise& p )
    : pRandomLUT( new unsigned char[RANDOM_LUT_SIZE] )
{
    std::copy( p.pRandomLUT,
               p.pRandomLUT + RANDOM_LUT_SIZE,
               pRandomLut );
}

PerlinNoise::~PerlinNoise()
{
    delete[] pRandomLUT;
}

PerlinNoise& PerlinNoise::operator = ( const PerlinNoise& rhs )
{
    // Only copy the random lookup table if we are not the same instance
    if ( this == &p )
    {
        // Destroy original random buffer
        delete[] pRandomLUT;

        // Now re-create the buffer, and populate it with the contents of
        // 'rhs'
        pRandomLUT = new unsigned char[RANDOM_LUT_SIZE];

        std::copy( rhs.pRandomLUT,
                   rhs.pRandomLUT + RANDOM_LUT_SIZE,
                   pRandomLUT );
    }

    return *this;
}

float PerlinNoise::noise3( const Vec3& pos ) const
{
    return noise3( pos.x(), pos.y(), pos.z(), 0, 0, 0 );
}

float PerlinNoise::noise3( float x, float y, float z ) const
{
    return noise3( x, y, z, 0, 0, 0 );
}

float PerlinNoise::noise3( float x,
                           float y,
                           float z,
                           int xWrap,
                           int yWrap,
                           int zWrap ) const
{
}

float PerlinNoise::grad( int hash, float x, float y, float z )
{
    const static float BASIS[12][4] =
    {
        {  1, 1, 0 },
        { -1, 1, 0 },
        {  1,-1, 0 },
        { -1,-1, 0 },
        {  1, 0, 1 },
        { -1, 0, 1 },
        {  1, 0,-1 },
        { -1, 0,-1 },
        {  0, 1, 1 },
        {  0,-1, 1 },
        {  0, 1,-1 },
        {  0,-1,-1 },
    };

    // perlin's gradient has 12 cases so some get used 1/16th of the time
    // and some 2/16ths. We reduce bias by changing those fractions
    // to 5/16ths and 6/16ths, and the same 4 cases get the extra weight.
    const static unsigned char INDICES[64] =
    {
        0, 1, 2, 3,  4, 5, 6, 7, 8, 9, 10, 11,
        0, 9, 1, 11,
        0, 1, 2, 3,  4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3,  4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3,  4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3,  4, 5, 6, 7, 8, 9, 10, 11,
    };

    // Generate the value using the reference permutation table
    float *pGrad = BASIS[ INDICES[ hash & 63 ] ];
    return pGrad[0] * x + pGrad[1] * y + pGrad[2] * z;
}

float PerlinNoise::lerp( float a, float b, float t )
{
    return a + ( b - a ) * t;
}

void PerlinNoise::init( unsigned int seed )
{
    boost::mt19937 rng( seed );
    boost::uniform_smallint<> randomNumber( 0, 255 );

    // Generate the look up table. Remember that the look up table contains
    // 256 unique elements, but each element is duplicated (at pos i and
    // i+256)
    for ( unsigned int i = 0; i < RANDOM_LUT_SIZE / 2; ++i )
    {
        unsigned char val   = randomNumber( rng );
        pRandomLUT[i + 0]   = val;
        pRandomLUT[i + 256] = val;
    }
}
