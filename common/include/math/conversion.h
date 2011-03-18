#ifndef SCOTT_MATHLIB_CONVERSION_H
#define SCOTT_MATHLIB_CONVERSION_H

#include "math/mathdefs.h"
#include "math/constants.h"

namespace Math
{

    inline Scalar deg2rad( Scalar deg )
    {
        return (Pi / 180.0f) * deg;
    }

    inline Scalar rad2deg( Scalar rad )
    {
        return (180.0f / Pi) * deg;
    }
}

#endif
