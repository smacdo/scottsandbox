/**
 * Unit tests for common/math/utils
 */
#include <googletest/googletest.h>
#include <common/math/utils.h>

//=========================================================================
// CloseEquals (fuzzy equality checking for floating point values)
//=========================================================================
TEST(MathUtils,EqualsCloseBasicEquals)
{
    // floating point
    EXPECT_TRUE( equals_close( 42.0f, 42.0f ) );

    // double
    EXPECT_TRUE( equals_close( 42.0, 42.0 ) );

    // int
    EXPECT_TRUE( equals_close( 42, 42 ) );
}

TEST(MathUtils,EqualsCloseBasicNotEquals)
{
    // floating point
    EXPECT_FALSE( equals_close( 42.0f, -42.0f ) );

    // double
    EXPECT_FALSE( equals_close( 42.0, -42.0 ) );

    // int
    EXPECT_FALSE( equals_close( 42, -42 ) );
}

TEST(MathUtils,EqualsCloseEdges)
{
    EXPECT_TRUE( equals_close( 1.0, 0.99999999 ) );
    EXPECT_TRUE( equals_close( 1.0, 1.00000001 ) );
}

TEST(MathUtils,EqualsIsZeroEdges)
{
    EXPECT_TRUE( equals_close( 0.0, 0.000001 ) );
    EXPECT_TRUE( equals_close( 0.0,-0.000001 ) );
    EXPECT_TRUE( equals_close( 0.0,-0.0      ) );
}

TEST(MathUtils,EqualsCloseNegatives)
{
    EXPECT_TRUE( equals_close( -2.00, -2.00001 ) );
}

//===========================================================================
// Close equals zero
//===========================================================================
TEST(MathUtils,IsZeroIsTrueIfZero)
{
    EXPECT_TRUE( is_zero( 0.00 ) );
}

TEST(MathUtils,IsZeroIsFalseIfNotZero)
{
    EXPECT_FALSE( is_zero( 0.1 ) );
}

TEST(MathUtils,IsZeroIsTrueIfWithinBounds)
{
    const int ZeroBounds = 0.000000001;

    EXPECT_TRUE( is_zero( 0.0000000009 ) );
    EXPECT_TRUE( is_zero(-0.0000000009 ) );
}

TEST(MathUtils,IsZeroIsFalseIfOutsideBounds)
{
    EXPECT_FALSE( is_zero( 0.000000002 ) );
    EXPECT_FALSE( is_zero(-0.000000002 ) );
}

//===========================================================================
// Close compare to zero
//===========================================================================

//===========================================================================
// Clamp
//===========================================================================
TEST(MathUtils,ClampLessThanMinimum)
{
    int a = 0, b = 2, v = -1;
    EXPECT_EQ( a, clamp( v, a, b ) );
}

TEST(MathUtils,ClampMoreThanMinimum)
{
    int a = 0, b = 2, v = 3;
    EXPECT_EQ( b, clamp( v, a, b ) );
}

TEST(MathUtils,ClampInBetween)
{
    int a = 0, b = 2, v = 1;
    EXPECT_EQ( v, clamp( v, a, b ) );
}

TEST(MathUtils,ClampLessEquals)
{
    int a = 0, b = 2, v = 0;
    EXPECT_EQ( a, clamp( v, a, b ) );
}

//===========================================================================
// Min2
//===========================================================================
TEST(MathUtils,MinALessB)
{
    EXPECT_EQ( 2, min( 2, 5 ) );
}

TEST(MathUtils,MinAEqualB)
{
    EXPECT_EQ( 3, min( 3, 3 ) );
}

TEST(MathUtils,MinAGreaterB)
{
    EXPECT_EQ( -2, min( 5, -2 ) );
}

//===========================================================================
// Max2
//===========================================================================
TEST(MathUtils,MaxALessB)
{
    EXPECT_EQ( 5, max( 2, 5 ) );
}

