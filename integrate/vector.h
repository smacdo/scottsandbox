// An older version of my vector class. It has several extra utility methods
// (mostly component wise comparisons, min/max, checks) that should be
// integerated as non-member functions. Possibly even on-friend

/*
 * Copyright (c) 2011 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Scott MacDonald nor the names of contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL SCOTT MACDONALD BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SCOTT_MATH_VECTOR_H
#define SCOTT_MATH_VECTOR_H

#include "math/mathdefs.h"
#include "math/mathutils.h"
#include <stdint.h>
#include <cassert>
#include <ostream>

/**
 * Defines a mathmatical vector with three components
 */
template<typename T>
class TVector4
{
public:
    TVector4()
    {
    }

    TVector4( T vx, T vy, T vz, T vw )
        : x(vx), y(vy), z(vz), w(vw)
    {
    }

    TVector4( T vx, T vy, T vz )
        : x(vx), y(vy), z(vz), w(static_cast<T>(1.0))
    {
    }

    explicit TVector4( T *pv )
        : x( pv[0] ), y( pv[1] ), z( pv[2] ), w( pv[3] )
    {
    }

    TVector4( const TVector4& v )
        : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }

    T& operator [] ( size_t offset )
    {
        math_assert( offset < 4 );
        return xyzw[offset];
    }

    const T& operator [] ( size_t offset ) const
    {
        math_assert( offset < 4 );
        return xyzw[offset];
    }

    T* c_ptr()
    {
        return &xyzw[0];
    }

    const T* c_ptr() const
    {
        return &xyzw[0];
    }

    TVector4& operator = ( const TVector4& rhs )
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;

        return *this;
    }

    bool operator == ( const TVector4& rhs ) const
    {
        return Math::CheckEquals( x, rhs.x ) &&
               Math::CheckEquals( y, rhs.y ) &&
               Math::CheckEquals( z, rhs.z ) &&
               Math::CheckEquals( w, rhs.w );
    }

    bool operator != ( const TVector4& rhs ) const
    {
        return !( *this == rhs );
    }

    bool operator < ( const TVector4& rhs ) const
    {
        return ((x < rhs.x) && (y < rhs.y) && (z < rhs.z) && (w < rhs.w));
    }

    bool operator <= ( const TVector4& rhs ) const
    {
        return ((x <= rhs.x) && (y <= rhs.y) && (z <= rhs.z) && (w <= rhs.w));
    }

    bool operator > ( const TVector4& rhs ) const
    {
        return ((x > rhs.x) && (y > rhs.y) && (z > rhs.z) && (w > rhs.w));
    }

    bool operator >= ( const TVector4& rhs ) const
    {
        return ((x >= rhs.x) && (y >= rhs.y) && (z >= rhs.z) && (w >= rhs.w));
    }

    TVector4 operator - () const
    {
        return TVector4( -x, -y, -z, -w );
    }

    TVector4 operator + ( const TVector4& rhs ) const
    {
        return TVector4( x + rhs.x,
                         y + rhs.y,
                         z + rhs.z,
                         w + rhs.w );
    }

    TVector4& operator += ( const TVector4& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;

        return *this;
    }

    TVector4 operator - ( const TVector4& rhs ) const
    {
        return TVector4( x - rhs.x,
                         y - rhs.y,
                         z - rhs.z,
                         w - rhs.w );
    }

    TVector4& operator -= ( const TVector4& rhs )
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;

        return *this;
    }

    TVector4 operator * ( T rhs ) const
    {
        return TVector4( x * rhs, y * rhs, z * rhs, w * rhs );
    }

    TVector4& operator *= ( T rhs )
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;

        return *this;
    }

    TVector4 operator / ( T rhs ) const
    {
        return TVector4( x / rhs, y / rhs, z / rhs, w / rhs );
    }

    TVector4& operator /= ( T rhs )
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;

        return *this;
    }

    template<typename U>
    friend std::ostream& operator << ( std::ostream& s, const TVector4<U>& v );

    /**
     * Computes the dot product of this vector and the given vector rhs
     */
    T dot( const TVector4& rhs ) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    /**
     * Returns the length of the vector. This is a somewhat expensive call,
     * since it must take the square root of the vector. Use lengthSquared
     * when possible instead.
     */
    T length() const
    {
        T slen = x * x + y * y + z * z + w * w;
        return sqrt( slen );
    }

    /**
     * Returns the squared length of the vector. This is significantly
     * faster than the length() call since it avoids a call to sqrt.
     */
    T lengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    /**
     * Returns a normalized form of the vector. A normalized vector is one
     * in which its length is equal to one.
     */
    TVector4 normalized() const
    {
        T len = length();
        math_assert( len > static_cast<T>(0.0) );

        return TVector4( x / len,
                         y / len,
                         z / len,
                         w / len );
    }

    /**
     * Normalizes this vector. A normalized vector is one in which its length
     * is equal to one
     */
    TVector4& normalize()
    {
        T len = length();

        x /= len;
        y /= len;
        z /= len;
        w /= len;

        return *this;
    }

    /**
     * Checks if the vector is normalized
     */
    bool isNormalized() const
    {
        return Math::CheckEquals( length(), static_cast<T>( 1.0 ) );
    }

    /**
     * Checks if this is a zero vector. A vero vector is one in which
     * all components are equal to zero
     */
    bool isZero() const
    {
        return Math::CheckIsZero( x ) && Math::CheckIsZero( y ) &&
               Math::CheckIsZero( z ) && Math::CheckIsZero( w );
    }

    /**
     * Checks if this is a homogenous 4d vector (eg, the fourth
     * component 'w' is equal to 1)
     */
    bool isHomogenous() const
    {
        return Math::CheckEquals( w, static_cast<T>( 1.0 ) );
    }

    T X() const
    {
        return x;
    }

    T Y() const
    {
        return y;
    }

    T Z() const
    {
        return z;
    }

    T W() const
    {
        return w;
    }

    const static TVector4 Zero;
    const static TVector4 One;

