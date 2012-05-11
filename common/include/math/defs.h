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
#ifndef SCOTT_MATH_DEFS_H
#define SCOTT_MATH_DEFS_H

#define MATH_COMMON_TYPEDEFS 1
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
#   include <app/debug.h>
#   include <limits>
#   define math_assert(x) scott_assert("Math assertion failed", x)
#   define SCOTT_NAN      std::numeric_limits<T>::signaling_NaN()
#   define MATRIX_DEBUG_MODE 1
#   define VECTOR_DEBUG_MODE 1
#else
#   define math_assert(x) 
#endif

#endif
