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

#define M_OFFSET(R,C) ((C) * NUM_ROWS + (R))


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
        : m11( MATRIX_DV ), m12( MATRIX_DV ), m13( MATRIX_DV ), m14( MATRIX_DV ),
          m21( MATRIX_DV ), m22( MATRIX_DV ), m23( MATRIX_DV ), m24( MATRIX_DV ),
          m31( MATRIX_DV ), m32( MATRIX_DV ), m33( MATRIX_DV ), m34( MATRIX_DV ),
          m41( MATRIX_DV ), m42( MATRIX_DV ), m43( MATRIX_DV ), m44( MATRIX_DV )
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
    TMatrix4( value_type m11, value_type m21, value_type m31, value_type m41,
              value_type m12, value_type m22, value_type m32, value_type m42,
              value_type m13, value_type m23, value_type m33, value_type m43,
              value_type m14, value_type m24, value_type m34, value_type m44 )
        : m11(m11), m12(m21), m13(m31), m14(m41),
          m21(m12), m22(m22), m23(m32), m24(m42),
          m31(m13), m32(m23), m33(m33), m34(m43),
          m41(m14), m42(m24), m43(m34), m44(m44)
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
        : m11( pVals[0]  ), m12( pVals[1]  ), m13( pVals[2]   ), m14( pVals[3]  ),
          m21( pVals[4]  ), m22( pVals[5]  ), m23( pVals[6]   ), m24( pVals[7]  ),
          m31( pVals[8]  ), m32( pVals[9]  ), m33( pVals[10] ),  m34( pVals[11] ),
          m41( pVals[12] ), m42( pVals[13] ), m43( pVals[14] ),  m44( pVals[15] )    
    {
        math_assert( pVals != NULL && "Cannot construct from null pointer" );
    }

    /**
     * Matrix copy constructor
     */
    TMatrix4( const TMatrix4<T>& m )
         : m11( m.m11 ), m12( m.m12 ), m13( m.m13 ), m14( m.m14 ),
           m21( m.m21 ), m22( m.m22 ), m23( m.m23 ), m24( m.m24 ),
           m31( m.m31 ), m32( m.m32 ), m33( m.m33 ), m34( m.m34 ),
           m41( m.m41 ), m42( m.m42 ), m43( m.m43 ), m44( m.m44 )
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
        m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13; m14 = rhs.m14;
        m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23; m24 = rhs.m24;
        m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33; m34 = rhs.m34;
        m41 = rhs.m41; m42 = rhs.m42; m43 = rhs.m43; m44 = rhs.m44;

        return *this;
    }

    /**
     * Matrix addition operator
     */
    TMatrix4<T> operator + ( const TMatrix4<T>& rhs ) const
    {
        return TMatrix4(
                m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13, m14 + rhs.m14,
                m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23, m24 + rhs.m24,
                m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33, m34 + rhs.m34,
                m41 + rhs.m41, m42 + rhs.m42, m43 + rhs.m43, m44 + rhs.m44
        );
    }

    /**
     * Matrix self addition operator
     */
    TMatrix4<T>& operator += ( const TMatrix4<T>& rhs )
    {
        m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14;
        m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23; m24 += rhs.m24;
        m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33; m34 += rhs.m34;
        m41 += rhs.m41; m42 += rhs.m42; m43 += rhs.m43; m44 += rhs.m44;

        return *this;
    }
    
    /**
     * Matrix subtraction operator
     */
    TMatrix4<T> operator - ( const TMatrix4<T>& rhs ) const
    {
        return TMatrix4(
                m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13, m14 - rhs.m14,
                m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23, m24 - rhs.m24,
                m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33, m34 - rhs.m34,
                m41 - rhs.m41, m42 - rhs.m42, m43 - rhs.m43, m44 - rhs.m44
        );
    }

    /**
     * Matrix self subtraction operator
     */
    TMatrix4<T>& operator -= ( const TMatrix4<T>& rhs )
    {
        m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14;
        m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23; m24 -= rhs.m24;
        m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33; m34 -= rhs.m34;
        m41 -= rhs.m41; m42 -= rhs.m42; m43 -= rhs.m43; m44 -= rhs.m44;

        return *this;
    }

    /**
     * Matrix scalar multiplication operator
     */
    TMatrix4<T> operator * ( value_type rhs ) const
    {
        return TMatrix4(
                m11 * rhs, m12 * rhs, m13 * rhs, m14 * rhs,
                m21 * rhs, m22 * rhs, m23 * rhs, m24 * rhs,
                m31 * rhs, m32 * rhs, m33 * rhs, m34 * rhs,
                m41 * rhs, m42 * rhs, m43 * rhs, m44 * rhs
        );
    }

    /**
     * Matrix self scalar multiplication operator
     */
    TMatrix4<T>& operator *= ( value_type rhs )
    {
        m11 *= rhs; m12 *= rhs; m13 *= rhs; m14 *= rhs;
        m21 *= rhs; m22 *= rhs; m23 *= rhs; m24 *= rhs;
        m31 *= rhs; m32 *= rhs; m33 *= rhs; m34 *= rhs;
        m41 *= rhs; m42 *= rhs; m43 *= rhs; m44 *= rhs;

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
/*        struct
        {
            value_type m0,  m1,  m2,  m3;
            value_type m4,  m5,  m6,  m7;
            value_type m8,  m9,  m10, m11;
            value_type m12, m13, m14, m15;
        };*/
        /*
        struct // row major
        {
            value_type m11, m21, m31, m41;
            value_type m12, m22, m32, m42;
            value_type m13, m23, m33, m43;
            value_type m14, m24, m34, m44;
        };
        */
        struct // column major
        {
            value_type m11, m12, m13, m14;
            value_type m21, m22, m23, m24;
            value_type m31, m32, m33, m34;
            value_type m41, m42, m43, m44;
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
        equalsClose( m.m11, v ) && isZero( m.m12 ) && isZero( m.m13 ) && isZero( m.m14 ) &&
        isZero( m.m21 ) && equalsClose( m.m22, v ) && isZero( m.m23 ) && isZero( m.m24 ) &&
        isZero( m.m31 ) && isZero( m.m32 ) && equalsClose( m.m33, v ) && isZero( m.m34 ) &&
        isZero( m.m41 ) && isZero( m.m42 ) && isZero( m.m43 ) && equalsClose( m.m44, v );
}

template<typename T>
TMatrix4<T> transpose( const TMatrix4<T>& m )
{
    TMatrix4<T> ret;
    // TODO: Implement me
    return ret;
}

template<typename T>
std::ostream& operator << ( std::ostream& os, const TMatrix4<T>& mat )
{
    os << "[ ";

    for ( unsigned int r = 0; r < TMatrix4<T>::NUM_ROWS; ++r )
    {
        os << "[";

        for ( unsigned int c = 0; c < TMatrix4<T>::NUM_COLS; ++c )
        {
            if ( c == TMatrix4<T>::NUM_COLS - 1 )
            {
                os << mat.at( r, c );
            }
            else
            {
                os << mat.at( r, c ) << ", ";
            }
        }

        os << "]";

        if ( r == TMatrix4<T>::NUM_ROWS - 1 )
        {
            os << " ";
        }
        else
        {
            os << "; ";
        }
    }

    os << " ]";
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

#endif
