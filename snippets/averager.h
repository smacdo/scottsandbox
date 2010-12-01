#ifndef SCOTT_SANDBOX_MATH_AVERAGER_H
#define SCOTT_SANDBOX_MATH_AVERAGER_H

#include <limits>
#include <cassert>

template<typename T>
class Averager
{
public:
    Averager()
        : m_currentAverage( T() ),
          m_count( 0 )
    {
    }

    void add( const T& value )
    {
        assert( count < numeric_limits<T>::max() );
        m_currentAverage += ( value - m_currentAverage ) / ++m_count;
    }

    T currentAverage() const { return m_currentAverage; }
    size_t count() const { return m_count; }

private:
    T m_currentAverage;
    size_t m_count;
};

#endif
