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
#ifndef SCOTT_COMMON_MATH_MATRIX_H
#define SCOTT_COMMON_MATH_MATRIX_H

#include <algorithm>
#include <functional>
#include <math/defs.h>
#include <math/util.h>

#if MATH_DEBUG_MODE == 1
#   include <cassert>
#   include <limits>
#   define MATRIX_DEBUG_MODE 1
#   define MATRIX_DV std::numeric_limits<T>::signaling_NaN()
#endif

#define M_OFFSET(R,C) ((R) * NUM_COLS + (C))

//
// Forward decalarations
//
template<typename T> class TMatrix4;

template<typename T> bool isZeroMatrix( const TMatrix4<T>& );
template<typename T> bool isIdentityMatrix( const TMatrix4<T>& );
template<typename T> TMatrix4<T> transpose( const TMatrix4<T>& );

/**
 * A templated, size-indepdent column major matrix. This templated matrix can
 * accomodate any N-sized square matrix that is sized NxN. The matrix stores
 * its values in column major format (each column is stored contigously),
 * rather than in row major format (where each row is stored contigously). This
 * is done because OpenGL uses column major format, and converting between the
 * two formats continously would be confusing.
 *
 * If you are using common matrix sizes (2x2, 3x3, 4x4), do not instantiate
 * this class directly. All of the common matrix sizes have their own classes
 * that inherit from this class and provide additional helper constructors. For
 * example, if you wish to construct a 4x4 float matrix,
 *
 *     DO  : TMatrix4<float>
 *     DONT: TMatrix<float,4>
 *
 * Additionally, while the matrices store their values internally in a column
 * major format, the constructors expect their values to be provided in row
 * major format. This might seem awkward and confusing, until you realize that
 * this allows you to call constructors with the "standard" matrix notation.
 *
 * Pretend you have a 3x3 matrix as follows
 *    
 *    M = [ A B C
 *          D E F
 *          G H I ]
 *
 * When calling Matrix3's constructor, you would call it like:
 *
 *    DO  : TMatrix3<float>( A, B, C,
 *                           D, E, F,
 *                           G, H, I )
 *
 * Even though the matrix stores the values like: [ A D G B E H C F I ]. The
 * only time you need concern yourself with this storage information is when:
 *    A) You call a TMatrix4<T>( T* ) constructor that takes a pointer to a
 *       1d array
 *    B) You call TMatrix4<T>::ptr() that returns a pointer to a 1d array
 *
 *
 *
 * That should be enough documentation to cover the matrix class. Otherwise the
 * matrix class (and derived classes) should behave as you expect. They
 * overload all the proper mathmatical operators, and implement the expected
 * helper methods. See method documentation for more information on individual
 * methods, or see the unit tests for examples on how they are used.
 */
template<typename T>
class TMatrix4
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

    /// Defines how many rows are in an instance of TMatrix4
    enum { NUM_ROWS = 4 };

    /// Defines how many cols are in an instance of TMatrix4
    enum { NUM_COLS = 4 };

    /// Defines how many total cells are in an instance of TMatrix4
    enum { NUM_VALUES = 16 };

    /**
     * Default constructor. Creates a new 4x4 matrix, but does not
     * initialize any of its values
     */
    TMatrix4()
#ifdef MATRIX_DEBUG_MODE
        : m00( MATRIX_DV ), m01( MATRIX_DV ), m02( MATRIX_DV ), m03( MATRIX_DV ),
          m04( MATRIX_DV ), m05( MATRIX_DV ), m06( MATRIX_DV ), m07( MATRIX_DV ),
          m08( MATRIX_DV ), m09( MATRIX_DV ), m10( MATRIX_DV ), m11( MATRIX_DV ),
          m12( MATRIX_DV ), m13( MATRIX_DV ), m14( MATRIX_DV ), m15( MATRIX_DV )