private:
    union
    {
        T xyzw[4];
        struct
        {
            T x, y, z, w;
        };
    };

};

template<typename T>
const TVector4<T> TVector4<T>::Zero = TVector4<T>( 0, 0, 0 );

template<typename T>
const TVector4<T> TVector4<T>::One  = TVector4<T>( 1, 1, 1 );

template<typename T>
std::ostream& operator << ( std::ostream& stream, const TVector4<T>& v )
{
    stream << "<"  << v.x << ", " << v.y << ", " << v.z
           << ", " << v.w << ">";
    return stream;
}

/////////////////////////////////////////////////////////////////////////////
/// Templated Vector3 class
/////////////////////////////////////////////////////////////////////////////
/**
 * Defines a mathematical vector consisting of three componenets
 */
template<typename T>
class TVector3
{
public:
    /**
     * Default constructor, does not initialize components to a valid state
     */
    TVector3()
    {
    }

    /**
     * Constructor that sets vector equal to provided (x/y/z) values
     *
     * \param vx X value
     * \param vy Y value
     * \param vz Z value
     */
    TVector3( T vx, T vy, T vz )
        : x(vx), y(vy), z(vz)
    {
    }

    /**
     * Copy constructor that takes a pointer to a c array and copies
     * the first three values as x, y and z.
     *
     * \param pv C array to copy three values from
     */
    explicit TVector3( T *pv )
        : x( pv[0] ), y( pv[1] ), z( pv[2] )
    {
    }

    /**
     * Copy constructor. Make this vector be just like that one
     *
     * \param v Vector to copy values from
     */
    TVector3( const TVector3& v )
        : x(v.x), y(v.y), z(v.z)
    {
    }

    /**
     * Index operator. Allows you to directly index any of the three
     * vector component (0 is x, 1 is y, 2 is z).
     *
     * \param  offset The component to look up
     * \return Reference to the value
     */
    T& operator [] ( size_t offset )
    {
        math_assert( offset < 3 );
        return xyz[offset];
    }

