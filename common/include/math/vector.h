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

#include <ostream>

#if MATH_DEBUG_MODE == 1
#   include <cassert>
#   include <limits>
#   define VECTOR_DEBUG_MODE 1
#endif

/////////////////////////////////////////////////////////////////////////////
// Forward declarations
//  - Need to forward declare all of our vector functions because C++ is
//    incapable of understanding a friend decalartion without already having
//    the function definition
/////////////////////////////////////////////////////////////////////////////
template<typename T> class TVector4;
template<typename T> class TVector3;
template<typename T> class TVector2;

template<typename T> T dot( const TVector3<T>& lhs, const TVector3<T>& rhs );
template<typename T> T dot( const TVector4<T>& lhs, const TVector3<T>& rhs );
template<typename T> TVector3<T> cross( const TVector3<T>& lhs, const TVector3<T>& rhs );
template<typename T> T length( const TVector3<T>& v );
template<typename T> T length( const TVector4<T>& v );
template<typename T> T lengthSquared( const TVector3<T>& v );
template<typename T> T lengthSquared( const TVector4<T>& v );
template<typename T> TVector3<T> normalized( const TVector3<T>& v );
template<typename T> TVector4<T> normalized( const TVector4<T>& v );
template<typename T> TVector3<T> rotateAroundX( const TVector3<T>& v, T angle );
template<typename T> TVector3<T> rotateAroundY( const TVector3<T>& v, T angle );
template<typename T> TVector3<T> rotateAroundZ( const TVector3<T>& v, T angle );
template<typename T> TVector3<T> rotateAround( const TVector3<T>& v,
                                               const TVector3<T>& axis,
                                               T angle );
                                                
template<typename T> T angleBetween( const TVector3<T>& lhs,
                                     const TVector3<T>& rhs );

template<typename T>
class TVector4
{
public:
    // Type traits
    typedef T value_type;
    typedef value_type const const_value_type;
    typedef value_type& reference;
    typedef const_value_type& const_reference;
    typedef value_type* pointer;
    typedef const_value_type* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // Constants
    enum { NUM_COMPONENTS = 4 };

    /**
     * Standard vector constructor. Follows C++ standards of not
     * initializing basic value types, so do not assume the vector
     * is created with values set to zero.
     *
     * If you need a zero vector, call the static method ::ZeroVector()
     */
    TVector4()
#ifdef VECTOR_DEBUG_MODE
        : mX( std::numeric_limits<T>::signaling_NaN() ),
          mY( std::numeric_limits<T>::signaling_NaN() ),
          mZ( std::numeric_limits<T>::signaling_NaN() ),
          mW( std::nuermic_limits<T>::signaling_NaN() )
#endif
    {
    }

    /**
     * Copy-initialize vector from a pointer to an array of values.
     * This constructor expects that the passed array contains at 
     * least four values, but there is nothing to check this constraint
     */
    explicit TVector4( const_pointer pVals )
        : mX( pVals[0] ), mY( pVals[1] ), mZ( pVals[2] ), mW( pVals[3] )
    {
    }

    /**
     * Vector x/y/z constructor. Takes the provided x/y/z values and
     * assigns it to the newly constructed vector.
     */
    TVector4( value_type x, value_type y, value_type z, value_type w )
        : mX( x ), mY( y ), mZ( z ), mW( w )
    {
    }

    /**
     * Copy constructor
     */
    TVector4( const TVector4<T>& v )
        : mX( v.mX ), mY( v.mY ), mZ( v.mZ ), mW( v.mW )
    {
    }

    /**
     * Read-only index operator, used to read one of the vector's four
     * cardinal axis values
     *
     * \param  index  The vector's component index (0 to 3)
     * \return        Value of the component in the vector
     */
    const_reference operator [] ( unsigned int index ) const
    {
        math_assert( index < NUM_COMPONENTS && "Vector operator[] out of range" );
        return *(&mX + index);
    }

    /**
     * Vector index operator, used to read one of the vector's four
     * cardinal axis values
     *
     * \param  index  The vector's component index (0 to 3)
     * \return        Value of the component in the vector
     */
    reference operator [] ( unsigned int index )
    {
        math_assert( index < NUM_COMPONENTS && "Vector operator[] out of range" );
        return *(&mX + index);
    }

