#ifndef SCOTT_COMMON_COLOR_H
#define SCOTT_COMMON_COLOR_H

#include <stdint.h>
#include <cmath>
#include <ostream>

/**
 * Represents a simple 32bit RGBA color value
 */
class Color
{
public:
    Color()
        : m_color( 0 )
    {
    }

    Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
        : m_color( a | ( b << 8 ) || ( g << 16 ) || ( r << 24 ) )
    {
    }

    Color( uint8_t r, uint8_t g, uint8_t b )
        : m_color( 0xFF | ( b << 8 ) | ( g << 16 ) | ( r << 24 ) )
    {
    }

    explicit Color( uint32_t pixel )
        : m_color( pixel )
    {
    }

    Color getGammaCorrected( float val ) const
    {
        return Color( std::pow( r(), val ),
                      std::pow( g(), val ),
                      std::pow( b(), val ),
                      a()
        );
    }

    Color getInterpolatedWith( const Color& rhs, float amount ) const
    {
        return Color( r() + amount * (rhs.r() - r()),
                      g() + amount * (rhs.g() - g()),
                      b() + amount * (rhs.b() - b()),
                      a() + amount * (rhs.a() - a())
        );
    }

    uint8_t r() const
    {
        return (m_color & 0xFF000000) >> 24;
    }

    uint8_t g() const
    {
        return (m_color & 0x00FF0000) >> 16;
    }

    uint8_t b() const
    {
        return (m_color & 0x0000FF00) >> 8;
    }

    uint8_t a() const
    {
        return m_color & 0x000000FF;
    }

    /**
     * Return int32 representation of the pixel
     */
    uint32_t rgba32() const
    {
        return m_color;
    }

    /**
     * Equality operator
     */
    bool operator == ( const Color& rhs ) const
    {
        return m_color == rhs.m_color;
    }

    /**
     * Inequality operator
     */
    bool operator != ( const Color& rhs ) const
    {
        return m_color != rhs.m_color;
    }

    friend std::ostream& operator << ( std::ostream& os, const Color& c );

private:
    uint32_t m_color;
};

std::ostream& operator << ( std::ostream& os, const Color& c )
{
    os << "{color;"
       << c.r() << ","
       << c.g() << ","
       << c.b() << ","
       << c.a()
       << "}";
}

#endif
