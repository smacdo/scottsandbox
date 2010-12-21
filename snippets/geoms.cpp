#include <iostream>

struct Plane
{
    Plane()
        : normal( 0.0f, 0.0f, 0.0f ),
          distance( 0.0f )
    {
    }

    Plane( const Vec3& n, float d )
        : normal( n ),
          distance( d )
    {
    }

    Plane( const Vec3& a, const Vec3& b, const Vec3& c )
        : normal( (cross( b - a, c - a )).normalize() ),
          distance( dot( -c, normal ) )
    {
    }

    Plane( float a, float b, float c, float d )
        : normal( a, b, c ),
          distance( d )
    {
        // this needs to be tested
    }

    /**
     * Checks if the given point lies on this plane
     */
    bool IsOnMe( const Vec3& pt ) const
    {
        float dist = getDistanceFromPoint( pt );
        return ( dist > -ZERO_DELTA && d < ZERO_DELTA );
    }

    bool IsFrontOfMe( const Vec3& pt ) const
    {
        return GetPointDistance(pt) > ZERO_DELTA;
    }

    bool IsBehindMe( const Vec3& pt ) const
    {
        return GetPointDistance(pt) < -ZERO_DELTA;
    }

    float getDistanceFromPoint( const Vec3& pt ) const
    {
        return dot( normal, pt ) + distance;
    }

    const Vec3& normal;
    float distance;
};
