/**
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
#ifndef SCOTT_MATH_RECT_H
#define SCOTT_MATH_RECT_H

#include "math/vector.h"

/**
 * 2d axis aligned rectangle.
 */
class RectF
{
    RectF( const Vec2& m_topLeft, const Vec2& size )
        : m_topLeft( topLeft ),
          m_size( size )
    {
        math_assert( m_size.X() > 0.0f && m_size.Y() > 0.0f );
    }

    RectF( float topX, float topY, float width, float height )
        : m_topLeft( topX, topY ),
          m_size( width, height )
    {
        math_assert( width > 0.0f && height > 0.0f );
    }

    RectF( const RectF& rect )
        : m_topLeft( rect.m_topLeft ),
          m_size( rect.m_size )
    {
    }

    RectF& operator = ( const RectF& rect )
    {
        m_topLeft = rect.m_topLeft;
        m_size    = rect.m_size;

        return *this;
    }

    bool operator == ( const RectF& rect ) const
    {
        return ( m_topLeft == rect.m_topLeft && m_size == rect.m_size );
    }

    bool operator != ( const RectF& rect ) const
    {
        return !( m_topLeft == rect.m_topLeft && m_size == rect.m_size ); 
    }

    Vec2 position() const
    {
        return m_topLeft;
    }

    Vec2 topLeft() const
    {
        return m_topLeft;
    }

    Vec2 topRight() const
    {
        return Vec2( m_topLeft.X() + m_size.X(), m_topLeft.Y() );
    }

    Vec2 bottomLeft() const
    {
        return Vec2( m_topLeft.X(), m_topLeft.Y() + m_size.Y() );
    }
    
    Vec2 bottomRight() const
    {
        return m_topLeft + m_size;
    }

    Vec2 center() const
    {
        return ( m_topLeft + m_size ) / 2.0f;
    }

    Vec2 size() const
    {
        return m_size;
    }

    float top() const
    {
        return m_topLeft.Y();
    }

    float left() const
    {
        return m_topLeft.X();
    }

    float right() const
    {
        return m_topLeft.X() + size.X();
    }

    float bottom() const
    {
        return m_topLeft.Y() + size.Y();
    }

    float width() const
    {
        return m_size.X();
    }

    float height() const
    {
        return m_size.Y();
    }

    void move( const Vec2& delta )
    {
        m_topLeft += delta;
    }

    void moveTo( const Vec2& position )
    {
        m_topLeft = position;
    }

    void adjustSize( const Vec2& newSize )
    {
        m_size += newSize;
        math_assert( size.X() > 0.0f && size.Y() > 0.0f );
    }

    void resize( const Vec2& newSize )
    {
        math_assert( newSize.X() > 0.0f && newSize.Y() > 0.0f );
        m_size = newSize;
    }

    Vec2 resize( float width, float height )
    {
        resize( Vec2( width, height ) );
    }

    bool contains( const Vec2& point ) const
    {
        Vec2 pt = point - m_topLeft;

        return ( pt.X() >= 0.0f && pt.X() <= m_size.X() &&
                 pt.Y() >= 0.0f && pt.Y() <= m_size.Y() );
    }

    bool contains( const RectF& rect ) const
    {
        return false;
    }

    bool intersects( const RectF& rect ) const
    {
        return false;
    }

private:
    Vec2 m_topLeft;
    Vec2 m_size;
};

#endif

