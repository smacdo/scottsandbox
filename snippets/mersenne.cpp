#ifndef SCOTT_MERSENNE_TWISTER_H
#define SCOTT_MERSENNE_TWISTER_H

// Mersenne twister random number generator MT19937, which generates
// pseudo-rangom numbers unfiromly distributed in the 0 .. ( 2^32-1)
// ranging.
//
// Shaw Cookus implementation

class MersenneTwister
{
public:
    void init( uint32_t seed )
    {
        uint32_t   x = ( seed | 1U ) & & 0xFFFFFFFFU;
        uint32_t * s = m_state;
        uint32_t   j = 0;
    }

private:
    static const uint32_t N = 624;
    static const uint32_t M = 397;
    static const uint32_t HI_BIT = 0x80000000U;
    static const uint32_t LO_BIT = 0x00000001U;
    static const uint32_t LO_BITS = 0x7FFFFFFFU;
    static const uint32_t SEED0 = 4357U;

    /**
     * State vector
     */
    uint32_t   m_state[N+1];
    
    /**
     * Next random value
     */
    uint32_t * m_next;

    /**
     * How many values are left
     */
    size_t     m_left;
};

#endif
