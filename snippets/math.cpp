

class Plane
{
public:
    Plane()
        : m_normal(),
          m_distance( std::numeric_limits<float>::infinity() ),
          m_inverseZ( std::numeric_limits<float>::infinite() )
    {
    }

    Plane( const Vec3& normal, float dist )
        : m_normal( normal ),
          m_distance( dist ),
          m_inverseZ( 1.0 / m_normal.Z() )
    {
    }

    Plane( const Vec3& ptA, const Vec3& ptB, const Vec3& ptC )
    {
        Vec3 v  = ptB - ptC;
        Vec3 w  = ptA - ptB;

        Vec3 vw    = Vec3::cross( v, w );      // DOUBLE CHECK THIS
        m_normal   = vw.normalized( Vec3::cross( ptB - ptC, ptA - ptB ) );
        m_distance = 0.0f - Vec3::dot( m_normal, ptA );
        
        if ( m_normal.Z() != 0.0f )
        {
            m_inverseZ = -1.0f / N.z;
        }
        else
        {
            m_inverseZ = 0.0f;      // is this error?
            assert( false );
        }
    }

    bool operator == ( const Plane& p ) const
    {
        return ( m_normal == p.m_normal &&
                 EqualsClose( m_distance, p.m_distance ) );
    }

    float distanceToPoint( const Vec3& pt ) const
    {
        return Vec3::dot( m_normal, pt ) + m_distance;
    }

    bool findIntersectionPoint( const Vec3& pt1,
                                const Vec3& pt2,
                                Vec3& result )
    {
        float dp1 = distanceToPoint( pt1 );
        float dp2 = distanceToPoint( pt2 );

        if ( dp1 <= 0.0 && dp2 <= 0 )
        {
            return false;
        }
        else if ( dp2 > 0.0 )
        {
            return false;
        }

        Vec3 direction = pt2 - pt1;

        float dot1 = Vec3::dot( direction, m_normal );
        float dot2 = dp1 - m_distance;
        float t    = -(D + dot2) / dot1;

        result = ( direction * t ) + pt1;
        return true;
    }

    bool inside( const Vec3& pos ) const
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

    float solveZ( float x, float y )
    {
        float z = ( m_normal.Y() * y + m_normal.X() * x + m_distance ) *
                  m_inverseZ;
        return z;
    }

private:
    /**
     * Ax By Cz
     */
    Vec3 m_normal;

    /**
     * D defines the plane equation
     */
    float m_distance;

    /**
     * Pre-computed (-1.0/m_normal.Z) to speed up intersection
     * computations
     */
    float m_inverseZ;
};


/**
 * Compute reflection vector between two vectors
 */
void reflection( const Vec3& a, const Vec3& b )
{
}

float inline bool CloseEquals( float lhs, float rhs )
{
    static const float epsilon = std::numeric_limits<float>::epsilon() * 3;
#ifdef CLOSE_EQUALS_BETTER
    return fabs( (lhs - rhs) / ((rhs == 0.0f ? 1.0f : rhs)) ) < epsilon;
#else
    return fabs( lhs - rhs ) < epsilon;
}

float inline bool CloseIsZero( float lhs )
{
    static const float epsilon = std::numeric_limits<float>::epsilon() * 3;
    return fabs( lhs ) < epsilon;
}