    /**
     * Returns a pointer to the first element in the vector. Useful for
     * when a legacy API takes a 4D vector as a T*
     */
    pointer ptr()
    {
        return &mX;
    }

    /**
     * Returns a constant pointer to the first element in the vector. Useful
     * for when a legacy API takes a 4D vector as a const T*
     */
    const_pointer ptr() const
    {
        return &mX;
    }

    /**
     * Returns a constant pointer to the first element in the vector. Useful
     * for when a legacy API takes a 4D vector as a const T*
     */
    const_pointer const_ptr() const
    {
        return &mX;
    }

    /**
     * Assignment operator. Copy values from the right hand side and assign
     * it to this vector
     */
    TVector4<T>& operator = ( const TVector4<T>& rhs )
    {
        mX = rhs.mX;
        mY = rhs.mY;
        mZ = rhs.mZ;
        mW = rhs.mW;

        return *this;
    }

    /**
     * Equality operator
     */
    bool operator == ( const TVector4<T>& rhs ) const
    {
#ifdef MATH_USE_FUZZY_EQUALS
    return ( Math::equalsClose( mX, rhs.mX ) &&
             Math::equalsClose( mY, rhs.mY ) &&
             Math::equalsClose( mZ, rhs.mZ ) &&
             Math::equalsClose( mW, rhs.mW ) );
#else
    return ( mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW = rhs.mW );
#endif
    }

    /**
     * Inequality operator
     */
    bool operator != ( const TVector4<T>& rhs ) const
    {
        return !( *this == rhs );
    }

    /**
     * Unary negation operator. Negates all components of the value, which is
     * the same as multiplying all components by -1.0.
     */
    friend TVector4<T> operator - ( const TVector4<T>& rhs )
    {
        return TVector4( -rhs.mX, -rhs.mY, -rhs.mZ, -rhs.mW );
    }

    /**
     * Vector multiplication operator. Scales the vector by the given vector,
     * which is identical to multiplying all of the vectors components by the
     * scalar.
     */
    friend TVector4<T> operator * ( const TVector4<T>& rhs, 
                                    value_type scalar )
    {
        return TVector4( rhs.mX * scalar, 
                         rhs.mY * scalar, 
                         rhs.mZ * scalar,
                         rhs.mW * scalar );
    }

    /**
     * Component wise addition operator
     */
    friend TVector4<T> operator + ( const TVector4<T>& lhs,
                                    const TVector4<T>& rhs )
    {
        return TVector4( lhs.mX + rhs.mX,
                         lhs.mY + rhs.mY,
                         lhs.mZ + rhs.mZ,
                         lhs.mW + rhs.mW );
    }

    /**
     * Component wise subtraction operator
     */
    friend TVector4<T> operator - ( const TVector4<T>& lhs,
                                    const TVector4<T>& rhs )
    {
        return TVector4<T>( lhs.mX - rhs.mX,
                            lhs.mY - rhs.mY,
                            lhs.mZ - rhs.mZ,
                            lhs.mW - rhs.mW );
    }

    /**
     * Component wise self addition operator
     */
    TVector4<T>& operator += ( const TVector4<T>& rhs )
    {
        mX += rhs.mX;
        mY += rhs.mY;
        mZ += rhs.mZ;
        mW += rhs.mW;

        return *this;
    }

    /**
     * Component wise self subtraction operator
     */
    TVector4<T>& operator -= ( const TVector4<T>& rhs )
    {
        mX -= rhs.mX;
        mY -= rhs.mY;
        mZ -= rhs.mZ;
        mW -= rhs.mW;

        return *this;
    }

    /**
     * Component wise scalar self multiplication operator
     */
    TVector4<T>& operator *= ( value_type rhs )
    {
        mX *= rhs;
        mY *= rhs;
        mZ *= rhs;
        mW *= rhs;

        return *this;
    }

    /**
     * Return the value of the vector's X component
     */
    inline value_type x() const
    {
        return mX;
    }

    /**
     * Return the value of the vector's Y component
     */
    inline value_type y() const
    {
        return mY;
    }

