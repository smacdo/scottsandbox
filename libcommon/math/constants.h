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
#ifndef SCOTT_MATH_CONSTANTS_H
#define SCOTT_MATH_CONSTANTS_H

#include <math/config.h>

///////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////
namespace Math
{
    // Zero delta
    const float  ZeroEpsilonF = 1e-5f;
    const double ZeroEpsilonD = 1e-9;

#ifdef MATH_DOUBLE
    const scalar_t ZeroEpsilon = 1e-9;
#else
    const scalar_t ZeroEpsilon = 1e-5f;
#endif

    // Pi
    const scalar_t Pi     = 3.141592653589793238462f;
    const scalar_t TwoPi  = 6.283185307179586476925286766559f;
    const scalar_t HalfPi = 1.570796326794896619231321691639f;

    // e constant
    const scalar_t E      = 2.71828182845904523536f;
    const scalar_t Log10E = 0.4342944819032518f;
    const scalar_t Log2E  = 1.4426950408889633f;

    // sqrt
    const scalar_t SqrtTwo      = 1.41421356237309504880f;
    const scalar_t SqrtThree    = 1.73205080756887729352f;
    const scalar_t SqrtOneHalf  = 0.70710678118654752440f;
    const scalar_t SqrtOneThird = 0.57735026918962576450f;

    // deg and radian conversions
    const scalar_t Deg2Rad = Pi / 180.0f;
    const scalar_t Rad2Deg = 180.0f / Pi;

    // Time conversions
    const scalar_t SecToMillisec = 1000.0f;
    const scalar_t MillisecToSec = 0.001f;
}

#endif
