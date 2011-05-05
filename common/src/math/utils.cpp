
#include <math/util.h>
#include <math/defs.h>

float Math::fastSqrt( float v )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = v * 0.5f;
    y  = v;
    i  = *( long *) &y;             // evil floating point bit level hack
    i  = 0x5f3759df - ( i >> 1 );   // seriously, wtf
    y  = *( float *) &i;            // more magic
    y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
//  y  = y * (threehalfs - (x2 * y * y));   // 2nd itr, can be removed

    return y;
}
