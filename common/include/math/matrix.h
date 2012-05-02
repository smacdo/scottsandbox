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

#include <math/vector.h>
#include <math/defs.h>
#include <math/util.h>

#if MATH_DEBUG_MODE == 1
#   include <limits>
#   define MATRIX_DEBUG_MODE 1
#   define MATRIX_DV std::numeric_limits<T>::signaling_NaN()
#endif

#define M_OFFSET(R,C) ((R) * NUM_COLS + (C))

//
// Forward decalarations
//
template<typename T> class TMatrix4;
namespace boost { namespace serialization { class access; } }

template<typename T> bool isZeroMatrix( const TMatrix4<T>& );
template<typename T> bool isIdentityMatrix( const TMatrix4<T>& );
template<typename T> TMatrix4<T> transpose( const TMatrix4<T>& );
template<typename T> T determinant( const TMatrix4<T>& );
template<typename T> TMatrix4<T> tryInverse( const TMatrix4<T>&, bool* = NULL);
template<typename T> TMatrix4<T> inverse( const TMatrix4<T>& );
template<typename T> TMatrix4<T> calculateInverse( const TMatrix4<T>&, T );
template<typename T> T trace( const TMatrix4<T>& );

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
     * 4x4 matrix constructor. Arguments are to be specified in row
     * major format.
     */
    TMatrix4( value_type m34, value_type m41, value_type m42, value_type m43,
              value_type M21, value_type M22, value_type M23, value_type M24,
              value_type M31, value_type M32, value_type M33, value_type M34,
              value_type M41, value_type M42, value_type M43, value_type M44 )
        : m11(m34), m12(m41), m13(m42), m14(m43),
          m21(M21), m22(M22), m23(M23), m24(M24),
          m31(M31), m32(M32), m33(M33), m34(M34),
          m41(M41), m42(M42), m43(M43), m44(M44)
    {
    }

    /**
     * Construct a matrix from an array of values
     *
     * \param  pVals  Pointer to an array of 16 values. Must be in row-major
     *                order
     */
    explicit TMatrix4( const_pointer pVals )
        : m11( pVals[0]  ), m12( pVals[1]  ), m13( pVals[2]   ), m14( pVals[3]  ),
          m21( pVals[4]  ), m22( pVals[5]  ), m23( pVals[6]   ), m24( pVals[7]  ),
          m31( pVals[8]  ), m32( pVals[9]  ), m33( pVals[10] ),  m34( pVals[11] ),
          m41( pVals[12] ), m42( pVals[13] ), m43( pVals[14] ),  m44( pVals[15] )    
    {
    }

    /**
     * Copy constructor
     *
     * \param  m  Matrix to copy values from
     */
    TMatrix4( const TMatrix4<T>& m )
        : m11( m.m11 ), m12( m.m12 ), m13( m.m13 ), m14( m.m14 ),
          m21( m.m21 ), m22( m.m22 ), m23( m.m23 ), m24( m.m24 ),
          m31( m.m31 ), m32( m.m32 ), m33( m.m33 ), m34( m.m34 ),
          m41( m.m41 ), m42( m.m42 ), m43( m.m43 ), m44( m.m44 )
    {
    }

    /**
     * Constant pointer that points to this instance's underlying 4x4
     * value array
     */
    const_pointer const_ptr()
    {
        return m;
    }

    /**
     * Constant pointer that points to this instance's underlying 4x4
     * value array
     */
    const_pointer ptr() const
    {
        return m;
    }

    /**
     * Pointer that points to this instance's underlying 4x4 value array
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
    const_reference operator[] ( unsigned int offset ) const
    {
        math_assert( NUM_VALUES && "Matrix4 operator[] out of range" );
        return m[offset];
    }

    /**
     * Index operator. Use this to directly read one of the matrix's values.
     * Note that the index is different depending on if the matrix is column
     * major or row major!
     */
    reference operator[] ( unsigned int offset )
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
        return TMatrix4<T>(
            m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 + m14 * rhs.m41,
            m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 + m14 * rhs.m42,
            m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 + m14 * rhs.m43,
            m11 * rhs.m14 + m12 * rhs.m24 + m13 * rhs.m34 + m14 * rhs.m44,
            m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 + m24 * rhs.m41,
            m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 + m24 * rhs.m42,
            m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 + m24 * rhs.m43,
            m21 * rhs.m14 + m22 * rhs.m24 + m23 * rhs.m34 + m24 * rhs.m44,
            m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 + m34 * rhs.m41,
            m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 + m34 * rhs.m42,
            m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 + m34 * rhs.m43,
            m31 * rhs.m14 + m32 * rhs.m24 + m33 * rhs.m34 + m34 * rhs.m44,
            m41 * rhs.m11 + m42 * rhs.m21 + m43 * rhs.m31 + m44 * rhs.m41,
            m41 * rhs.m12 + m42 * rhs.m22 + m43 * rhs.m32 + m44 * rhs.m42,
            m41 * rhs.m13 + m42 * rhs.m23 + m43 * rhs.m33 + m44 * rhs.m43,
            m41 * rhs.m14 + m42 * rhs.m24 + m43 * rhs.m34 + m44 * rhs.m44
        );
    }

    /**
     * Matrix self multiplication operator
     */
    TMatrix4<T>& operator *= ( const TMatrix4<T>& rhs )
    {
        m11 = m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 + m14 * rhs.m41;
        m12 = m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 + m14 * rhs.m42;
        m13 = m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 + m14 * rhs.m43;
        m14 = m11 * rhs.m14 + m12 * rhs.m24 + m13 * rhs.m34 + m14 * rhs.m44;
        m21 = m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 + m24 * rhs.m41;
        m22 = m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 + m24 * rhs.m42;
        m23 = m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 + m24 * rhs.m43;
        m24 = m21 * rhs.m14 + m22 * rhs.m24 + m23 * rhs.m34 + m24 * rhs.m44;
        m31 = m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 + m34 * rhs.m41;
        m32 = m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 + m34 * rhs.m42;
        m33 = m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 + m34 * rhs.m43;
        m34 = m31 * rhs.m14 + m32 * rhs.m24 + m33 * rhs.m34 + m34 * rhs.m44;
        m41 = m41 * rhs.m11 + m42 * rhs.m21 + m43 * rhs.m31 + m44 * rhs.m41;
        m42 = m41 * rhs.m12 + m42 * rhs.m22 + m43 * rhs.m32 + m44 * rhs.m42;
        m43 = m41 * rhs.m13 + m42 * rhs.m23 + m43 * rhs.m33 + m44 * rhs.m43;
        m44 = m41 * rhs.m14 + m42 * rhs.m24 + m43 * rhs.m34 + m44 * rhs.m44;

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
    value_type at( unsigned int r, unsigned int c ) const
    {
        math_assert( r < NUM_ROWS && "Matrix row out of range");
        math_assert( c < NUM_COLS && "Matrix column out of range");

        return m[ M_OFFSET(r,c) ];
    }

    /**
     * Sets the value at the given (r,c) matrix cell.
     */
    void set( unsigned int r, unsigned int c, value_type v )
    {
        math_assert( r < NUM_ROWS && "Matrix row out of range");
        math_assert( c < NUM_COLS && "Matrix column out of range");

        m[ M_OFFSET(r,c) ] = v;
    }

    /**
     * Returns a row of the matrix
     */
    TVector4<T> row( unsigned int r ) const
    {
        math_assert( r < NUM_ROWS && "Matrix row out of range" );

        return TVector4<T>( m[ M_OFFSET(r,0) ],
                            m[ M_OFFSET(r,1) ],
                            m[ M_OFFSET(r,2) ],
                            m[ M_OFFSET(r,3) ] );
    }

    /**
     * Returns a column of the matrix
     */
    TVector4<T> column( unsigned int c ) const
    {
        math_assert( c < NUM_COLS && "Matrix col out of range" );

        return TVector4<T>( m[ M_OFFSET(0,c) ],
                            m[ M_OFFSET(1,c) ],
                            m[ M_OFFSET(2,c) ],
                            m[ M_OFFSET(3,c) ] );
    }

    friend class boost::serialization::access;

    /**
     * Serialization
     */
    template<typename Archive>
    void serialize( Archive& ar, const unsigned int version )
    {
        ar & m[0]  & m[1]  & m[2]  & m[3]
           & m[4]  & m[5]  & m[6]  & m[7]
           & m[8]  & m[9]  & m[10] & m[11]
           & m[12] & m[13] & m[14] & m[15];
    }

    friend bool isZeroMatrix<>( const TMatrix4<T>& );
    friend bool isIdentityMatrix<>( const TMatrix4<T>& );
    friend TMatrix4<T> transpose<>( const TMatrix4<T>& );
    friend value_type trace<>( const TMatrix4<T>& );
    friend value_type determinant<>( const TMatrix4<T>& );
    friend TMatrix4<T> inverse<>( const TMatrix4<T>& );
    friend TMatrix4<T> tryInverse<>( const TMatrix4<T>&, bool* );
    friend TMatrix4<T> calculateInverse<>( const TMatrix4<T>&, value_type );

public:
    static const TMatrix4<T> ZERO_MATRIX;
    static const TMatrix4<T> IDENTITY;

protected:
    /// The matrix's data
    union
    {
        value_type m[NUM_VALUES];
        struct      // row major ordering
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

/**
 * Check if the matrix is entirely zero
 *
 * \param  m  Matrix to check
 * \return    True if the matrix is all zero, false otherwise
 */
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

/**
 * Checks if the given matrix is an identity matrix
 *
 * \param  m  Matrix to check
 * \return    True if the matrix is an identity matrix, false otherwise
 */
template<typename T>
bool isIdentityMatrix( const TMatrix4<T>& m )
{
    using namespace Math;
    
    T v = static_cast<T>( 1 );

    return 
        equalsClose( m.m11, v ) && isZero( m.m21 ) && isZero( m.m31 ) && isZero( m.m41 ) &&
        isZero( m.m21 ) && equalsClose( m.m22, v ) && isZero( m.m32 ) && isZero( m.m42 ) &&
        isZero( m.m31 ) && isZero( m.m23 ) && equalsClose( m.m33, v ) && isZero( m.m43 ) &&
        isZero( m.m41 ) && isZero( m.m24 ) && isZero( m.m34 ) && equalsClose( m.m44, v );
}

/**
 * Returns the transpose of the given matrix
 *
 * \param  m  Matrix to tranpose
 * \return    The transposed matrix
 */
template<typename T>
TMatrix4<T> transpose( const TMatrix4<T>& m )
{
    return TMatrix4<T>( m.m11, m.m21, m.m31, m.m41,
                        m.m12, m.m22, m.m32, m.m42,
                        m.m13, m.m23, m.m33, m.m43,
                        m.m14, m.m24, m.m34, m.m44 );
}

/**
 * Returns the sum of the elements on the matrix's diagonal
 *
 * \param  m  Matrix to calculate trace
 * \return    Trace value of the matrix
 */
template<typename T>
T trace( const TMatrix4<T>& m )
{
    return m.m11 + m.m22 + m.m33 + m.m44;
}

/**
 * Calculates the determinant of the matrix
 *
 * \param  m  Matrix to calculate
 * \return    Value of the determinant
 */
template<typename T>
T determinant( const TMatrix4<T>& m )
{
    return 
        m.m14 * m.m23 * m.m32 * m.m41-m.m13 * m.m24 * m.m32 * m.m41-m.m14 * m.m22 * m.m33 * m.m41+m.m12 * m.m24 * m.m33 * m.m41
      + m.m13 * m.m22 * m.m34 * m.m41-m.m12 * m.m23 * m.m34 * m.m41-m.m14 * m.m23 * m.m31 * m.m42+m.m13 * m.m24 * m.m31 * m.m42
      + m.m14 * m.m21 * m.m33 * m.m42-m.m11 * m.m24 * m.m33 * m.m42-m.m13 * m.m21 * m.m34 * m.m42+m.m11 * m.m23 * m.m34 * m.m42
      + m.m14 * m.m22 * m.m31 * m.m43-m.m12 * m.m24 * m.m31 * m.m43-m.m14 * m.m21 * m.m32 * m.m43+m.m11 * m.m24 * m.m32 * m.m43
      + m.m12 * m.m21 * m.m34 * m.m43-m.m11 * m.m22 * m.m34 * m.m43-m.m13 * m.m22 * m.m31 * m.m44+m.m12 * m.m23 * m.m31 * m.m44
      + m.m13 * m.m21 * m.m32 * m.m44-m.m11 * m.m23 * m.m32 * m.m44-m.m12 * m.m21 * m.m33 * m.m44+m.m11 * m.m22 * m.m33 * m.m44;
}

/**
 * Conditional matrix inversion. This function attempts to calculate the
 * inverse of a matrix, but it will abort if the determinant is equal to zero.
 * If the caller passes a non-null pointer to a bool, it will contain the
 * success/failure of the calculation.
 *
 * \param  m        Matrix to invert
 * \param  pStatus  Optional. If not null, this will be true if the matrix
 *                  inversion succeeded, false otherwsie
 * \return          Result of inversion. If not possible, the identity matrix
 *                  will be returned
 */
template<typename T>
TMatrix4<T> tryInverse( const TMatrix4<T>& m, bool * pStatus )
{
    // Calculate the matrix's determinant. Can it even be inverted?
    T det = determinant( m );

    if ( Math::isZero( det ) )
    {
        if ( pStatus != NULL )
        {
            *pStatus = false;
        }

        return TMatrix4<T>::IDENTITY;
    }
    else if ( pStatus!= NULL )
    {
        *pStatus = true;
    }

    return calculateInverse( m, det );
}

/**
 * Calculates inverse of the given matrix. This function will assert and die
 * if the determinant is equal to zero.
 *
 * \param  m  Matrix to invert
 * \return    Inverted matrix
 */
template<typename T>
TMatrix4<T> inverse( const TMatrix4<T>& m )
{
    // Calculate the matrix determinant. Can it even be inverted?
    T det = determinant( m );
    assert( Math::isNotZero( det ) && "Cannot invert a singular matrix" );

    return calculateInverse( m, det );
}

/**
 * Internal function that calculates the "second half" of a matrix inversion.
 * It takes the matrix's determinant (to avoid recalculating it), and assumes
 * that it is never zero. It is the responsibility of the caller to ensure
 * that the determinant is not zero.
 *
 * \param  m    Matrix to invert
 * \param  det  Determinant value of the matrix
 * \return      An inverted matrix
 */
template<typename T>
TMatrix4<T> calculateInverse( const TMatrix4<T>& m, T det )
{
    return TMatrix4<T>(
        (m.m22*(m.m33*m.m44 - m.m43*m.m34) - m.m23*(m.m32*m.m44 - m.m42*m.m34) - m.m24*(m.m42*m.m33 - m.m32*m.m43)) / det,
        (m.m12*(m.m43*m.m34 - m.m33*m.m44) - m.m13*(m.m42*m.m34 - m.m32*m.m44) - m.m14*(m.m32*m.m43 - m.m42*m.m33)) / det,
        (m.m12*(m.m23*m.m44 - m.m43*m.m24) - m.m13*(m.m22*m.m44 - m.m42*m.m24) - m.m14*(m.m42*m.m23 - m.m22*m.m43)) / det,
        (m.m12*(m.m33*m.m24 - m.m23*m.m34) - m.m13*(m.m32*m.m24 - m.m22*m.m34) - m.m14*(m.m22*m.m33 - m.m32*m.m23)) / det,

        (m.m21*(m.m43*m.m34 - m.m33*m.m44) - m.m23*(m.m41*m.m34 - m.m31*m.m44) - m.m24*(m.m31*m.m43 - m.m41*m.m33)) / det,
        (m.m11*(m.m33*m.m44 - m.m43*m.m34) - m.m13*(m.m31*m.m44 - m.m41*m.m34) - m.m14*(m.m41*m.m33 - m.m31*m.m43)) / det,
        (m.m11*(m.m43*m.m24 - m.m23*m.m44) - m.m13*(m.m41*m.m24 - m.m21*m.m44) - m.m14*(m.m21*m.m43 - m.m41*m.m23)) / det,
        (m.m11*(m.m23*m.m34 - m.m33*m.m24) - m.m13*(m.m21*m.m34 - m.m31*m.m24) - m.m14*(m.m31*m.m23 - m.m21*m.m33)) / det,

        (m.m21*(m.m32*m.m44 - m.m42*m.m34) - m.m22*(m.m31*m.m44 - m.m41*m.m34) - m.m24*(m.m41*m.m32 - m.m31*m.m42)) / det,
        (m.m11*(m.m42*m.m34 - m.m32*m.m44) - m.m12*(m.m41*m.m34 - m.m31*m.m44) - m.m14*(m.m31*m.m42 - m.m41*m.m32)) / det,
        (m.m11*(m.m22*m.m44 - m.m42*m.m24) - m.m12*(m.m21*m.m44 - m.m41*m.m24) - m.m14*(m.m41*m.m22 - m.m21*m.m42)) / det,
        (m.m11*(m.m32*m.m24 - m.m22*m.m34) - m.m12*(m.m31*m.m24 - m.m21*m.m34) - m.m14*(m.m21*m.m32 - m.m31*m.m22)) / det,

        (m.m21*(m.m42*m.m33 - m.m32*m.m43) - m.m22*(m.m41*m.m33 - m.m31*m.m43) - m.m23*(m.m31*m.m42 - m.m41*m.m32)) / det,
        (m.m11*(m.m32*m.m43 - m.m42*m.m33) - m.m12*(m.m31*m.m43 - m.m41*m.m33) - m.m13*(m.m41*m.m32 - m.m31*m.m42)) / det,
        (m.m11*(m.m42*m.m23 - m.m22*m.m43) - m.m12*(m.m41*m.m23 - m.m21*m.m43) - m.m13*(m.m21*m.m42 - m.m41*m.m22)) / det,
        (m.m11*(m.m22*m.m33 - m.m32*m.m23) - m.m12*(m.m21*m.m33 - m.m31*m.m23) - m.m13*(m.m31*m.m22 - m.m21*m.m32)) / det
   );
}

/**
 * Output stream operator. Prints a formatted version of the matrix to
 * a text stream
 */
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
               << m.at(2,3) << ", " << m.at(3,3) << " ] " << "]";

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