    /**
     * Index operator. Allows you to directly index any of the tree
     * vector components (0 is x, 1 is y, 2 is z ). The reference
     * returned is const.
     *
     * \param  offset The component to look up
     * \return Const reference to the value
     */
    const T& operator [] ( size_t offset ) const
    {
        math_assert( offset < 3 );
        return xyz[offset];
    }

    /**
     * Returns a pointer to the vector's internal storage. Use of this
     * method is discouraged unless required by a method (such as OpenGL)
     */
    T* c_ptr()
    {
        return &xyz[0];
    }

    /**
     * Returns a const pointer to the vector's internal storage. USe of this
     * method is discourage unless required by a method (such as OpenGL)
     */
    const T* c_ptr() const
    {
        return &xyz[0];
    }

    /**
     * Assignment operator. Makes this vector have the same values as the
     * vector on the right hand side of the expression
     *
     * \param  rhs The vector to copy from
     * \return A reference to the newly assigned LH vector
     */
    TVector3& operator = ( const TVector3& rhs )
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return *this;
    }


    /**
     * Equality operator. Checks if this vector is equal in value to the
     * 'rhs' vector.
     * 
     * \param  rhs Vector to compare to
     * \return If the vectors are similiar (almost equal) in value
     */
    bool operator == ( const TVector3& rhs ) const
    {
        return Math::CheckEquals( x, rhs.x ) &&
               Math::CheckEquals( y, rhs.y ) &&
               Math::CheckEquals( z, rhs.z );
    }

    bool operator != ( const TVector3& rhs ) const
    {
        return !( *this == rhs );
    }

    bool operator < ( const TVector3& rhs ) const
    {
        return ((x < rhs.x) && (y < rhs.y) && (z < rhs.z));
    }

    bool operator <= ( const TVector3& rhs ) const
    {
        return ((x <= rhs.x) && (y <= rhs.y) && (z <= rhs.z));
    }

    bool operator > ( const TVector3& rhs ) const
    {
        return ((x > rhs.x) && (y > rhs.y) && (z > rhs.z));
    }

    bool operator >= ( const TVector3& rhs ) const
    {
        return ((x >= rhs.x) && (y >= rhs.y) && (z >= rhs.z));
    }

    TVector3 operator - () const
    {
        return TVector3( -x, -y, -z );
    }

    TVector3 operator + ( const TVector3& rhs ) const
    {
        return TVector3( x + rhs.x,
                         y + rhs.y,
                         z + rhs.z );
    }

    TVector3& operator += ( const TVector3& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    TVector3 operator - ( const TVector3& rhs ) const
    {
        return TVector3( x - rhs.x,
                         y - rhs.y,
                         z - rhs.z );
    }

    TVector3& operator -= ( const TVector3& rhs )
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    TVector3 operator * ( T rhs ) const
    {
        return TVector3( x * rhs, y * rhs, z * rhs );
    }

    TVector3& operator *= ( T rhs )
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;

        return *this;
    }

    TVector3 operator / ( T rhs ) const
    {
        return TVector3( x / rhs, y / rhs, z / rhs );
    }

    TVector3& operator /= ( T rhs )
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;

        return *this;
    }

    /**
     * Output stream operator
     */
    template<typename U>
    friend std::ostream& operator << ( std::ostream& s, const TVector3<U>& v );

    /**
     * Calculates the dot product of two vectors and returns the result
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Dot product of the two vectors
     */
    friend T dot( const TVector3& lhs, const TVector3& rhs ) const
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    /**
     * Returns the cross product of two Vector3 values
     * Calculates the cross product of two vectors and returns the result
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Result of crossing the first vector with the second
     */
    friend TVector3 cross( const TVector3& lhs, const TVector3& rhs )
    {
        // x := y0 * z1 - z0 * y1
        // y := z0 * x1 - x0 * z1
        // z := x0 * y1 - y0 * x1

        return TVector3( rhs.y * lhs.z - rhs.z * lhs.y,
                         rhs.z * lhs.x - rhs.x * lhs.z,
                         rhs.x * lhs.y - rhs.y * lhs.x  );
    }

    /**
     * Calculates the distance between two vectors and returns the result
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Distance between the first vector and the second
     */
    friend T distance( const TVector3& lhs, const TVector3& rhs )
    {
        std::sqrt( ( rhs.x - lhs.x ) * ( rhs.x - lhs.x ) +
                   ( rhs.y - lhs.y ) * ( rhs.y - lhs.y ) +
                   ( rhs.z - lhs.z ) * ( rhs.z - lhs.z ) );
    }

    /**
     * Returns the distance between two vectors squared. This is
     * significantly faster than distance(), use it when you need to compare
     * distances without regard to direction
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Distancfe between the first vector and the second, squared
     */
    friend T distanceSquared( const TVector3& lhs, const TVector3& rhs )
    {
        return ( ( rhs.x - lhs.x ) * ( rhs.x - lhs.x ) +
                 ( rhs.y - lhs.y ) * ( rhs.y - lhs.y ) +
                 ( rhs.z - lhs.z ) * ( rhs.z - lhs.z ) );
    }

    /**
     * Returns a vector that consists of the maximum value in each of the
     * components of the two vectors passed to this function
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Vector consisting of the maximum value in each component
     *         from the first and second vector
     */
    friend TVector3 max( const TVector3& lhs, const TVector3& rhs )
    {
        return TVector3( std::max( lhs.x, rhs.x ),
                         std::max( lhs.y, rhs.y ),
                         std::max( lhs.z, rhs.z ) );
    }

    /**
     * Returns a vector that consists of the minimum value in each of the
     * components of the two vectors passed to this function
     *
     * \param  lhs The first vector
     * \param  rhs The second vector
     * \return Vector consisting of the minimum value in each component
     *         from the first and second vector
     */
    friend TVector3 min( const TVector3& lhs, const TVector3& rhs )
    {
        return TVector3( std::min( lhs.x, rhs.x ),
                         std::min( lhs.y, rhs.y ),
                         std::min( lhs.z, rhs.z ) );
    }

    /**
     * Calculate and return the angle between the two provided vectors
     *
     * \param  The first vector
     * \param  The second vector
     * \return Angle between the first vector and the second
     */
    friend T angleBetween( const TVector3& lhs, const TVector3& rhs )
    {
        T lenQ = length( rhs ) * length( lhs );

        // Make sure calculated length > 0 to avoid dividing by zero
        assert( lenQ > static_cast<T>( 0 ) );

        T f = ( dot( rhs, lhs ) ) / lenQ;

        // Make sure input is in range [-1..1]
        f = clamp( f, static_cast<T>(1), static_cast<T>(-1) );

        // Calculate angle
        return static_cast<T>( acos( f ) );
    }

    /**
     * Returns the vector's length (also known as the vector's magnitude)
     *
     * \return Vector length
     */
    T length() const
    {
        return sqrt( x * x + y * y + z * z );
    }

    /**
     * Returns the squared length (also known as the vector's magnitude)
     * of the vector. Faster than a call to length() as it avoids a call
     * to sqrt, but makes it unable to determine exact length or direction
     *
     * \param Square of the vector's length
     */
    T lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    /**
     * Returns a copy of this vector in normalized form. No harm will
     * come to the original vector
     *
     * \return The normalized form of this vector
     */
    TVector3 normalized() const
    {
        T len = length();
        math_assert( len > static_cast<T>(0.0) );

        return TVector3( x / len,
                         y / len,
                         z / len );
    }

    /**
     * Normalizes the vector by dividing each component value against the
     * vectors length.
     *
     * \return A reference to our newly normalized vector
     */
    TVector3& normalize()
    {
        T len = length();

        x /= len;
        y /= len;
        z /= len;

        return *this;
    }

    /**
     * Checks if the vector is normal (the length is one), and returns
     * true if this is the case
     *
     * \return True if the vector is normalized, false otherwise
     */
    bool isNormalized() const
    {
        return Math::CheckEquals( length(), static_cast<T>( 1.0 ) );
    }

    /**
     * Checks if the vector consists entirely of zero values, and returns
     * true if that is the case
     *
     * \return True if the vector's components are all zero
     */
    bool isZero() const
    {
        return Math::CheckEquals( x, static_cast<T>( 0.0 ) ) &&
               Math::CheckEquals( y, static_cast<T>( 0.0 ) ) &&
               Math::CheckEquals( z, static_cast<T>( 0.0 ) );
    }

    /**
     * Returns the vector's X value
     *
     * \return X component of the vector
     */
    T X() const
    {
        return x;
    }

    /**
     * Returns the vector's Y value
     * \return Y component of the vector
     */
    T Y() const
    {
        return y;
    }

    /**
     * Returns the vector's Z value
     * \return Z component of the vector
     */
    T Z() const
    {
        return z;
    }

    const static TVector3<T> Zero;
    cosnt static TVector3<T> One;
    const static TVector3<T> Up;
    const static TVector3<T> Down;
    const static TVector3<T> Forward;
    const static TVector3<T> Backward;
    const static TVector3<T> Left;
    const static TVector3<T> Right;
    const static TVector3<T> BasisX;
    const static TVector3<T> BasisY;
    const static TVector3<T> BasisZ;

