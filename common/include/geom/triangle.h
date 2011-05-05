#ifndef SCOTT_COMMON_GEOM_TRIANGLE
#define SCOTT_COMMON_GEOM_TRIANGLE

#include <math/vector.h>

struct Triangle
{
    Triangle( const Vec3& pA, const Vec3& pB, const Vec3& pC )
        : a( pA ),
          b( pB ),
          c( pC )
    {
    }

    bool operator == ( const Triangle& rhs ) const
    {
        return ( a == rhs.a && b == rhs.b && c == rhs.c );
    }

    Vec3 a, b, c;
};

#endif
