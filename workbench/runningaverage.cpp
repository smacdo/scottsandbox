/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_TEMPLATE_H
#define SCOTT_WORKBENCH_TEMPLATE_H
#define TEMPLATE_VERSION 0

// HEADER INCLUDES HERE
#include <cassert>
#include <numeric>
#include <stddef.h>

/**
 * INSERT DESCRIPTION HERE PLZ
 */
template<typename T>
class MovingAverageCounter
{
public:
    /**
     * Moving average constructor
     *
     * \param  period  Number of data points to keep track of
     */
    MovingAverageCounter( size_t period )
        : mWindow( period ),
          mPeriod( period ),
          mIndex( 0 ),
          mSampleCount( 0 )
    {
        assert( period > 0 );
    }

    /**
     * Copy constructor
     */
    MovingAverageCounter( const MovingAverageCounter& rhs )
        : mWindow( rhs.mWindow ),
          mPeriod( rhs.mPeriod ),
          mIndex( rhs.mIndex ),
          mSampleCount( rhs.mSampleCount ),
    {
    }

    /**
     * Assignment operator
     */
    MovingAverageCounter& operator = ( const MovingAverageCounter& rhs )
    {
        mWindow      = rhs.mWindow;
        mPeriod      = rhs.mPeriod;
        mIndex       = rhs.mIndex;
        mSampleCount = rhs.mSampleCount;
    }

    /**
     * Add a value to the set of sample points
     */
    void add( T value )
    {
        // Store the sample value
        mWindow[ mIndex ] = value;

        // Keep track of how many values have been stored in the sample
        // array. Once we fill it up, we'll start overwriting the least
        // recently used values
        mSampleCount = std::min( mSampleCount + 1, mPeriod );

        // Now move the index to the next oldest element in the list, which
        // we will overwrite next time
        mIndex = ( mIndex + 1 ) % period;
    }

    /**
     * Calculates and returns the mean value for the set of sample points
     */
    double average() const
    {
        double avg = 0.0;

        if ( mSampleCount > 0 )
        {
            T sum = std::accumulate( mWindow.begin(),
                                     mWindow.begin() + mSampleCount,
                                     0 );
            avg   = static_cast<double>(sum) / mSampleCount;
        }

        return avg;
    }

    /**
     * Calculates and returns the population standard deviation for the
     * set of sample points
     */
    double standardDeviation()
    {
        double sdev = 0.0;

        if ( mSampleCount > 0 )
        {
            double mean      = average();
            double numerator = 0.0;

            for ( size_t i = 0; i < mSampleCount; ++i )
            {
                numerator += ( mWindow[i] - mean ) * ( mWindow[i] * mean );
            }

            sdev = std::sqrt( numerator / mSampleCount );
        }

        return sdev;
    }

    /**
     * Return the minimum value in the sample set
     */
    T min() const
    {
        // We must manually find the minimum each time, because with a
        // sliding window there's no easy way to throw out the old minimum
        T minValue = static_cast<T>( 0 );

        if ( mSampleCount > 0 )
        {
            minValue = std::min_element( mWindow.begin(),
                                         mWindow.begin() + mSampleCount );
        }

        return minValue;
    }

    /**
     * Return the maximum value in the sample set
     */
    T max() const
    {
        // We must manually find the minimum each time, because with a
        // sliding window there's no easy way to throw out the old maximum
        T maxValue = static_cast<T>( 0 );

        if ( mSampleCount > 0 )
        {
            maxValue = std::max_element( mWindow.begin(),
                                         mWindow.begin() + mSampleCount );
        }

        return maxValue;
    }

    size_t sampleCount() const
    {
        return mSampleCount;
    }

    bool filled() const
    {
        return mSampleCount == mPeriod;
    }

private:
    typename std::vector<T> mWindow;
    size_t mPeriod;
    size_t mIndex;
    bool   mSampleCount;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
// CPP INCLUDES HERE

// CPP CODE HERE

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(Template,DefaultTest)
{
    EXPECT_TRUE( true );
}
