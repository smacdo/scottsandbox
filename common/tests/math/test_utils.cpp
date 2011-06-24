/**
 * Unit tests for common/math/utils
 */
#include <googletest/googletest.h>
#include <math/util.h>

using namespace Math;

//=========================================================================
// CloseEquals (fuzzy equality checking for floating point values)
//=========================================================================
TEST(Math,Utils_EqualsClose_Basic)
{
    // floating point
    EXPECT_TRUE( equalsClose( 42.0f, 42.0f ) );

    // double
    EXPECT_TRUE( equalsClose( 42.0, 42.0 ) );

    // int
    EXPECT_TRUE( equalsClose( 42, 42 ) );
}

TEST(Math,Utils_EqualsClose_NotEquals)
{
    // floating point
    EXPECT_FALSE( equalsClose( 42.0f, -42.0f ) );

    // double
    EXPECT_FALSE( equalsClose( 42.0, -42.0 ) );

    // int
    EXPECT_FALSE( equalsClose( 42, -42 ) );
}

TEST(Math,Utils_EqualsClose_Edges)
{
    EXPECT_TRUE( equalsClose( 1.0, 0.99999999 ) );
    EXPECT_TRUE( equalsClose( 1.0, 1.00000001 ) );
}

TEST(Math,Utils_EqualsClose_ZeroEdges)
{
    EXPECT_TRUE( equalsClose( 0.0, 0.000001 ) );
    EXPECT_TRUE( equalsClose( 0.0,-0.000001 ) );
    EXPECT_TRUE( equalsClose( 0.0,-0.0      ) );
}

TEST(Math,Utils_EqualsClose_Negatives)
{
    EXPECT_TRUE( equalsClose( -2.00, -2.00001 ) );
}

//===========================================================================
// Close equals zero
//===========================================================================
TEST(Math,Utils_IsZero_Zero_BasicTypes)
{
    EXPECT_TRUE( isZero(static_cast<int>(0)) );

    EXPECT_FALSE( isZero(static_cast<int>( 1  )) );
    EXPECT_FALSE( isZero(static_cast<int>( -1 )) );
    EXPECT_FALSE( isZero(static_cast<int>( 2  )) );

    EXPECT_TRUE( isZero(static_cast<unsigned int>( 0 )) );
    EXPECT_TRUE( isZero(static_cast<bool>( false )) );
    EXPECT_TRUE( isZero(static_cast<char>( 0 )) );
    EXPECT_TRUE( isZero(static_cast<long long>( 0 )) );
}

TEST(Math,Utils_IsZero_Zero)
{
    EXPECT_TRUE( isZero( 0.00 ) );
}

TEST(Math,Utils_IsZero_NotZero)
{
    EXPECT_FALSE( isZero( 0.1 ) );
}

TEST(Math,Utils_IsZero_WithinBounds)
{
    const int ZeroBounds = 0.000000001;

    EXPECT_TRUE( isZero( 0.0000000009 ) );
    EXPECT_TRUE( isZero(-0.0000000009 ) );
}

TEST(Math,Utils_IsZero_OutsideBounds)
{
    EXPECT_FALSE( isZero( 0.000000002 ) );
    EXPECT_FALSE( isZero(-0.000000002 ) );
}

//===========================================================================
// Close compare to zero
//===========================================================================
TEST(Math,Utils_NotZero_True)
{
    EXPECT_TRUE(Math::notZero( 1 ));
    EXPECT_TRUE(Math::notZero( (double) 1.0  ));
    EXPECT_TRUE(Math::notZero( (float)  1.0f ));
    EXPECT_TRUE(Math::notZero( (bool) true ));
}

TEST(Math,Utils_NotZero_False)
{
    EXPECT_FALSE(Math::notZero( 0 ));
    EXPECT_FALSE(Math::notZero( (double) 0.0  ));
    EXPECT_FALSE(Math::notZero( (float)  0.0f ));
    EXPECT_FALSE(Math::notZero( (bool) false ));
}

