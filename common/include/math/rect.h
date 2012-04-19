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
public:
    RectF( const Vec2& topLeft, const Vec2& rectSize )
        : mTopLeft( topLeft ),
          mSize( rectSize )
    {
        math_assert( mSize.X() > 0.0f && mSize.Y() > 0.0f );
    }

    RectF( float topX, float topY, float width, float height )
        : mTopLeft( topX, topY ),
          mSize( width, height )
    {
        math_assert( width > 0.0f && height > 0.0f );
    }

    RectF( const RectF& rect )
        : mTopLeft( rect.mTopLeft ),
          mSize( rect.mSize )
    {
    }

    RectF& operator = ( const RectF& rect )
    {
        mTopLeft = rect.mTopLeft;
        mSize    = rect.mSize;

        return *this;
    }

    bool operator == ( const RectF& rect ) const
    {
        return ( mTopLeft == rect.mTopLeft && mSize == rect.mSize );
    }

    bool operator != ( const RectF& rect ) const
    {
        return !( mTopLeft == rect.mTopLeft && mSize == rect.mSize ); 
    }

    Vec2 position() const
    {
        return mTopLeft;
    }

    Vec2 topLeft() const
    {
        return mTopLeft;
    }

    Vec2 topRight() const
    {
        return Vec2( mTopLeft.x() + mSize.x(), mTopLeft.y() );
    }

    Vec2 bottomLeft() const
    {
        return Vec2( mTopLeft.x(), mTopLeft.y() + mSize.y() );
    }
    
    Vec2 bottomRight() const
    {
        return mTopLeft + mSize;
    }

    Vec2 center() const
    {
        return ( mTopLeft + mSize ) / 2.0f;
    }

    Vec2 size() const
    {
        return mSize;
    }

    float top() const
    {
        return mTopLeft.y();
    }

    float left() const
    {
        return mTopLeft.x();
    }

    float right() const
    {
        return mTopLeft.x() + mSize.x();
    }

    float bottom() const
    {
        return mTopLeft.y() + mSize.y();
    }

    float width() const
    {
        return mSize.x();
    }

    float height() const
    {
        return mSize.y();
    }

    void move( const Vec2& delta )
    {
        mTopLeft += delta;
    }

    void moveTo( const Vec2& position )
    {
        mTopLeft = position;
    }

    void adjustSize( const Vec2& newSize )
    {
        mSize += newSize;
        math_assert( mSize.x() > 0.0f && mSize.y() > 0.0f );
    }

    void resize( const Vec2& newSize )
    {
        math_assert( newSize.x() > 0.0f && newSize.y() > 0.0f );
        mSize = newSize;
    }

    Vec2 resize( float width, float height )
    {
        resize( Vec2( width, height ) );
    }

    bool contains( const Vec2& point ) const
    {
        Vec2 pt = point - mTopLeft;

        return ( pt.x() >= 0.0f && pt.x() <= mSize.x() &&
                 pt.y() >= 0.0f && pt.y() <= mSize.y() );
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
    Vec2 mTopLeft;
    Vec2 mSize;
};

#endif

