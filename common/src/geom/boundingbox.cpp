#include "geom/boundingbox.h"

/**
 * Axis aligned bounding box constructor. Takes two points, the first of which
 * represents the smallest axis aligned point, and the second is the largest
 * axis aligned point.
 */
BoundingBox::BoundingBox( const Vec3& minPt, const Vec3& maxPt )
    : minPoint( minPt ),
      maxPoint( maxPt )
{
}

/**
 * Checks if the sphere intersects this bounding box, including in the
 * case where the sphere only touches the box
 */
bool BoundingBox::intersects( const Sphere& sphere ) const
{
    float sr = sphere.radius();
    float sx = sphere.origin().x();
    float sy = sphere.origin().y();
    float sz = sphere.origin().z();

    return ( sx - sr < mMaxPt.x() ) && ( sx + sr > mMinPt.x() ) &&
           ( sy - sr < mMaxPt.y() ) && ( sy + sr > mMinPt.y() ) &&
           ( sz - sr < mMaxPt.z() ) && ( sz + sr > mMinPt.z() );
}

/**
 * Checks if the boxes intersect, but not if they touch
 */
bool BoundingBox::intersects( const BoundingBox& other ) const
{
    return 
        ( mMinPt.x() < other.mMaxPt.x() && mMaxPt.x() > other.mMinPt.x() &&
          mMinPt.y() < other.mMaxPt.y() && mMaxPt.y() > other.mMinPt.y() &&
          mMinPt.z() < other.mMaxPt.z() && mMaxPt.z() > other.mMinPt.z() );
}