//===========================================================================
// Clamp
//===========================================================================
TEST(Math,Utils_Clamp_LessThanMinimum)
{
    int a = 0, b = 2, v = -1;
    EXPECT_EQ( a, clamp( v, a, b ) );
}

TEST(Math,Utils_Clam_pMoreThanMinimum)
{
    int a = 0, b = 2, v = 3;
    EXPECT_EQ( b, clamp( v, a, b ) );
}

TEST(Math,Utils_Clamp_InBetween)
{
    int a = 0, b = 2, v = 1;
    EXPECT_EQ( v, clamp( v, a, b ) );
}

TEST(Math,Utils_Clamp_LessEquals)
{
    int a = 0, b = 2, v = 0;
    EXPECT_EQ( a, clamp( v, a, b ) );
}

//===========================================================================
// Wrap
//===========================================================================
TEST(Math,Utils_Wrap_ZeroToTen)
{
    EXPECT_EQ( 0, wrap( 0, 10 ) );
}

TEST(Math,Utils_Wrap_LessTenToTen)
{
    EXPECT_EQ( 5, wrap( 5, 10 ) );
}

TEST(Math,Utils_Wrap_MoreTenToTen)
{
    EXPECT_EQ( 2, wrap( 22, 10 ) );
}

//===========================================================================
// Snap
//===========================================================================
TEST(Math,Utils_Snap_SameValue)
{
    EXPECT_EQ( 2, snap( 2, 2 ) );
}

TEST(Math,Utils_Snap_GreaterMultiple)
{
    EXPECT_EQ( 4, snap( 4, 2 ) );
}

TEST(Math,Utils_Snap_HalfToGreaterMultiple)
{
    EXPECT_EQ( 6, snap( 5, 2 ) );
}

TEST(Math,Utils_Snap_LessHalfToLessMultiple)
{
    EXPECT_EQ( 5, snap( 6, 5 ) );
}

TEST(Math,Utils_Snaps_ToZeroAsMultiple)
{
    EXPECT_EQ( 0, snap( 1, 3 ) );
}

TEST(Math,Utils_Snaps_NearestNegativeMultipleIfNegative)
{
    EXPECT_EQ( -10, snap( -9, 5 ) );
}

//===========================================================================
// square and cube
//===========================================================================
TEST(Math,Utils_Square_Value)
{
    EXPECT_EQ( 25, square( 5 ) );
}

TEST(Math,Utils_Square_Zero)
{
    EXPECT_EQ( 0, square( 0 ) );
}

TEST(Math,Utils_Square_One)
{
    EXPECT_EQ( 1, square( 1 ) );
}

TEST(Math,Utils_Cube_Value)
{
    EXPECT_EQ( 125, cube( 3 ) );
}

TEST(Math,Utils_Cube_Zero)
{
    EXPECT_EQ( 0, cube( 0 ) );
}

TEST(Math,Utils_Cube_One)
{
    EXPECT_EQ( 1, cube( 1 ) );
}

//===========================================================================
// Cast and clamp
//===========================================================================
TEST(Math,Utils_ClampedCast_SameTypeDoesNotChangeValue)
{
    int result = clampedCast<int, int>( 1024 );
    EXPECT_EQ( 1024, result );
}

TEST(Math,Utils_ClampedCast_SmallerTypeDoesNotChangeIfInRange)
{
    char result = clampedCast<int, char>( 64 );
    EXPECT_EQ( (char) 64, result );
}

TEST(Math,Utils_ClampedCast_UnsignedClampsAtZero)
{
    unsigned int result = clampedCast<int, unsigned int>( -1024 );
    EXPECT_EQ( (unsigned int) 0, result );
}

TEST(Math,Utils_ClampedCast_FromLargerToEdge)
{
    unsigned char result = clampedCast<int, unsigned char>( 671 );
    EXPECT_EQ( (unsigned char) 127, result );
}

