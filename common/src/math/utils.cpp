#include "math/utils.h"
#include "math/mathdefs.h"

int random( int min, int max )
{
    Scalar r = ((static_cast<Scalar>(rand()) / static_cast<Scalar>(RAND_MAX)) *
                (high - low) + low);
    return lround(r);
}

Scalar random( Scalar min, Scalar max )
{
    return ((static_cast<Scalar>(rand()) / static_cast<Scalar>(RAND_MAX)) *
            (high - low) + low);
}

bool isPowerOfTwo( long value )
{
    return ( value & ( value-1) ) == 0;
}



