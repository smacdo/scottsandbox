#ifndef SCOTT_COMMON_MATH_RATIONAL_H
#define SCOTT_COMMON_MATH_RATIONAL_H

class Rational
{
public:
    Rational( long value )
        : m_numerator( value )
    {
    }

    Rational( const Rational& r )
        : m_numerator( r.m_numerator ),
          m_denominator( r.m_denominator )
    {
    }

    float asFloat() const
    {
        math_assert( m_denominator != 0 );

        return static_cast<float>(m_numerator) /
               static_cast<float>(m_denominator);
    }

    double asDouble() const
    {
        math_assert( m_denominator != 0 );

        return static_cast<double>(m_numerator) /
               static_cast<double>(m_denominator);
    }


    Rational& operator = ( const Rational& r )
    {
        m_numerator   = r.m_numerator;
        m_denominator = r.m_denominator;
        return *this;
    }

    Rational operator + ( const Rational& rhs ) const
    {
    }

    Rational& operator += ( const Rational& rhs );
    Rational& operator -= ( const Rational& rhs );
    Rational& operator *= ( const Rational& rhs );
    Rational& operator /= ( const Rational& rhs );

    Rational& operator += ( long rhs );

    bool operator == ( const Rational& rhs ) const;
    bool operator != ( const Rational& rhs ) const;

    bool operator < ( const Rational& rhs ) const;
    bool operator <= ( const Rational& rhs ) const;
    bool operator >  ( const Rational& rhs ) const;
    bool operator >= ( const Rational& rhs ) const;

    friend Rational operator - ( const Rational& rhs ) const;
    friend Rational operator + ( const Rational& rhs ) const;

    bool operator == ( long rhs ) const;
    bool operator != ( long rhs ) const;

    bool operator <  ( long rhs ) const;
    bool operator <= ( long rhs ) const;

    long numerator() const { return m_numerator; }
    long denominator() const { return m_denominator; }

    friend std::ostream& operator << ( std::ostream& os, const Rational& r );

public:
    static Rational abs( const Rational& r );

private:
    long gcd( long n, long m )
    {
        math_assert( n != 0 );
        math_assert( n != m );

        return ( n * m ) / ( lcd( n, m ) );
    }

    long lcd( long n, long m )
    {
    }

private:
    long m_numerator;
    long m_denominator;
};

#endif