private:
    union
    {
        T xyz[3];
        struct
        {
            T x, y, z;
        };
    };
};

template<typename T>
const TVector3<T> TVector3<T>::Zero  = TVector3<T>( 0, 0, 0 );

template<typename T>
const TVector3<T> TVector3<T>::One   = TVector3<T>( 1, 1, 1 );

template<typename T>
const TVector3<T> TVector3<T>::Up    = TVector3<T>( 0, 1, 0 );

template<typename T>
const TVector3<T> TVector3<T>::Down  = TVector3<T>( 0, -1, 0 );

template<typename T>
const TVector3<T> TVector3<T>::Left  = TVector3<T>( -1, 0, 0 );

template<typename T>
const TVector3<T> TVector3<T>::Right = TVector3<T>( 1, 0, 0 );

template<typename T>
const TVector3<T> TVector3<T>::Forward = TVector3<T>( 1, 0, -1 );

template<typename T>
const TVector3<T> TVector3<T>::Back    = TVector3<T>( 1, 0, 1 );

template<typename T>
const TVector3<T> TVector3<T>::BasisX  = TVector3<T>( 1, 0, 0 );

template<typename T>
const TVector3<T> TVector3<T>::BasisY  = TVector3<T>( 0, 1, 0 );

template<typename T>
const TVector3<T> TVector3<T>::BasisZ  = TVector3<T>( 0, 0, 1 );

