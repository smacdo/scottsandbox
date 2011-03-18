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
#ifndef SCOTT_COMMON_SPHERE_H
#define SCOTT_COMMON_SPHERE_H

#include <common/math/mathdefs.h>
#include <common/math/vector.h>

/**
 * Information representing a sphere in 3d space
 */
struct Sphere
{
    /**
     * Sphere constructor that creates a new sphere from a vector representing
     * the sphere's center, and a scalar representing the sphere's radius
     */
    Sphere( const Vec3& center, const Scalar& radius )
        : center( center ),
          radius( radius )
    {
    }

    /**
     * Sphere constructor that creates a new sphere from a set of scalars
     * representing a vector for the sphere's center, and another scalar for
     * the sphere's radius
     */
    Sphere( const Scalar& x, const Scalar& y, const Scalar& z,
            const Scalar& radius )
        : center( Vec3( x, y, z ) ),
          radius( radius )
    {
    }

    /**
     * The center of the sphere
     */
    Vec3   center;

    /**
     * Radius of the sphere
     */
    Scalar radius;
};

#endif