#endif
    {
    }


    /**
     * 4x4 matrix constructor. Arguments to the constructor are
     * in the normal "expected" (row major) format, even though the
     * underlying matrix class will store these values in column major
     * format.
     *
     * eg, to construct [ [a, b, c, d]
     *                    [e, f, g, h]
     *                    [i, j, k, l]
     *                    [m, n, o, p] ]
     *
     * just call TMatrix4( a, b, c, d,
     *                     e, f, g, h,
     *                     i, j, k, l,
     *                     m, n, o, p );
     *
     * and the matrix will store these values in memory as follows:
     *  [ a, e, i, m, b, f, j, n, c, g, k, o, d, h, l, p ]
     * 
     */
    TMatrix4( value_type m11, value_type m12, value_type m13, value_type m14,
              value_type m21, value_type m22, value_type m23, value_type m24,
              value_type m31, value_type m32, value_type m33, value_type m34,
              value_type m41, value_type m42, value_type m43, value_type m44 )
        : m00(m11), m01(m12), m02(m13), m03(m14),
          m04(m21), m05(m22), m06(m23), m07(m24),
          m08(m31), m09(m32), m10(m33), m11(m34),
          m12(m41), m13(m42), m14(m43), m15(m44)
    {
    }

    /**
     * Construct a matrix from an array of values. The 
     * constructor assumes the values are listed in column-major
     * rather than row-major format.
     *
     * IMPORTANT NOTE: This constructor expects the array in
     * column-major format!!! Not row major like the value constructors
     *
     * eg [ m11 m21 m23 m21 m22 ... ] rather than [ m11 m21 ... ]
     */
    explicit TMatrix4( const_pointer pVals )
        : m00( pVals[0]  ), m01( pVals[1]  ), m02( pVals[2]   ), m03( pVals[3]  ),
          m04( pVals[4]  ), m05( pVals[5]  ), m06( pVals[6]   ), m07( pVals[7]  ),
          m08( pVals[8]  ), m09( pVals[9]  ), m10( pVals[10] ),  m11( pVals[11] ),
          m12( pVals[12] ), m13( pVals[13] ), m14( pVals[14] ),  m15( pVals[15] )    
    {
    }

    /**
     * Matrix copy constructor
     */
    TMatrix4( const TMatrix4<T>& m )
        : m00( m.m00 ), m01( m.m01 ), m02( m.m02 ), m03( m.m03 ),
          m04( m.m04 ), m05( m.m05 ), m06( m.m06 ), m07( m.m07 ),
          m08( m.m08 ), m09( m.m09 ), m10( m.m10 ), m11( m.m11 ),
          m12( m.m12 ), m13( m.m13 ), m14( m.m14 ), m15( m.m15 )
    {
    }

    /**
     * Constant pointer to this matrix instance's underyling value array
     */
    const_pointer const_ptr()
    {
        return m;
    }

    /**
     * Constant pointer to this matrix instance's underyling value array
     */
    const_pointer ptr() const
    {
        return m;
    }

    /**
     * Pointer to this matrix instance's underyling value array
     */
    pointer ptr() 
    {
        return m;
    }

    /**
     * Index operator. Use this to directly read one of the matrix's values.
     * Note that the index is different depending on if the matrix is column
     * major or row major!
     */
    const_reference operator[] ( size_t offset ) const
    {
        math_assert( NUM_VALUES && "Matrix4 operator[] out of range" );
        return m[offset];
    }

    /**
     * Index operator. Use this to directly read one of the matrix's values.
     * Note that the index is different depending on if the matrix is column
     * major or row major!
     */
    reference operator[] ( size_t offset )
    {
        math_assert( NUM_VALUES && "Matrix4 operator[] out of range" );
        return m[offset];
    }

    /**
     * Assignment operator
     */
    TMatrix4<T>& operator = ( const TMatrix4<T>& rhs )
    {
        m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = rhs.m03;
        m04 = rhs.m04; m05 = rhs.m05; m06 = rhs.m06; m07 = rhs.m07;
        m08 = rhs.m08; m09 = rhs.m09; m10 = rhs.m10; m11 = rhs.m11;
        m12 = rhs.m12; m13 = rhs.m13; m14 = rhs.m14; m15 = rhs.m15;

        return *this;
    }

    /**
     * Matrix addition operator
     */
    TMatrix4<T> operator + ( const TMatrix4<T>& rhs ) const
    {
        return TMatrix4(
            m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
            m04 + rhs.m04, m05 + rhs.m05, m06 + rhs.m06, m07 + rhs.m07,
            m08 + rhs.m08, m09 + rhs.m09, m10 + rhs.m10, m11 + rhs.m11,
            m12 + rhs.m12, m13 + rhs.m13, m14 + rhs.m14, m15 + rhs.m15
        );
    }

    /**
     * Matrix self addition operator
     */
    TMatrix4<T>& operator += ( const TMatrix4<T>& rhs )
    {
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
        m04 += rhs.m04; m05 += rhs.m05; m06 += rhs.m06; m07 += rhs.m07;
        m08 += rhs.m08; m09 += rhs.m09; m10 += rhs.m10; m11 += rhs.m11;
        m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14; m15 += rhs.m15;

        return *this;
    }
    
    /**
     * Matrix subtraction operator
     */
    TMatrix4<T> operator - ( const TMatrix4<T>& rhs ) const
    {
        return TMatrix4(
            m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
            m04 - rhs.m04, m05 - rhs.m05, m06 - rhs.m06, m07 - rhs.m07,
            m08 - rhs.m08, m09 - rhs.m09, m10 - rhs.m10, m11 - rhs.m11,
            m12 - rhs.m12, m13 - rhs.m13, m14 - rhs.m14, m15 - rhs.m15
        );
    }

    /**
     * Matrix self subtraction operator
     */
    TMatrix4<T>& operator -= ( const TMatrix4<T>& rhs )
    {
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
        m04 -= rhs.m04; m05 -= rhs.m05; m06 -= rhs.m06; m07 -= rhs.m07;
        m08 -= rhs.m08; m09 -= rhs.m09; m10 -= rhs.m10; m11 -= rhs.m11;
        m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14; m15 -= rhs.m15;

        return *this;
    }

    /**
     * Matrix scalar multiplication operator
     */
    TMatrix4<T> operator * ( value_type rhs ) const
    {
        return TMatrix4(
            m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
            m04 * rhs, m05 * rhs, m06 * rhs, m07 * rhs,
            m08 * rhs, m09 * rhs, m10 * rhs, m11 * rhs,
            m12 * rhs, m13 * rhs, m14 * rhs, m15 * rhs
        );
    }

    /**
     * Matrix self scalar multiplication operator
     */
    TMatrix4<T>& operator *= ( value_type rhs )
    {
        m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
        m04 *= rhs; m05 *= rhs; m06 *= rhs; m07 *= rhs;
        m08 *= rhs; m09 *= rhs; m10 *= rhs; m11 *= rhs;
        m12 *= rhs; m13 *= rhs; m14 *= rhs; m15 *= rhs;

        return *this;
    }

    /**
     * Matrix multiplcation operator
     */
    TMatrix4<T> operator * ( const TMatrix4<T>& rhs ) const
    {
        return *this;
    }

    /**
     * Matrix self multiplication operator
     */
    TMatrix4<T>& operator *= ( const TMatrix4<T>& rhs )
    {
        return *this;
    }

    /**
     * Marix equality operator
     */
    bool operator == ( const TMatrix4<T>& rhs ) const
    {
#ifdef MATH_USE_FUZZY_EQUALS
        return std::equal( m, m + NUM_VALUES, rhs.m, Math::equalsClose<T> );
#else
        return std::equal( m, m + NUM_VALUES, rhs.m );
#endif
    }

    /**
     * Matrix inequality operator
     */
    bool operator != ( const TMatrix4<T>& rhs ) const
    {
        return !( *this == rhs );
    }

    /**
     * Returns the value at the given (r,c) matrix cell.
     */
    value_type at( size_t r, size_t c ) const
    {
        math_assert( r < NUM_ROWS && "Matrix row out of range");
        math_assert( c < NUM_COLS && "Matrix column out of range");

        return m[ M_OFFSET(r,c) ];
    }

    /**
     * Sets the value at the given (r,c) matrix cell.
     */
    void set( size_t r, size_t c, value_type v )
    {
        math_assert( r < NUM_ROWS && "Matrix row out of range");
        math_assert( c < NUM_COLS && "Matrix column out of range");

        m[ M_OFFSET(r,c) ] = v;
    }

    /**
     * Checks if all the entries in the matrix are zero
     */
    friend bool isZeroMatrix<>( const TMatrix4<T>& );

    /**
     * Checks if the gien matrix is an identity matrix
     */
    friend bool isIdentityMatrix<>( const TMatrix4<T>& );
        
    /**
     * Returns the transpose of this matrix
     */
    friend TMatrix4<T> transpose<>( const TMatrix4<T>& );