    /**
     * Return the value of the vector's Z component
     */
    inline value_type z() const
    {
        return mZ;
    }

    /**
     * Return the value of the vector's W component
     */
    inline value_type w() const
    {
        return mW;
    }

    /**
     * Returns a vector containing entirely zero
     */
    static TVector4<T> ZeroVector()
    {
        return TVector4<T>( static_cast<T>(0), 
                            static_cast<T>(0),
                            static_cast<T>(0),
                            static_cast<T>(0) );
    }

    /**
     * Returns the length (magnitude) of this vector.
     */
    friend value_type length<>( const TVector4<T>& v );

    /**
     * Returns the length squared of the vector (no sqrt)
     */
    friend value_type lengthSquared<>( const TVector4<T>& v );

    /**
     * Returns a normalized version of this vector
     */
    friend TVector4<T> normalized<>( const TVector4<T>& v );

private:
    union
	{
		struct { value_type mX, mY, mZ, mW; };
		struct { value_type v[NUM_COMPONENTS]; };
	};
};

//////////////////////////////////////////////////////////////////////

/**
 * Generic templated 3d graphics vector class. Implements all commonly
 * used functionality, including vector rotations and projections.
 */
template<typename T>
class TVector3
{
public:
    // Type traits
    typedef T value_type;
    typedef value_type const const_value_type;
    typedef value_type& reference;
    typedef const_value_type& const_reference;
    typedef value_type* pointer;
    typedef const_value_type* const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // Constants
    enum { NUM_COMPONENTS = 3 };

    /**
     * Standard vector constructor. Follows C++ standards of not
     * initializing basic value types, so do not assume the vector
     * is created with values set to zero.
     *
     * If you need a zero vector, call the static method ::ZeroVector()
     */
    TVector3()
#ifdef VECTOR_DEBUG_MODE
        : mX( std::numeric_limits<T>::signaling_NaN() ),
          mY( std::numeric_limits<T>::signaling_NaN() ),
          mZ( std::numeric_limits<T>::signaling_NaN() )
#endif
    {
    }

    /**
     * Copy-initialize vector from a pointer to an array of values.
     * This constructor expects that the passed array contains at 
     * least four values, but there is nothing to check this constraint
     */
    explicit TVector3( const_pointer pVals )
        : mX( pVals[0] ), mY( pVals[1] ), mZ( pVals[2] )
    {
    }

    /**
     * Vector x/y/z constructor. Takes the provided x/y/z values and
     * assigns it to the newly constructed vector.
     */
    TVector3( value_type x, value_type y, value_type z )
        : mX( x ), mY( y ), mZ( z )
    {
    }

    /**
     * Copy constructor
     */
    TVector3( const TVector3<T>& v )
        : mX( v.mX ), mY( v.mY ), mZ( v.mZ )
    {
    }

    /**
     * Read-only index operator, used to read one of the vector's three 
     * cardinal axis values
     *
     * \param  index  The vector's component index (0 to 2)
     * \return        Value of the component in the vector
     */
    const_reference operator [] ( unsigned int index ) const
    {
        math_assert( index < NUM_COMPONENTS && "Vector operator[] out of range" );
        return *(&mX + index);
    }

    /**
     * Vector index operator, used to read one of the vector's three
     * cardinal axis values
     *
     * \param  index  The vector's component index (0 to 2)
     * \return        Value of the component in the vector
     */
    reference operator [] ( unsigned int index )
    {
        math_assert( index < NUM_COMPONENTS && "Vector operator[] out of range" );
        return *(&mX + index);
    }

    /**
     * Returns a pointer to the first element in the vector. Useful for
     * when a legacy API takes a 3D vector as a T*
     */
    pointer ptr()
    {
        return &mX;
    }

    /**
     * Returns a constant pointer to the first element in the vector. Useful
     * for when a legacy API takes a 3D vector as a const T*
     */
    const_pointer ptr() const
    {
        return &mX;
    }

