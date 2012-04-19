 
class Plane
{
public:
    Plane()
        : m_normal(),
          m_distance( std::numeric_limits<float>::infinity() )
    {
    }

    Plane( const Vec3& normal, float distance )
        : m_normal( normal ),
          m_distance( distance )
    {
    }
    
    Plane( const Vec3& ptA, const Vec3& ptB, const Vec3& ptC )
        : m_normal( Vec3::cross(( ptB-ptA ), ( ptC - ptA )) ),
          m_distance( -Vec3::dot( ptA, normal ) )
    {
    }
    
    bool operator == ( const Plane& p ) const
    {
        return ( Math::CheckEquals( m_distance, p.m_distance ) &&
                 m_normal == p.m_normal );
    }
    
    float distanceToPoint( const Vec3& pt ) const
    {
        return Vec3::dot( m_normal, pt ) + m_distance;
    }
    
    bool intersects( const Ray& ray ) const
    {
        return IsZero( Vec3::dot( m_normal, ray.direction() ) );
    }
    
    bool intersects( const Plane& plane ) const
    {
        return GreaterZero( Vec3::cross( plane.normal, normal ) );  
    }
    
    bool findIntersectionPoint( const Ray& ray, Vec3& intersectionPoint ) const
    {
        float t2 = Vec3::dot( m_normal, ray.direction() );
        
        if ( IsZero( t2 ) )
        {
            return false;
        }
        
        float t1 = -( Vec3::dot(m_normal, ray.origin()) + m_distance ) / t2;
        intersectionPoint = ray.origin() + ( ray.direction * t1 );
        return true;
    }
    
    bool isInside( const Vec3& pos ) const
    {
        return distanceToPoint( pos ) <= 0.0f;
    }

    /**
     * test inclusion, exclusion and intersection between plane and box
     */
    bool exclude( bool& intersects, const AABBox& box ) const
    {
        const Vec3 boxMin = box.min();
        const Vec3 boxMax = box.max();

        Vec3 neg, pos;

        if ( m_normal.X() > 0.0f )
        {
            if ( m_normal.Y() > 0.0f )
            {
                if ( m_normal.Z() > 0.0f )
                {
                    pos = boxMax;
                    neg = boxMin;
                }
                else
                {
                    pos = Vec3( boxMax.X(), boxMax.Y(), boxMin.Z() );
                    neg = Vec3( boxMin.X(), boxMin.Y(), boxMax.Z() );
                }
            }
            else        // normal.Y() <= 0.0f
            {
                if ( m_normal.Z() > 0.0f )
                {
                    pos = Vec3( boxMax.X(), boxMin.Y(), boxMax.Z() );
                    neg = Vec3( boxMin.X(), boxMax.Y(), boxMin.Z() );
                }
                else
                {
                    pos = Vec3( boxMax.X(), boxMin.Y(), boxMin.Z() );
                    neg = Vec3( boxMin.X(), boxMax.Y(), boxMax.Z() );
                }
            }
        }
        else            // normal.X() <= 0.0f
        {
            if ( m_normal.Y() > 0.0f )
            {
                if ( m_normal.Z() > 0.0f )
                {
                    pos = Vec3( minBox.X(), maxBox.Y(), maxBox.Z() );
                    neg = Vec3( maxBox.X(), minBox.Y(), minBox.Z() );
                }
                else
                {
                    pos = Vec3( minBox.X(), maxBox.Y(), minBox.Z() );
                    neg = Vec3( maxBox.X(), minBox.Y(), maxBox.Z() );
                }
            }
            else    // normal.Y() <= 0.0f
            {
                if ( m_normal.Z() > 0.0f )
                {
                    pos = Vec3( minBox.X(), minBox.Y(), maxBox.Z() );
                    neg = Vec3( maxBox.X(), maxBox.Y(), minBox.Z() );
                }
                else
                {
                    pos = minBox;
                    neg = maxBox;
                }
            }
        }

        if ( distanceToPoint( neg ) > 0.0f )
        {
            // box is excluded
            return true;
        }
        else if ( distanceToPoint( pos ) > 0.0f )
        {
            // intersects plane
            intersects = true;
        }

        // box intersects the plane, so it is not fully excluded
        return false;
    }

    
    /**
     * Returns the normal vector that defines the plane in 3d space
     */
    Vec3 normal() const
    {
        return m_normal;
    }
    
    /**
     * Returns the distance between the origin and the normal vector
     */
    float distance() const
    {
        return m_distance;
    }

private:
    float calculateDistanceFromOrigin( const Vec3& normal, const Vec3& pt )
    {
        return 
    }
    
private:
    /**
     * Normal vector that defines the plane's orientation in 3d space
     */
    Vec3 m_normal;
    
    /**
     * Distance from the origin to the plane's normal vector
     */
    float m_distance;
};
 */