public:
    static const TMatrix4<T> ZERO_MATRIX;
    static const TMatrix4<T> IDENTITY;

protected:
    /// The matrix's data
    union
    {
        value_type m[NUM_VALUES];
        struct
        {
            value_type m00, m01, m02, m03;
            value_type m04, m05, m06, m07;
            value_type m08, m09, m10, m11;
            value_type m12, m13, m14, m15;
        };
    };
};

/////////////////////////////////////////////////////////////////////////////
// TMatrix4 utility methods and functions
/////////////////////////////////////////////////////////////////////////////
template<typename T>
bool isZeroMatrix( const TMatrix4<T>& m )
{
    bool status = true;

    for ( unsigned int i = 0; i < TMatrix4<T>::NUM_VALUES && status; ++i )
    {
        if (! Math::isZero<T>( m.m[i] ) )
        {
            status = false;
        }
    }

    return status;
}

template<typename T>
bool isIdentityMatrix( const TMatrix4<T>& m )
{
    using namespace Math;
    
    T v = static_cast<T>( 1 );

    return 
        equalsClose( m.m00, v ) && isZero( m.m01 ) && isZero( m.m02 ) && isZero( m.m03 ) &&
        isZero( m.m04 ) && equalsClose( m.m05, v ) && isZero( m.m06 ) && isZero( m.m07 ) &&
        isZero( m.m08 ) && isZero( m.m09 ) && equalsClose( m.m10, v ) && isZero( m.m11 ) &&
        isZero( m.m12 ) && isZero( m.m13 ) && isZero( m.m14 ) && equalsClose( m.m15, v );
}