    /**
     * Returns a constant pointer to the first element in the vector. Useful
     * for when a legacy API takes a 4D vector as a const T*
     */
    const_pointer const_ptr() const
    {
        return &mX;
    }
    /**
     * Assignment operator. Copy values from the right hand side and assign
     * it to this vector
     */
    TVector3<T>& operator = ( const TVector3<T>& rhs )
    {
        mX = rhs.mX;
        mY = rhs.mY;
        mZ = rhs.mZ;

        return *this;
    }

    /**
     * Equality operator
     */
    bool operator == ( const TVector3<T>& rhs ) const
    {
#ifdef MATH_USE_FUZZY_EQUALS
    return ( Math::equalsClose( mX, rhs.mX ) &&
             Math::equalsClose( mY, rhs.mY ) &&
             Math::equalsClose( mZ, rhs.mZ ) );
#else
    return ( mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ );
#endif
    }

    /**
     * Inequality operator
     */
    bool operator != ( const TVector3<T>& rhs ) const
    {
        return !( *this == rhs );
    }

    /**
     * Unary negation operator. Negates all components of the value, which is
     * the same as multiplying all components by -1.0.
     */
    friend TVector3<T> operator - ( const TVector3<T>& rhs )
    {
        return TVector3( -rhs.mX, -rhs.mY, -rhs.mZ );
    }

    /**
     * Vector multiplication operator. Scales the vector by the given vector,
     * which is identical to multiplying all of the vectors components by the
     * scalar.
     */
    friend TVector3<T> operator * ( const TVector3<T>& rhs, 
                                    value_type scalar )
    {
        return TVector3( rhs.mX * scalar, 
                         rhs.mY * scalar, 
                         rhs.mZ * scalar );
    }

    /**
     * Component wise addition operator
     */
    friend TVector3<T> operator + ( const TVector3<T>& lhs,
                                    const TVector3<T>& rhs )
    {
        return TVector3( lhs.mX + rhs.mX,
                         lhs.mY + rhs.mY,
                         lhs.mZ + rhs.mZ );
    }

    /**
     * Component wise subtraction operator
     */
    friend TVector3<T> operator - ( const TVector3<T>& lhs,
                                    const TVector3<T>& rhs )
    {
        return TVector3<T>( lhs.mX - rhs.mX,
                            lhs.mY - rhs.mY,
                            lhs.mZ - rhs.mZ );
    }

    /**
     * Component wise self addition operator
     */
    TVector3<T>& operator += ( const TVector3<T>& rhs )
    {
        mX += rhs.mX;
        mY += rhs.mY;
        mZ += rhs.mZ;

        return *this;
    }

    /**
     * Component wise self subtraction operator
     */
    TVector3<T>& operator -= ( const TVector3<T>& rhs )
    {
        mX -= rhs.mX;
        mY -= rhs.mY;
        mZ -= rhs.mZ;

        return *this;
    }

    /**
     * Component wise scalar self multiplication operator
     */
    TVector3<T>& operator *= ( value_type rhs )
    {
        mX *= rhs;
        mY *= rhs;
        mZ *= rhs;

        return *this;
    }

    /**
     * Return the value of the vector's X component
     */
    inline value_type x() const
    {
        return mX;
    }

    /**
     * Return the value of the vector's Y component
     */
    inline value_type y() const
    {
        return mY;
    }

    /**
     * Return the value of the vector's Z component
     */
    inline value_type z() const
    {
        return mZ;
    }

    /**
     * Returns a vector containing entirely zero
     */
    static TVector3<T> ZeroVector()
    {
        return TVector3( static_cast<T>(0), 
                         static_cast<T>(0),
                         static_cast<T>(0) );
    }

    /**
     * Rotates the vector around the X axis, and returns the result
     * of this rotation
     */
    friend TVector3<T> rotateAroundX<>( const TVector3<T>& v,
                                        value_type angle );

    /**
     * Rotates the vector around the Y axis, and returns the result
     * of this rotation
     */
    friend TVector3<T> rotateAroundY<>( const TVector3<T>& v,
                                        value_type angle );

    /**
     * Rotates the vector around the Z axis, and returns the result
     * of the rotation
     */
    friend TVector3<T> rotateAroundZ<>( const TVector3<T>& v,
                                        value_type angle );