TEST(Math,Utils_ClampedCast_ToSmallerTypeClampsAtMax)
{
    unsigned char result = clampedCast<int, unsigned char>( 127 );
    EXPECT_EQ( (unsigned char) 127, result );
}

TEST(Math,Utils_ClampedCast_ToSmallerSignedTypeClampsAtMax)
{
    signed char result = clampedCast<int, signed char>( 255 );
    EXPECT_EQ( (signed char) 255, result );
}

TEST(Math,Utils_Rad2Deg_Values)
{
    // pi constant deliberately not used
    EXPECT_FLOAT_EQ( 0.0f,   rad2deg( 0.00000000f ) );
    EXPECT_FLOAT_EQ( 90.0f,  rad2deg( 1.57079633f ) );
    EXPECT_FLOAT_EQ( 180.0f, rad2deg( 3.14159265f ) );
    EXPECT_FLOAT_EQ( 360.0f, rad2deg( 6.28318531f ) );
    EXPECT_FLOAT_EQ( 420.0f, rad2deg( 7.33038286f ) );
}

TEST(Math,Utils_Deg2Rad_Values)
{
    // pi constant deliberately not used
    EXPECT_FLOAT_EQ( 0.00000000f, deg2rad( 0.0f ) );
    EXPECT_FLOAT_EQ( 1.57079633f, deg2rad( 90.0f ) );
    EXPECT_FLOAT_EQ( 3.14159265f, deg2rad( 180.0f ) );
    EXPECT_FLOAT_EQ( 6.28318531f, deg2rad( 360.0f ) );
    EXPECT_FLOAT_EQ( 7.33038286f, deg2rad( 420.0f ) );
}

TEST(Math,Utils_ClampAngle_IntValues)
{
    EXPECT_EQ( 270, clampAngle( -90 ) );
    EXPECT_EQ( 0,   clampAngle( 0 ) );
    EXPECT_EQ( 180, clampAngle( 180 ) );
    EXPECT_EQ( 0,   clampAngle( 360 ) );
    EXPECT_EQ( 60,  clampAngle( 420 ) );
    EXPECT_EQ( 0,   clampAngle( 720 ) );
}

TEST(Math,Utils_ClampAngle_FloatValues)
{
    EXPECT_FLOAT_EQ( 270.0f, clampAngle( -90.0f ) );
    EXPECT_FLOAT_EQ( 0.0f,   clampAngle( 0.0f ) );
    EXPECT_FLOAT_EQ( 180.0f, clampAngle( 180.0f ) );
    EXPECT_FLOAT_EQ( 0.0f,   clampAngle( 360.0f ) );
    EXPECT_FLOAT_EQ( 60.0f,  clampAngle( 420.0f ) );
    EXPECT_FLOAT_EQ( 0.0f,   clampAngle( 720.0f ) );
}

TEST(Math,Utils_ClampAngle_DoubleValues)
{
    EXPECT_FLOAT_EQ( 270.0, clampAngle( -90.0 ) );
    EXPECT_FLOAT_EQ( 0.0,   clampAngle( 0.0 ) );
    EXPECT_FLOAT_EQ( 180.0, clampAngle( 180.0 ) );
    EXPECT_FLOAT_EQ( 0.0,   clampAngle( 360.0 ) );
    EXPECT_FLOAT_EQ( 60.0,  clampAngle( 420.0 ) );
    EXPECT_FLOAT_EQ( 0.0,   clampAngle( 720.0 ) );
}

TEST(Math,Utils_NextPowerOfTwo_Values)
{
    EXPECT_EQ( 1, nextPowerOfTwo( 0 ) );
    EXPECT_EQ( 2, nextPowerOfTwo( 1 ) );
    EXPECT_EQ( 4, nextPowerOfTwo( 2 ) );
    EXPECT_EQ( 4, nextPowerOfTwo( 4 ) );
    EXPECT_EQ( 16, nextPowerOfTwo( 12 ) );
}
