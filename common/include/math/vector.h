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
#ifndef SCOTT_COMMON_MATH_VECTOR_H
#define SCOTT_COMMON_MATH_VECTOR_H

#include <math/defs.h>
#include <math/util.h>
#include <math/constants.h>
#include <cmath>

#if MATH_DEBUG_MODE == 1
#   include <cassert>
#   include <limits>
#   define VECTOR_DEBUG_MODE 1
#   define vector_assert(x) assert(x)
#else
#   define vector_assert(x)
#endif

/**
 * Generic templated 3d graphics vector class. Implements all commonly
 * used functionality, including vector rotations and projections.
 */
template<typename T>
class TVector3
{
public:
    /**
     * Standard vector constructor. Follows C++ standards of not
     * initializing basic value types, so do not assume the vector
     * is created with values set to zero.
     *
     * If you need a zero vector, call the static method ::ZeroVector()
     */
    TVector3()
#ifdef VECTOR_DEBUG_MODE
        : m_x( std::numeric_limits<T>::signaling_NaN() ),
          m_y( std::numeric_limits<T>::signaling_NaN() ),
          m_z( std::numeric_limits<T>::signaling_NaN() )
#endif
    {
    }

    /**
     * Copy-initialize vector from a pointer to an array of values.
     * This constructor expects that the passed array contains at 
     * least three values, but there is nothing to check this constraint
     */
    explicit TVector3( const T* vptr )
        : m_x( vptr[0] ), m_y( vptr[1] ), m_z( vptr[2] )
    {
        vector_assert( vptr != 0 );
    }

    /**
     * Standard vector copy constructor. Takes the provided vector,
     * and initializes this vector with the provided's values.
     *
     * TVector3 v( some_other_vector );
     */
    TVector3( const TVector3<T>& vec )
        : m_x( vec.m_x ), m_y( vec.m_y ), m_z( vec.m_z )
    {
    }

    /**
     * Vector x/y/z constructor. Takes the provided x/y/z values and
     * assigns it to the newly constructed vector.
     */
    TVector3( const T& vx, const T& vy, const T& vz )
        : m_x(vx), m_y(vy), m_z(vz)
    {
    }

    /**
     * Read-only index operator. Allows user to look up one of vector's
     * three cardinal axis values.
     */
    const T& operator [] ( size_t index ) const
    {
        vector_assert( index < 3 && "Vector operator[] out of range" );
        return *(&m_x + index);
    }

    /**
     * Writable index operator. Returns a reference to the requested
     * cardinal axis value
     */
    T& operator [] ( size_t index )
    {
        vector_assert( index < 3 && "Vector operator[] out of range" );
        return *(&m_x + index);
    }

    /**
     * Looks up one of the vector's X/Y/Z components, and returns a copy of
     * the value
     */
    T at( size_t index ) const
    {
        switch ( index )
        {
            case 0:
                return m_x;
                break;
            case 1:
                return m_y;
                break;
            case 2:
                return m_z;
                break;
            default:
                vector_assert( false && "Vector at() index out of range" );
                return T();
        }
    }

    /**
     * Look up one of the vector's X/Y/Z components using the axis
     * name, and return the value
     */
    T at( char axis ) const
    {
        switch ( axis )
        {
            case 'x':
            case 'X':
                return m_x;
                break;
            case 'y':
            case 'Y':
                return m_y;
                break;
            case 'z':
            case 'Z':
                return m_z;
                break;
            default:
                vector_assert( false && "Vector at() unknown axis" );
                return T();
        }
    }

    /** 
     * Returns a pointer to the vector's internal data
     */
    const T* ptr() const
    {
        return &m_x;
    }

    /**
     * Vector assignment operator. Assigns the value of the right hand
     * vector to ourself.
     */
    const TVector3<T>& operator = ( const TVector3<T>& rhs )
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;

