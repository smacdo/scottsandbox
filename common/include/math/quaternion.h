/*
 * Copyright 2010-2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_MATH_QUATERNION_H
#define SCOTT_MATH_QUATERNION_H

#include <math/defs.h>
#include <math/vector.h>

// Forward declarations
template<typename T> class TQuaternion;

template<typename T> T magnitude( const TQuaternion<T>& q );
template<typename T> TQuaternion<T> conjugate( const TQuaternion<T>& q );
template<typename T> TQuaternion<T> inverse( const TQuaternion<T>& q );
template<typename T> TQuaternion<T> normalize( const TQuaternion<T>& q );

/**
 * Quaternions are used to represent rotations in the game engine. Quaternions
 * are compact, do not suffer from gimbal lock and are easily interpolated.
 *
 * However, quaternions suffer from a significant downside - they are not
 * easily understood by looking at their value. The codebase is designed in
 * such a way that you should never need to manually create or read a
 * quaternion. Instead the user should simply use any number of utility
 * functions that can generate quarternions for you
 */
template<typename T>
class TQuaternion
{
public:
    // Public typedefs
    typedef T value_type;
    typedef value_type const const_value_type;
    typedef value_type& reference;
    typedef const_value_type& const_reference;
    typedef value_type* pointer;
    typedef const_value_type* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    /**
     * Quaternion default constructor
     */
    TQuaternion()
#ifdef MATH_DEBUG_MODE
        : x( std::numeric_limits<T>::signaling_NaN() ),
          y( std::numeric_limits<T>::signaling_NaN() ),
          z( std::numeric_limits<T>::signaling_NaN() ),
          w( std::numeric_limits<T>::signaling_NaN() )
#endif
    {
    }

    /**
     * Quaternion constructor
     */
    TQuaternion( value_type x, value_type y, value_type z, value_type w )
        : mX( x ),
          mY( y ),
          mZ( z ),
          mW( w )
    {
    }

    /**
     * Copy constructor
     */
    TQuaternion( const TQuaternion<T>& q )
        : mX( q.mX ),
          mY( q.mY ),
          mZ( q.mZ ),
          mW( q.mW )
    {
    }

    /**
     * Assignment operator
     */
    TQuaternion<T>& operator =  ( const TQuaternion<T>& q )
    {
        mX = q.mX;
        mY = q.mY;
        mZ = q.mZ;
        mW = q.mW;

        return *this;
    }

    /**
     * Equality operator
     */
    bool operator == ( const TQuaternion<T>& q ) const
    {
        return Math::equalsClose( mX, q.mX ) &&
               Math::equalsClose( mY, q.mY ) &&
               Math::equalsClose( mZ, q.mZ ) &&
               Math::equalsClose( mW, q.mW );
    }

    /**
     * Inequality operator
     */
    bool operator != ( const TQuaternion& rhs ) const
    {
        return !( *this == rhs );
    }

    /**
     * Index operator
     */
    const_reference operator[]( unsigned int index ) const
    {
        math_assert( index < 4 && "Quaternion index out of range" );
        return *(&mX + index);
    }

    /**
     * Index operator
     */
    reference operator[]( unsigned int index )
    {
        math_assert( index < 4 && "Quaternion index out of range" );
        return *(&mX + index);
    }

    /**
     * Multiplication operator
     */
    TQuaternion<T> operator * ( const TQuaternion<T>& rhs ) const
    {
        return TQuaternion<T>(
                mW * rhs.mX + mX * rhs.mW + mY * rhs.mZ - mZ * rhs.mY,
                mW * rhs.mY + mY * rhs.mW + mZ * rhs.mX - mX * rhs.mZ,
                mW * rhs.mZ + mZ * rhs.mW + mX * rhs.mY - mY * rhs.mX,
                mW * rhs.mW - mX * rhs.mX - mY * rhs.mY - mZ * rhs.mZ
        );
    }

