#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

#include <limits>

class ScopedTrace
{

};

template<typename T>
class TimedStatCounter
{

};

class StatCounter
{
public:
    /**
     * Stat counter constructor - create a new stat counter
     */
    StatCounter()
        : m_counter(0),
          m_delta(0.0f),
          m_mean(0.0f),
          m_m2(0.0f),
          m_lowestValue( std::numeric_limits<double>::max() ),
          m_highestValue( std::numeric_limits<double>::min() )
    {
    }

    void add( double value )
    {
        //
        // update min/max limits
        //
        if ( value < m_lowestValue )
        {
            m_lowestValue = value;
        }

        if ( value > m_highestValue )
        {
            m_highestValue = value;
        }

        // 
        // Update average / standard deviation calculations
        //
        m_counter += 1;

        m_delta  = value - m_mean;
        m_m2     = m_m2 + m_delta * ( value - m_mean );
    }

    /**
     * Smallest value added to the counter
     */
    double lowestValue() const
    {
        return m_lowestValue;
    }

    /**
     * Returns the largest value added to the counter
     */
    double highestValue() const
    {
        return m_highestValue;
    }

    /**
     * The number of values that have been added to this counter
     */
    size_t count() const
    {
        return m_counter;
    }

    /**
     * The mean of the set of values that have been added to this counter
     */
    double average() const
    {
        return m_mean;
    }

    /**
     * The variance calculated from the values added to this counter
     */
    double variance() const
    {
        assert( m_counter > 1 );
        return m_m2 / ( m_counter - 1 );
    }

    /**
     * The biased variance calculated from the values added to this counter
     */
    double variance_n() const
    {
        assert( m_counter > 0 );
        return m_m2 / m_counter;
    }

    /**
     * The standard deviation calculated from the values added to this
     * counter
     */
    double standardDeviation() const
    {
        assert( m_counter > 1 );
        std::sqrt( m_m2 / ( m_counter - 1 ) );
    }

    double standardDeviation_n() const
    {
        assert( m_counter > 0 );
        return sqrt( m_m2 / m_counter );
    }

    /**
     * Resets the counter, thereby resetting all collected statistics
     * about the values added to the counter
     */
    void reset()
    {
        m_counter      = 0.0;
        m_delta        = 0.0;
        m_mean         = 0.0;
        m_m2           = 0.0;
        m_lowestValue  = std::numeric_limits<double>::max();
        m_highestValue = std::numeric_limits<double>::min();
    }

private:
    size_t m_counter;
    double m_delta;
    double m_mean;
    double m_m2;
    double m_lowestValue;
    double m_highestValue;
};


/*int main( int argc, char* argv[] )
{
    StatCounter counter;
    counter.add( 42 );
    counter.add( 2 );
    counter.add( 10 );
    counter.add( 12 );

    std::cout << "Stat counter min=" << counter.lowestValue()  << std::endl
              << "             max=" << counter.highestValue() << std::endl
              << "             avg=" << counter.average()      << std::endl
              << "             dev=" << counter.standardDeviation()
              << std::endl;
}*/
