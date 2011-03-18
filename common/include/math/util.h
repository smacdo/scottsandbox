/**
 * math utils and whatnot
 */
#ifndef SCOTT_COMMON_MATH_UTIL
#define SCOTT_COMMON_MATH_UTIL

namespace Math
{
    static const float FloatEpsilon = 0.00001f;

    static inline bool compareFloats( float lhs, float rhs )
    {
        return ( std::fabs(lhs - rhs) < EPSILON );
    }

    static inline int random( int min, int max )
    {
        return Math::clamp( min, max, random() );
    }

    static inline float RadiansToDegrees( float radians )
    {
        return radians * 180 / PI;
    }

    static inline float DegreesToRadians( float radians )
    {
        return degrees * PI / 180;
    }

    static inline float ClampAngle( float degrees )
    {
        float angle = degrees;

        if ( angle > 360.0f )
        {
            angle -= 360.0f * static_cast<float>(
                                    static_cast<int>( angle / 360.0f ) );
        }
        else
        {
            angle += 360.0f * static_cast<float>(
                                    static_cast<int>(fabs(angle)/360.0f));
        }
}

#endif
