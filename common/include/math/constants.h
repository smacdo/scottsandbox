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
#ifndef SCOTT_COMMON_MATH_CONSTANTS_H
#define SCOTT_COMMON_MATH_CONSTANTS_H

#include <math/defs.h>

///////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////
namespace Math
{
    // Zero delta
    const Scalar ZeroEpsilon  = 1e-8;
    const float  ZeroEpsilonF = 1e-5;
    const double ZeroEpsilonD = 1e-9;

    // Pi
    const Scalar Pi     = 3.141592653589793238462f;
    const Scalar TwoPi  = 6.283185307179586476925286766559f;
    const Scalar HalfPi = 1.570796326794896619231321691639f;

    // e constant
    const Scalar E      = 2.71828182845904523536f;
    const Scalar Log10E = 0.4342944819032518f;
    const Scalar Log2E  = 1.4426950408889633f;

    // sqrt
    const Scalar SqrtTwo      = 1.41421356237309504880f;
    const Scalar SqrtThree    = 1.73205080756887729352f;
    const Scalar SqrtOneHalf  = 0.70710678118654752440f;
    const Scalar SqrtOneThird = 0.57735026918962576450f;

    // deg and radian conversions
    const Scalar Deg2Rad = Pi / 180.0f;
    const Scalar Rad2Deg = 180.0f / Pi;

    // Time conversions
    const Scalar SecToMillisec = 1000.0f;
    const Scalar MillisecToSec = 0.001f;
}

#endif