        return *this;
    }

    /**
     * Equality operator
     */
    bool operator == ( const TVector3<T>& rhs ) const
    {
#ifdef MATH_USE_FUZZY_EQUALS
    return ( equals_close( m_x, rhs.m_x ) &&
             equals_close( m_y, rhs.m_y ) &&
             equals_close( m_z, rhs.m_z ) );
#else
    return ( m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z );
#endif
    }

    /**
     * Inequality operator - nice and simple :)
     */
    bool operator != ( const TVector3<T>& lhs ) const
    {
        return !( *this == lhs );
    }

    /**
     * Unary vector negation. Negates the vector
     */
    friend const TVector3<T> operator - ( const TVector3<T>& lhs )
    {
        return TVector3( -lhs.m_x, -lhs.m_y, -lhs.m_z );
    }

    /**
     * Vector scalar scaling operator
     */
    friend const TVector3<T> operator * ( const TVector3<T>& lhs, 
                                          const T& scalar )
    {
        return TVector3( lhs.m_x * scalar, 
                         lhs.m_y * scalar, 
                         lhs.m_z * scalar );
    }

    /**
     * Vector addition operator
     */
    friend const TVector3<T> operator + ( const TVector3<T>& lhs,
                                          const TVector3<T>& rhs )
    {
        return TVector3( lhs.m_x + rhs.m_x,
                         lhs.m_y + rhs.m_y,
                         lhs.m_z + rhs.m_z );
    }

    /**
     * Vector subtraction operator
     */
    friend const TVector3<T> operator - ( const TVector3<T>& lhs,
                                          const TVector3<T>& rhs )
    {
        return TVector3<T>( lhs.m_x - rhs.m_x,
                            lhs.m_y - rhs.m_y,
                            lhs.m_z - rhs.m_z );
    }

    /**
     * Self addition operator. Adds rhs to self, and stores the result
     */
    TVector3<T>& operator += ( const TVector3<T>& rhs )
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;

        return *this;
    }

    /**
     * Self subtraction operator. Subtracts rhs from self
     */
    TVector3<T>& operator -= ( const TVector3<T>& rhs )
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;

        return *this;
    }

    /**
     * Self scale operation. Scales self according to rhs scale factor
     */
    TVector3<T>& operator *= ( T rhs )
    {
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;

        return *this;
    }

    /**
     * Calculates the result of crossing lhs vector from rhs vector
     */
    template<typename U>
    friend TVector3<U> cross ( const TVector3<U>& lhs,
                               const TVector3<U>& rhs );

    /**
     * Calculates the result of the dot product of lhs vector and rhs
     * vector
     */
    template<typename U>
    friend U dot( const TVector3<U>& lhs, const TVector3<U>& rhs );

    /**
     * Get the angle between two vectors
     */
    template<typename U>
    friend U angleBetween( const TVector3<U>& lhs, const TVector3<U>& rhs );

    /**
     * Get the distance between two vectors
     */
    template<typename U>
    friend U distance( const TVector3<U>& lhs, const TVector3<U>& rhs );


    /**
     * Returns the length (vector magnitude) of this vector.
     *
     * TODO specalize this method so we make appropriate sqrt* calls
     *
     * NOTE: This method has a call to sqrt, which can be expensive if
     * used frequently. If exact distance, and or direction is not needed
     * use the faster squaredLength method instead.
     */
    T length() const
    {
        return sqrtf( m_x * m_x + m_y * m_y + m_z * m_z );
    }

    /**
     * Returns the squared length (vector magnitude) of this vector. Since
     * is the squared length, direction is not correctly indicated.
     *
     * However, squaredLength allows us to avoid a call to sqrt, which
     * makes it significantly faster. Use this method when you simply want
     * to compare absolute lengths.
     */
    T lengthSquared() const
    {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    /**
     * Returns a new vector that this is the normalized representation
     * of this vector.
     */
    TVector3<T> normalized() const
    {
        T len = length();
        vector_assert( len > 0 );

        if ( equals_close( len, static_cast<T>(1.0) ) )
        {
            // If the vector is already normalized, then merely return it
            return TVector3( m_x, m_y, m_z );
        }
        else
        {
            return TVector3( m_x / len, m_y / len, m_z / len );
        }
    }

    /**
     * Rotates around the requested axis
     */
    TVector3<T> getRotatedAroundAxis( T angle, char axis ) const
    {
        switch ( axis )
        {
            case 'x':
                return getRotatedAroundXAxis( angle );
            case 'y':
                return getRotatedAroundYAxis( angle );
            case 'z':
                return getRotatedAroundZAxis( angle );
            default:
                vector_assert( false &&
                           "Vec::getRotatedAround should be 'x','y','z'" );
                return ZeroVector();
        }
    }


    /**
     * Rotates the vector around the X axis, and returns the result
     * of this rotation
     */
    TVector3<T> getRotatedAroundXAxis( const T& angle ) const
    {
        vector_assert( angle != 0.0 ); // or do we assume a=0 means no rotation?

        T sangle = static_cast<T>( sin( Math::Pi * angle / 180 ) );
        T cangle = static_cast<T>( cos( Math::Pi * angle / 180 ) );
        
        return TVector3<T>( m_x,
                            m_y * cangle - m_z * sangle,
                            m_y * sangle + m_z * cangle );
    }

    /**
     * Rotates the vector around the Y axis, and returns the result
     * of this rotation
     */
    TVector3<T> getRotatedAroundYAxis( const T& angle ) const
    {
        vector_assert( angle != 0.0 ); // or do we assume a=0 means no
                                       // rotation?

        T sangle = static_cast<T>( sin( Math::Pi * angle / 180 ) );
        T cangle = static_cast<T>( cos( Math::Pi * angle / 180 ) );

        return TVector3<T>( m_x * cangle + m_z * sangle,
                            m_y,
                           -m_x * sangle + m_z * cangle );
    }

    /**
     * Rotates the vector around the Z axis, and returns the result
     * of the rotation
     */
    TVector3<T> getRotatedAroundZAxis( const T& angle ) const
    {
        vector_assert( angle != 0.0 ); // or do we assume a=0 means no rotation?

        T sangle = static_cast<T>( sin( Math::Pi * angle / 180 ) );
        T cangle = static_cast<T>( cos( Math::Pi * angle / 180 ) );

        return TVector3<T>( m_x * cangle - m_y * sangle,
                            m_y * sangle + m_y * cangle,
                            m_z );
    }

    /**
     * Rotate around an arbitrary axis. We can easily accomplish
     * this with a matrix, however for the sake of not depending on a
     * matrix implementation we'll fake it
     */
    TVector3<T> getRotatedAround( T angle, const TVector3<T>& axis ) const
    {
        vector_assert( angle != 0.0 );

        T sangle = static_cast<T>( sin( Math::Pi * angle / 180 ) );
        T cangle = static_cast<T>( cos( Math::Pi * angle / 180 ) );
        T xangle = static_cast<T>( 1 ) - cangle;    // "1 minus cos angle"

        TVector3<T> u  = axis.normalized();
        TVector3<T> r1( u.m_x * u.m_x + cangle * ( 1 - u.m_x * u.m_x ),
                        u.m_x * u.m_y * xangle - sangle * u.m_z,
                        u.m_y * u.m_z * xangle + sangle * u.m_y );
        TVector3<T> r2( u.m_x * u.m_y * xangle + sangle * u.m_z,
                        u.m_y * u.m_y + cangle * ( 1 - u.m_y * u.m_y ),
                        u.m_y * u.m_z * xangle - sangle * u.m_x );
        TVector3<T> r3( u.m_x * u.m_z * xangle - sangle * u.m_y,
                        u.m_y * u.m_z * xangle + sangle * u.m_x,
                        u.m_z * u.m_z + cangle * ( 1 - u.m_z * u.m_z ) );

        return TVector3<T>( dot( *this, r1 ),
                            dot( *this, r2 ),
                            dot( *this, r3 ) );

    }

    /**
     * Return the X component of the vector
     */
    inline T x() const
    {
        return m_x;
    }

    /**
     * Return the Y component of the vector
     */
    inline T y() const
    {
        return m_y;
    }

    /**
     * Return the Z component of the vector
     */
    inline T z() const
    {
        return m_z;
    }

    /**
     * Returns a zero vector
     */
    static TVector3<T> ZeroVector()
    {
        return TVector3( T(0), T(0), T(0) );
    }

private:
    union
	{
		struct { T m_x, m_y, m_z; };
		struct { T v[3]; };
	};
};

