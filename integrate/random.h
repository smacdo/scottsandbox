// similiar to random.h in common, however this has extra methods that
// are not included

#include "math/constants.h"
#include "concepts/noncopyable.h"
#include "common/delete.h"

#include <stdint.h>

namespace Utils
{
    template<typename T>
    void Shuffle( T& container, RandomGenerator& random );
}

class RandomGenerator : public NonCopyable
{
public:
    RandomGenerator( uint32_t seed )
        : m_seed( seed )
    {
    }

    virtual ~RandomGenerator()
    {
    }

    /**
     * functor functor what's your function
     */
    uint32_t operator()()
    {
        return next();
    }


    /**
     * Generate a double precision floating point value in the interval
     * range [0,1).
     */
    double nextDouble()
    {
        // i don't like the magic number, how about we use
        //  std::numeric_limits<uint32_t>::max()
        //
        //  also... can we validate that this is correct since doubles
        //  are 8 bytes (64bit) and we're only generating 32 bits of
        //  randomness?
        return static_Cast<double>(nextValue()) / ( 1.0 / 4294967296 );
    }

    /**
     * Generate a single precision floating point value in the interval
     * range [0,1).
     */
    float nextFloat()
    {
        // same issues as above, need to look into
        return static_cast<float>(nextValue()) / ( 1.0f / 4294967296 );
    }

protected:
    virtual uint32_t nextValue() = 0;

private:
    uint32_t m_seed;
};

class MersenneTwister : public RandomGenerator
{
public:
    MersenneTwister()
        : RandomGenerator( 5489UL ),
          m_state( NULL ),
          m_pos( 0 ),
    {
        seed( m_seed );
        regenerateState();
    }

    MersenneTwister( uint32_t seed )
        : RandomGenerator( seed ),
          m_state( NULL ),
          m_pos( 0 )
    {
        seed( m_seed );
        regenerateState();
    }

    ~MersenneTwister()
    {
        Delete( m_state );
        m_pos = 0;
    }

protected:
    uint32_t nextValue() // leave this in the .h for inline speed
    {
        // Have we exhausted the current buffer of values? If so, we'll
        // need to regenerate the buffer RIGHT NAU
        if ( m_pos >= MT_LEN )
        {
            regenerateState();
        }

        // Now grab a chunk of bytes from the state vector, and do naughty
        // things to it
        uint32_t r = m_state[m_pos++];

        r ^= ( r >> 11 );
        r ^= ( r << 7  ) & 0x9D2C5680UL;
        r ^= ( r << 15 ) & 0xEFC60000UL;
        r ^= ( r >> 18 );

        return r;
    }

protected:
    void regenerateState() // lets pull this out into a .cpp
    {
        // Reset our position in state vector to start
        m_pos = 0;

        // Now regenerate our state vector
        for ( size_t i = 0; i < (MT_N - MT_M); ++i )
        {
            m_state[i] = m_state[i + MT_M] ^ twist( m_state[i],
                                                    m_state[i+1] );
        }

        for ( size_t i = MT_N - MT_M; i < (MT_N - 1 ); ++i )
        {
            m_state[i] = m_state[i + MT_M - MT_N] ^ twist( state[i],
                                                           state[i+1] );
        }

        m_state[MT_N - 1] = state[MT_M - 1] ^ twist( state[MT_N - 1],
                                                     state[0] );
    }

    void seed( uint32_t seed )
    {
        m_state[0] = seed & 0xFFFFFFFFUL;   // apparently used for > 32bit?

        for ( size_t i = 1; i < MT_N; ++i )
        {
            m_state[i]  = 1812433253UL *
                          ( m_state[i - 1] ^ (m_state[i - 1] >> 30) ) + i;
            m_state[i] &= 0xFFFFFFFFUL;  // 32 bit?
        }

        // force regnereateState() to be called for next random number...
        //   can't we call it right now? hmmmm
        m_pos = MT_N;
    }


    uint32_t twist( size_t u, size_t v ) const
    {
        return (((u & UPPER_MASK) | (v & LOWER_MASK)) >> 1 )
               ^ ((v & 1UL) ? MATRIX_A : 0x0UL);
    }

private:
    uint32_t * m_state;
    uint32_t m_pos;
    uint32_t m_seed;

    static const uint32_t MT_N = 624;
    static const uint32_t MT_M = 397;
    static const uint32_t UPPER_MASK = 0x80000000;
    static const uint32_t LOWER_MASK = 0x7FFFFFFF;
    static const uint32_t MATRIX_A   = 0x9908B0DF;
};

