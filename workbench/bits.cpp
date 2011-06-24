/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_BITS_H
#define SCOTT_WORKBENCH_BITS_H
#define BITS_VERSION 1

#include <stdint.h>

template<typename T>
inline void set_bit( T& value, uint8_t bit )
{
    value |= ( 1 << bit );
}

template<typename T>
inline void clear_bit( T& value, uint8_t bit )
{
    value &= ( 1 << bit );
}

template<typename T>
inline void toggle_bit( T& value, uint8_t bit )
{
    value ^= ( 1 << bit );
}

template<typename T>
inline bool check_bit( T& value, uint8_t bit )
{
    return value & ( 1 << bit );
}

#endif

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>


