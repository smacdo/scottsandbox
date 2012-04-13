/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_RUNNINGAVERAGE_H
#define SCOTT_WORKBENCH_RUNNINGAVERAGE_H
#define TEMPLATE_VERSION 0

#include <cassert>
#include <cmath>
#include <algorithm>
#include <vector>

/**
 * The moving average counter is a statistical counter that keeps details
 * on a "moving set" of sample values. The counter will keep track of up to
 * a maximum number of samples (period), and will begin to overwrite the
 * least recently added values once this sample set has been filled.
 */
template<typename T>
class MovingAverageCounter
{
public:
    /**
     * Moving average constructor.
     *
     * \param  period  Number of sample values to track
     */
    MovingAverageCounter( size_t period )
        : mWindow( period ),
          mPeriod( period ),
          mIndex( 0u ),
          mSampleCount( 0u ),
          mTotalCount( 0u )
    {
        assert( period > 0u );
    }

    /**
     * Copy constructor
     */
    MovingAverageCounter( const MovingAverageCounter& rhs )
        : mWindow( rhs.mWindow ),
          mPeriod( rhs.mPeriod ),
          mIndex( rhs.mIndex ),
          mSampleCount( rhs.mSampleCount ),
          mTotalCount( rhs.mTotalCount )
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
        mTotalCount  = rhs.mTotalCount;
    }

    /**
     * Add a value to this counter's set of sample points. Once the counter's
     * internal sample array has reached capacity, this method will begin
     * overwriting the least recently added entries.
     *
     * \param  value  A new value that should be tracked
     */
    void add( T value )
    {
        // Store the sample value
        mWindow[ mIndex ] = value;

        // Keep track of how many values have been stored in the sample
        // array. Once we fill it up, we'll start overwriting the least
        // recently used values
        mSampleCount  = std::min( mSampleCount + 1, mPeriod );
        mTotalCount  += 1;

        // Now move the index to the next oldest element in the list, which
        // we will overwrite next time
        mIndex = ( mIndex + 1 ) % mPeriod;
    }

    /**
     * Calculates and returns the moving mean value for the current set of
     * sample values.
     *
     * \return  The current moving mean
     */
    double average() const
    {
        double avg = 0.0;

        if ( mSampleCount > 0 )
        {
            T sum = std::accumulate( mWindow.begin(),
                                     mWindow.begin() + mSampleCount,
                                     0 );
            avg   = static_cast<double>(sum) / 
                    static_cast<double>(mSampleCount);
        }

        return avg;
    }

    /**
     * Calculates and returns the population standard deviation for the
     * set of sample points
     *
     * \return  The population standard deviation
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
     * Return the moving minimum value in the sample set. This minimum value
     * is whichever the smallest *stored* value in the sample array, not the
     * counter's lifetime minimum value.
     *
     * \return  Moving minimum sample value
     */
    T min() const
    {
        // We must manually find the minimum each time, because with a
        // sliding window there's no easy way to throw out the old minimum
        T minValue = static_cast<T>( 0 );

        if ( mSampleCount > 0 )
        {
            auto itr = std::min_element( mWindow.begin(),
                                         mWindow.begin() + mSampleCount );
            minValue = *itr;
        }

        return minValue;
    }

    /**
     * Return the moving maximum value in the sample set. This maximum value
     * is whichever the largest *stored* value in the sample array, not the
     * counter's lifetime maximum value
     *
     * \param  Moving maximum sample value
     */
    T max() const
    {
        // We must manually find the maximum each time, because with a
        // sliding window there's no easy way to throw out the old maximum
        T maxValue = static_cast<T>( 0 );

        if ( mSampleCount > 0 )
        {
            auto itr = std::max_element( mWindow.begin(),
                                         mWindow.begin() + mSampleCount );
            maxValue = *itr;
        }

        return maxValue;
    }

    /**
     * Returns the number of samples that are being tracked for statistical
     * purposes. This will never be larger than the counter's period.
     * 
     * \return  Number of samples stored in the counter's sample array
     */
    size_t sampleCount() const
    {
        return mSampleCount;
    }

    /**
     * Returns the total number of samples that have been added over the
     * lifetime of this counter. This includes samples that have already been
     * discarded from the moving sample array
     *
     * \param  Total number of values added over lifetime of the counter
     */
    size_t totalCount() const
    {
        return mTotalCount;
    }

    /**
     * Checks if the counter's internal sample array has been filled.
     *
     * \return  True if the sample array is at capacity, false otherwise
     */
    bool filled() const
    {
        return mSampleCount == mPeriod;
    }

private:
    typename std::vector<T> mWindow;
    size_t mPeriod;
    size_t mIndex;
    size_t mSampleCount;
    size_t mTotalCount;
};

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(MovingAverageCounter,Constructor)
{
    MovingAverageCounter<int> m( 5 );
    EXPECT_TRUE( true );
}

TEST(MovingAverageCounter,NoElementsDefaultValues)
{
    MovingAverageCounter<int> m( 5 );

    EXPECT_EQ( 0.0, m.average() );
    EXPECT_EQ( 0.0, m.standardDeviation() );
    EXPECT_EQ( 0, m.min() );
    EXPECT_EQ( 0, m.max() );
    EXPECT_EQ( 0u, m.sampleCount() );
    EXPECT_EQ( 0u, m.totalCount() );
    EXPECT_FALSE( m.filled() );
}

#endif