    /**
     * Rotate around an arbitrary axis. We can easily accomplish
     * this with a matrix, however for the sake of not depending on a
     * matrix implementation we'll fake it
     */
    friend TVector3<T> rotateAround<>( const TVector3<T>& v,
                                       const TVector3<T>& axis,
                                       value_type angle );

    /**
     * Cross product. Returns the result of crossing the lhs vector with
     * the rhs vector
     */
    friend TVector3<T> cross<>( const TVector3<T>& lhs,
                                const TVector3<T>& rhs );

    /**
     * Dot product. Returns the result of dotting the lhs vector with the 
     * rhs vector
     */
    friend value_type dot<>( const TVector3<T>& lhs, const TVector3<T>& rhs );

    /**
     * Returns the angle between the lhs vector and the rhs vector
     */
    friend value_type angleBetween<>( const TVector3<T>& lhs,
                                      const TVector3<T>& rhs );

    /**
     * Returns the length (magnitude) of the vector
     */
    friend value_type length<>( const TVector3<T>& v );

    /**
     * Returns the length squared of the vector (no sqrt)
     */
    friend value_type lengthSquared<>( const TVector3<T>& v );

    /**
     * Returns a normalized verison of this vector
     */
    friend TVector3<T> normalized<>( const TVector3<T>& v );

private:
    union
	{
		struct { value_type mX, mY, mZ; };
		struct { value_type v[NUM_COMPONENTS]; };
	};
};

/////////////////////////////////////////////////////////////////////////////
// Vector operations
/////////////////////////////////////////////////////////////////////////////
template<typename T>
TVector3<T> cross ( const TVector3<T>& lhs,
                    const TVector3<T>& rhs )
{
    return TVector3<T>( lhs.mY * rhs.mZ - rhs.mY * lhs.mZ,
                        lhs.mZ * rhs.mX - rhs.mZ * lhs.mX,
                        lhs.mX * rhs.mY - rhs.mX * lhs.mY );
}

template<typename T>
T dot ( const TVector3<T>& lhs, const TVector3<T>& rhs )
{
    return lhs.mX * rhs.mX + lhs.mY * rhs.mY + lhs.mZ * rhs.mZ;
}

template<typename T> T angleBetween( const TVector3<T>& lhs, const TVector3<T>& rhs );
template<> float angleBetween( const TVector3<float>& lhs, const TVector3<float>& rhs );

template<typename T> T length( const TVector3<T>& v );  // must specialize
template<> float length( const TVector3<float>& v );

template<typename T> T length( const TVector4<T>& v );
template<> float length( const TVector4<float>& v );

template<typename T>
T lengthSquared( const TVector3<T>& v )
{
    return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ;
}

template<typename T>
T lengthSquared( const TVector4<T>& v )
{
    return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW;
}

template<typename T> TVector3<T> normalized( const TVector3<T>& v );
template<> TVector3<float> normalized( const TVector3<float>& v );

template<typename T> TVector4<T> normalized( const TVector4<T>& v );
template<> TVector4<float> normalized( const TVector4<float>& v );

template<typename T> TVector3<T> rotateAroundX( const TVector3<T>& v, T angle );
template<> TVector3<float> rotateAroundX( const TVector3<float>& v, float angle );

template<typename T> TVector3<T> rotateAroundY( const TVector3<T>& v, T angle );
template<> TVector3<float> rotateAroundY( const TVector3<float>& v, float angle );

template<typename T> TVector3<T> rotateAroundZ( const TVector3<T>& v, T angle );
template<> TVector3<float> rotateAroundZ( const TVector3<float>& v, float angle );

template<typename T> TVector3<T> rotateAround( const TVector3<T>& v,
                                               const TVector3<T>& axis,
                                               T angle );
template<> TVector3<float> rotateAround( const TVector3<float>& v,
                                         const TVector3<float>& axis,
                                         float angle );

template<typename T>
std::ostream& operator << ( std::ostream& os, const TVector3<T>& v )
{
    os << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
    return os;
}

/////////////////////////////////////////////////////////////////////////////
// Vector typedefs - typedef common vector types
/////////////////////////////////////////////////////////////////////////////
typedef TVector3<long>   IVec;
typedef TVector3<float>  Vec3;
typedef TVector3<float>  Vec3f;

#endif