/////////////////////////////////////////////////////////////////////////////
// Vector operations
/////////////////////////////////////////////////////////////////////////////

template<typename T>
TVector3<T> cross ( const TVector3<T>& lhs,
                    const TVector3<T>& rhs )
{
    return TVector3<T>( lhs.m_y * rhs.m_z - rhs.m_y * lhs.m_z,
                        lhs.m_z * rhs.m_x - rhs.m_z * lhs.m_x,
                        lhs.m_x * rhs.m_y - rhs.m_x * lhs.m_y );
}

template<typename T>
T dot ( const TVector3<T>& lhs, const TVector3<T>& rhs )
{
    return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y + lhs.m_z * rhs.m_z;
}

template<typename T>
T angleBetween( const TVector3<T>& lhs, const TVector3<T>& rhs )
{
    T val = acos( dot( lhs.normalized(), rhs.normalized() ) );
    return val * 180 / Math::Pi;
}

template<typename T>
T distance( const TVector3<T>& lhs, const TVector3<T>& rhs )
{
    static_cast<T>( sqrt(( lhs.m_x - rhs.m_x ) * ( lhs.m_x - rhs.m_x ) +
                         ( lhs.m_y - rhs.m_y ) * ( lhs.m_y - rhs.m_y ) +
                         ( lhs.m_z - rhs.m_z ) * ( lhs.m_z - rhs.m_z )) );
}

/////////////////////////////////////////////////////////////////////////////
// Vector typedefs - typedef common vector types
/////////////////////////////////////////////////////////////////////////////
#ifdef MATHLIB_COMMON_TYPEDEFS
typedef TVector3<float>  Vec3;
typedef TVector3<float>  Vec3f;
typedef TVector3<double> Vec3d;
#endif

#endif