template<typename T>
std::ostream& operator << ( std::ostream& stream, const TVector3<T>& v )
{
    stream << "<"  << v.x << ", " << v.y << ", " << v.z << ">";
    return stream;
}

/////////////////////////////////////////////////////////////////////////////
/// Templated Vector2 class
/////////////////////////////////////////////////////////////////////////////
template<typename T>
class TVector2
{
public:
    TVector2()
    {
    }

    TVector2( T vx, T vy )
        : x(vx), y(vy)
    {
    }

    explicit TVector2( T *pv )
        : x( pv[0] ), y( pv[1] )
    {
    }

    TVector2( const TVector2& v )
        : x(v.x), y(v.y)
    {
    }

    T& operator [] ( size_t offset )
    {
        math_assert( offset < 2 );
        return xy[offset];
    }

    const T& operator [] ( size_t offset ) const
    {
        math_assert( offset < 2 );
        return xy[offset];
    }

    T* c_ptr()
    {
        return &xy[0];
    }

    const T* c_ptr() const
    {
        return &xy[0];
    }

    TVector2& operator = ( const TVector2& rhs )
    {
        x = rhs.x;
        y = rhs.y;

        return *this;
    }

    bool operator == ( const TVector2& rhs ) const
    {
        return Math::CheckEquals( x, rhs.x ) &&
               Math::CheckEquals( y, rhs.y );
    }

    bool operator != ( const TVector2& rhs ) const
    {
        return !( *this == rhs );
    }

