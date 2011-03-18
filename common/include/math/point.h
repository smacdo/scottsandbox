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
#ifndef SCOTT_COMMON_MATH_POINT_H
#define SCOTT_COMMON_MATH_POINT_H

#include "common/math/mathdefs.h"

class Point
{
public:
    Point()
        : m_x(0), m_y(0), m_z(0)
    {
    }

    Point( int px, int py )
        : m_x(px), m_y(py)
    {
    }

    Point( int px, int py, int pz )
        : m_x(px), m_y(py), m_z(pz)
    {
    }

    Point( const Point& point )
        : m_x(point.m_x), m_y(point.m_y), m_z(point.m_z)
    {
    }

    const Point& operator = ( const Point& rhs )
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        return *this;
    }

    bool operator == ( const Point& rhs ) const
    {
        return ( m_x == rhs.m_x &&
                 m_y == rhs.m_y &&
                 m_z == rhs.m_z );
    }

    bool operator != ( const Point& rhs ) const
    {
        return ( m_x != rhs.m_x ||
                 m_y != rhs.m_y ||
                 m_z != rhs.m_z );
    }

    Point operator + ( const Point& rhs ) const
    {
        return Point( m_x + rhs.m_x,
                      m_y + rhs.m_y,
                      m_z + rhs.m_z );
    }

    Point& operator += ( const Point& rhs )
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        return *this;
    }

    Point operator - ( const Point& rhs ) const
    {
        return Point( m_x - rhs.m_x,
                      m_y - rhs.m_y,
                      m_z - rhs.m_z );
    }

    Point& operator -= ( const Point& rhs )
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        return *this;
    }

    Point operator * ( Scalar rhs ) const
    {
        return Point( m_x * rhs,
                      m_y * rhs,
                      m_z * rhs );
    }

    Point operator *= ( Scalar rhs )
    {
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
        return *this;
    }

    Point operator / ( Scalar rhs ) const
    {
        return Point( m_x / rhs,
                      m_y / rhs,
                      m_z / rhs );
    }

    Point& operator /= ( Scalar rhs )
    {
        m_x /= rhs;
        m_y /= rhs;
        m_z /= rhs;
        return *this;
    }

    int& operator[] ( size_t index )
    {
        math_assert( index < 3 && "Point operator[] index must be in range");
        return v[index];
    }

    int  operator[] ( size_t index ) const
    {
        math_assert( index < 3 && "Point operator[] index must be in range");
        return v[index];
    }

    bool isZero() const
    {
        return ( m_x == 0 && m_y == 0 && m_z == 0 );
    }

    int x() const { return m_x; }
    int y() const { return m_y; }
    int z() const { return m_z; }

protected:
    union 
    {
        struct { int m_x; int m_y; int m_z; };
        int v[3];
    };
};

#endif
