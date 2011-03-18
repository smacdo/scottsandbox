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
#include "math/geoms.h"
#include <limits>

IntersectResult::IntersectResult()
    : point(Vec3(  0.0f, 0.0f, 0.0f )),
      normal(Vec3( 0.0f, 0.0f, 0.0f )),
      distance( std::numeric_limits<Scalar>::infinity() )
{
}

IntersectResult::IntersectResult( const Vec3& pt,
                                  const Vec3& n,
                                  Scalar dist )
    : point( pt ),
      normal( n ),
      distance( dist )
{
}

bool IntersectResult::didHit() const
{
    return distance != std::numeric_limits<Scalar>::infinity();
}

Plane::Plane( const Vec3& normal, const Scalar& distance )
    : normal( normal ),
      distance( distance )
{
}

Plane::Plane( const Scalar& nX, const Scalar& nY, const Scalar& nZ,
              const Scalar& distance )
    : normal( Vec3( nX, nY, nZ ) ),
      distance( distance )
{
}

bool Plane::intersects( const Ray& ray ) const
{
    Scalar d2 = dot( normal, ray.direction );

    if ( greaterequal_zero(d2) ) 
    {
        return false;     // ray is parallel to plane, or ray hits wrong side
    }

    Scalar d1 = -( dot( normal, ray.origin ) + distance );
    Scalar  t = d1 / d2;
    
    if ( less_zero(t) )
    {
        return false;     // ray intersects behind plane origin
    }

    Vec3 intersectPt = ray.origin + ray.direction * t;
    return true;
}