template<typename T>
TMatrix4<T> transpose( const TMatrix4<T>& m )
{
    return TMatrix4<T>( m.m00, m.m04, m.m08, m.m12,
                        m.m01, m.m05, m.m09, m.m13,
                        m.m02, m.m06, m.m10, m.m14,
                        m.m03, m.m07, m.m11, m.m15 );
}

template<typename T>
std::ostream& operator << ( std::ostream& os, const TMatrix4<T>& m )
{
    os << "["
       << "[ " << m.at(0,0) << ", " << m.at(1,0) << ", "
               << m.at(2,0) << ", " << m.at(3,0) << " ], "
       << "[ " << m.at(0,1) << ", " << m.at(1,1) << ", "
               << m.at(2,1) << ", " << m.at(3,1) << " ], "
       << "[ " << m.at(0,2) << ", " << m.at(1,2) << ", "
               << m.at(2,2) << ", " << m.at(3,2) << " ], "
       << "[ " << m.at(0,3) << ", " << m.at(1,3) << ", "
               << m.at(2,3) << ", " << m.at(3,3) << " ] " << "[";

    return os;
}

/////////////////////////////////////////////////////////////////////////////
// Static definitions
/////////////////////////////////////////////////////////////////////////////
template<typename T>
const TMatrix4<T> TMatrix4<T>::ZERO_MATRIX = TMatrix4(
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
);

template<typename T>
const TMatrix4<T> TMatrix4<T>::IDENTITY = TMatrix4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);

/////////////////////////////////////////////////////////////////////////////
// Common typedefs
/////////////////////////////////////////////////////////////////////////////
typedef TMatrix4<float> Mat4;

/////////////////////////////////////////////////////////////////////////////
// Macro clean up
/////////////////////////////////////////////////////////////////////////////
#if MATH_DEBUG_MODE == 1
#   undef MATRIX_DEBUG_MODE
#   undef MATRIX_DV
#endif

#undef M_OFFSET
#undef INNER_PRODUCT

#endif
