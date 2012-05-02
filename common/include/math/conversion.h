#ifndef SCOTT_MATHLIB_CONVERSION_H
#define SCOTT_MATHLIB_CONVERSION_H

#include "math/constants.h"

namespace Math
{
    /**
     * Convert degrees to radians
     */
    template<typename T>
    inline T deg2rad( T deg )
    {
        return static_cast<T>(Pi) / static_cast<T>(180.0) * deg;
    }

    /**
     * Convert radians to degrees
     */
    template<typename T>
    inline T rad2deg( T rad )
    {
        return static_cast<T>(180.0) / static_cast<T>(Pi) * rad;
    }
}

#endif