    bool operator < ( const TVector2& rhs ) const
    {
        return ((x < rhs.x) && (y < rhs.y));
    }

    bool operator <= ( const TVector2& rhs ) const
    {
        return ((x <= rhs.x) && (y <= rhs.y));
    }

    bool operator > ( const TVector2& rhs ) const
    {
        return ((x > rhs.x) && (y > rhs.y));
    }

    bool operator >= ( const TVector2& rhs ) const
    {
        return ((x >= rhs.x) && (y >= rhs.y));
    }

    TVector2 operator - () const
    {
        return TVector2( -x, -y );
    }

    TVector2 operator + ( const TVector2& rhs ) const
    {
        return TVector2( x + rhs.x,
                         y + rhs.y );
    }

    TVector2& operator += ( const TVector2& rhs )
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    TVector2 operator - ( const TVector2& rhs ) const
    {
        return TVector2( x - rhs.x,
                         y - rhs.y );
    }

    TVector2& operator -= ( const TVector2& rhs )
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    TVector2 operator * ( T rhs ) const
    {
        return TVector2( x * rhs, y * rhs );
    }

    TVector2& operator *= ( T rhs )
    {
        x *= rhs;
        y *= rhs;

        return *this;
    }

    TVector2 operator / ( T rhs ) const
    {
        return TVector2( x / rhs, y / rhs );
    }

    TVector2& operator /= ( T rhs )
    {
        x /= rhs;
        y /= rhs;

        return *this;
    }

    template<typename U>
    friend std::ostream& operator << ( std::ostream& s, const TVector2<U>& v );

    /**
     * Computes the dot product of this vector and the given vector rhs
     */
    T dot( const TVector2& rhs ) const
    {
        return x * rhs.x + y * rhs.y;
    }

    friend T cross( const TVector2& lhs, const TVector2& rhs )
    {
        return ( lhs.x * rhs.y - lhs.y * rhs.x );
    }

    /**
     * Returns the length of the vector. This is a somewhat expensive call,
     * since it must take the square root of the vector. Use lengthSquared
     * when possible instead.
     */
    T length() const
    {
        T slen = x * x + y * y;
        return sqrt( slen );
    }

    /**
     * Returns the squared length of the vector. This is significantly
     * faster than the length() call since it avoids a call to sqrt.
     */
    T lengthSquared() const
    {
        return x * x + y * y;
    }

    /**
     * Returns a normalized form of the vector. A normalized vector is one
     * in which its length is equal to one.
     */
    TVector2 normalized() const
    {
        T len = length();
        math_assert( len > static_cast<T>(0.0) );

        return TVector2( x / len,
                         y / len );
    }

    /**
     * Normalizes this vector. A normalized vector is one in which its length
     * is equal to one
     */
    TVector2& normalize()
    {
        T len = length();

        x /= len;
        y /= len;

        return *this;
    }

    /**
     * Checks if the vector is normalized
     */
    bool isNormalized() const
    {
        return Math::CheckEquals( length(), static_cast<T>( 1.0 ) );
    }

    /**
     * Checks if this is a zero vector. A vero vector is one in which
     * all components are equal to zero
     */
    bool isZero() const
    {
        return Math::CheckEquals( x, static_cast<T>( 0.0 ) ) &&
               Math::CheckEquals( y, static_cast<T>( 0.0 ) );
    }

    T X() const
    {
        return x;
    }

    T Y() const
    {
        return y;
    }

    static TVector2 Zero()
    {
        return TVector2( static_cast<T>( 0.0 ),
                         static_cast<T>( 0.0 ) );
    }

private:
    union
    {
        T xy[3];
        struct
        {
            T x, y;
        };
    };

};

template<typename T>
std::ostream& operator << ( std::ostream& stream, const TVector2<T>& v )
{
    stream << "<"  << v.x << ", " << v.y << ">";
    return stream;
}

typedef TVector4<float> Vec4;
typedef TVector3<float> Vec3;
typedef TVector2<float> Vec2;

typedef TVector4<int> IVec4;
typedef TVector3<int> IVec3;
typedef TVector2<int> IVec2;

#endif

