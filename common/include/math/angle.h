#ifndef SCOTT_MATH_COMMON_ANGLE_H
#define SCOTT_MATH_COMMON_ANGLE_H

#include <math/defs.h>

class Angle;
class Radian;

class Angle
{
    explicit Angle( Scalar angle )
        : m_angle( fmodf(angle, 360.0) + ( angle < 0.0f ? 360.0 : 0 )  )
    {
    }

    Angle( const Radian& rad )
        : m_angle( rad.degrees() )
    {
    }

    Angle( const Angle& angle )
        : m_angle( angle.m_angle )
    {
    }

    Angle& operator = ( const Angle& rhs )
    {
        m_angle = rhs.m_angle;
        return *this;
    }

    Angle& operator = ( const Radian& rhs )
    {
        m_angle = rhs.degrees();
        return *this;
    }

    bool operator == ( const Angle& rhs ) const
    {
        return Math::equalsClose( m_angle, rhs.m_angle );
    }

    bool operator == ( const Radian& rhs ) const
    {
        return Math::equalsClose( m_angle, rhs.degrees() );
    }

    bool operator != ( const Angle& rhs ) const
    {
        return (! Math::equalsClose( m_angle, rhs.m_angle ) );
    }

    bool operator == ( const Radian& rhs ) const
    {
        return (! Math::equalsClose( m_angle, rhs.degrees() ) );
    }

    float toFloat() const
    {
        return static_cast<float>( m_angle );
    }

    double toDouble() const
    {
        return static_cast<double>( m_angle );
    }

    long toLong() const
    {
        return static_cast<long>( roundf( m_angle ) );
    }

    void set( Scalar angle )
    {
        fmodf( angle, 360.0 ) + ( angle < 0.0f ? 360.0 : 0 );   
    }

private:
    Scalar m_angle;
};

#endif
