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
#ifndef SCOTT_COMMON_MATH_DEFS_H
#define SCOTT_COMMON_MATH_DEFS_H

#define MATHLIB_COMMON_TYPEDEFS 1
#define MATH_USE_FUZZY_EQUALS 1

/**
 * Defines the Scalar type, which is used by the math library to determine
 * the precision of a floating point value. To change this, you can define
 * either MATH_SCALAR_FLOAT or MATH_SCALAR_DOUBLE. If none are defined,
 * then mathlib will use a default of "float".
 */
#ifndef MATH_SCALAR_DEFINED
#define MATH_SCALAR_DEFINED
#   ifdef MATH_SCALAR_FLOAT
        typedef float Scalar;
#   elif  MATH_SCALAR_DOUBLE
        typedef double Scalar;
#   else
        typedef float Scalar;
#   endif
#endif

/**
 * Math assertion macro. Assertions in the mathlibrary use math_assert,
 * rather than assert. This allows us to selectively disable math
 * assertions and continue using assertions in the rest of the application.
 * By default, math_assert is only enabled when MATH_DEBUG_MODE is defined.
 */
#ifdef MATH_DEBUG_MODE
#   include <cassert>
#   define math_assert(x) assert(x)
#else
#   define math_assert(x) 
#endif

#endif