    /**
     * Self multiplication operator
     */
    TQuaternion<T>& operator *= ( const TQuaternion<T>& rhs )
    {
        mX = mW * rhs.mX + mX * rhs.mW + mY * rhs.mZ - mZ * rhs.mY;
        mY = mW * rhs.mY + mY * rhs.mW + mZ * rhs.mX - mX * rhs.mZ;
        mZ = mW * rhs.mZ + mZ * rhs.mW + mX * rhs.mY - mY * rhs.mX;
        mW = mW * rhs.mW - mX * rhs.mX - mY * rhs.mY - mZ * rhs.mZ;

        return *this;
    }

    /**
     * Returns a vector3 of euler angles that are equivilant to this
     * quaternion in value
     */
    TVector3<T> eulerAngles() const
    {
        return TVector3<T>::IDENTITY;
    }

    /**
     * X component
     */
    value_type x() const
    {
        return mX;
    }

    /**
     * Y component
     */
    value_type y() const
    {
        return mY;
    }

    /**
     * Z component
     */
    value_type z() const
    {
        return mZ;
    }

    /**
     * W component
     */
    value_type w() const
    {
        return mW;
    }

    // Calculate the magnitude of a quaternion
    friend value_type magnitude<>( const TQuaternion<T>& q );

    // Calculate the conjugate of a quaternion
    friend TQuaternion<T> conjugate<>( const TQuaternion<T>& q );

    // Calculate the inverse of a quaternion
    friend TQuaternion<T> inverse<>( const TQuaternion<T>& q );

    // Normalize a quaternion
    friend TQuaternion<T> normalize<>( const TQuaternion<T>& q );

public:
    static const TQuaternion<T> ZERO;
    static const TQuaternion<T> IDENTITY;

private:
    value_type mX, mY, mZ, mW; 

private:
    friend class boost::serialization::access;

    /**
     * Serialization
     */
    template<typename Archive>
    void serialize( Archive& ar, const unsigned int version )
    {
        ar & mX & mY & mZ & mW;
    }
};

/**
 * Calculates and returns the magnitude of the quaternion
 *
 * TODO: Break this out into a .cpp to avoid the cmath include
 */
template<typename T>
T magnitude( const TQuaternion<T>& q )
{
    return( std::sqrt( q.mX * q.mX + q.mY * q.mY +
                       q.mZ * q.mZ + q.mW * q.mW ) );
}

/**
 * Calculates and returns the quaternion's conjugate
 */
template<typename T>
TQuaternion<T> conjugate( const TQuaternion<T>& q )
{
    return TQuaternion<T>( -q.mX, -q.mY, -q.mZ, q.mW );
}

/**
 * Calculates and returns the quaternion's inverse
 */
template<typename T>
TQuaternion<T> inverse( const TQuaternion<T>& q )
{
    TQuaternion<T> t = conjugate( q );
    return TQuaternion<T>( static_cast<T>(1) / t.mX,
                           static_cast<T>(1) / t.mY,
                           static_cast<T>(1) / t.mZ,
                           static_cast<T>(1) / t.mW );
}

/**
 * Normalize a quaternion
 */
template<typename T>
TQuaternion<T> normalize( const TQuaternion<T>& q )
{
    T mag = magnitude( q );
    return TQuaternion<T>( mag / q.mX, mag / q.mY, mag / q.mZ, mag / q.mW );
}

/**
 * Console stream output
 */
template<typename T>
std::ostream& operator << ( std::ostream& os, const TQuaternion<T>& q )
{
    os << "{quat; "
       << "x: " << q.x() << ", "
       << "y: " << q.y() << ", "
       << "z: " << q.z() << ", "
       << "w: " << q.w()
       << "}";

    return os;
}

/**
 * Zero quaternion (all components are zero)
 */
template<typename T>
const TQuaternion<T> TQuaternion<T>::ZERO = TQuaternion<T>( 0, 0, 0, 0 );

/**
 * Identity quaternion
 */
template<typename T>
const TQuaternion<T> TQuaternion<T>::IDENTITY = TQuaternion<T>( 0, 0, 0, 1 );

#ifdef MATH_COMMON_TYPEDEFS
typedef TQuaternion<float> Quat;
typedef TQuaternion<double> Quatd;
#endif

#endif