TEST(MathUtils,MaxAEqualB)
{
    EXPECT_EQ( 3, max( 3, 3 ) );
}

TEST(MathUtils,MaxAGreaterB)
{
    EXPECT_EQ( 5, max( 5, -2 ) );
}

//===========================================================================
// Wrap
//===========================================================================
TEST(MathUtils,WrapZeroToTen)
{
    EXPECT_EQ( 0, wrap( 0, 10 ) );
}

TEST(MathUtils,WrapLessTenToTen)
{
    EXPECT_EQ( 5, wrap( 5, 10 ) );
}

TEST(MathUtils,WrapMoreTenToTen)
{
    EXPECT_EQ( 2, wrap( 22, 10 ) );
}

//===========================================================================
// Lerp
//===========================================================================
TEST(MathUtils,LerpStartLeft)
{
    EXPECT_EQ( 1, lerp( 1, 3, 0 ) );
}

TEST(MathUtils,LerpEndRight)
{
    EXPECT_EQ( 3, lerp( 1, 3, 1 ) );
}

TEST(MathUtils,LerpMidle)
{
    EXPECT_EQ( 3, lerp( 2.0f, 4.0f, 0.5f ) );
}

//===========================================================================
// Snap
//===========================================================================
TEST(MathUtils,SnapToSameValue)
{
    EXPECT_EQ( 2, snap( 2, 2 ) );
}

TEST(MathUtils,SnapGreaterMultiple)
{
    EXPECT_EQ( 4, snap( 4, 2 ) );
}

TEST(MathUtils,SnapHalfToGreaterMultiple)
{
    EXPECT_EQ( 6, snap( 5, 2 ) );
}

TEST(MathUtils,SnapLessHalfToLessMultiple)
{
    EXPECT_EQ( 5, snap( 6, 5 ) );
}

TEST(MathUtils,SnapsToZeroAsMultiple)
{
    EXPECT_EQ( 0, snap( 1, 3 ) );
}

TEST(MathUtils,SnapsToNearestNegativeMultipleIfNegative)
{
    EXPECT_EQ( -10, snap( -9, 5 ) );
}

//===========================================================================
// square and cube
//===========================================================================
TEST(MathUtils,ValueSquared)
{
    EXPECT_EQ( 25, square( 5 ) );
}

TEST(MathUtils,ZeroSquareIsStillZero)
{
    EXPECT_EQ( 0, square( 0 ) );
}

TEST(MathUtils,OneSquaredIsStillOne)
{
    EXPECT_EQ( 1, square( 1 ) );
}

TEST(MathUtils,ValueCubed)
{
    EXPECT_EQ( 125, cube( 3 ) );
}

TEST(MathUtils,ZeroCubedIsStillZero)
{
    EXPECT_EQ( 0, cube( 0 ) );
}

TEST(MathUtils,OneCubedIsStillOne)
{
    EXPECT_EQ( 1, cube( 1 ) );
}

//===========================================================================
// Cast and clamp
//===========================================================================
TEST(MathUtils,CastClampSameTypeDoesNotChangeValue)
{
    EXPECT_EQ( 1024, castAndClamp<int,int>( 1024 ) );
}

TEST(MathUtils,CastClampToSammlerTypeDoesNotChangeIfInRange)
{
    EXPECT_EQ( 64, castAndClamp<int,char>( 64 ) );
}

TEST(MathUtils,CastClampToUnsignedClampsAtZero)
{
    EXPECT_EQ( 0, castAndClamp<int, unsigned int>( -1024 ) );
}

TEST(MathUtils,CastClampToSmallerTypeClampsAtMax)
{
    EXPECT_EQ( 127, castAndClamp<int, unsigned char>( 127 ) );
}

TEST(MathUtils,CastClampToSmallerSignedTypeClampsAtMax)
{
    EXPECT_EQ( 255, castAndClamp<int, signed char>( 255 ) );
}

