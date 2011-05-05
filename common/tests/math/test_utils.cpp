/**
 * Unit tests for common/math/utils
 */
#include <googletest/googletest.h>
#include <math/util.h>

using namespace Math;

//=========================================================================
// CloseEquals (fuzzy equality checking for floating point values)
//=========================================================================
TEST(MathUtils,EqualsCloseBasicEquals)
{
    // floating point
    EXPECT_TRUE( equalsClose( 42.0f, 42.0f ) );

    // double
    EXPECT_TRUE( equalsClose( 42.0, 42.0 ) );

    // int
    EXPECT_TRUE( equalsClose( 42, 42 ) );
}

TEST(MathUtils,EqualsCloseBasicNotEquals)
{
    // floating point
    EXPECT_FALSE( equalsClose( 42.0f, -42.0f ) );

    // double
    EXPECT_FALSE( equalsClose( 42.0, -42.0 ) );

    // int
    EXPECT_FALSE( equalsClose( 42, -42 ) );
}

TEST(MathUtils,EqualsCloseEdges)
{
    EXPECT_TRUE( equalsClose( 1.0, 0.99999999 ) );
    EXPECT_TRUE( equalsClose( 1.0, 1.00000001 ) );
}

TEST(MathUtils,EqualsIsZeroEdges)
{
    EXPECT_TRUE( equalsClose( 0.0, 0.000001 ) );
    EXPECT_TRUE( equalsClose( 0.0,-0.000001 ) );
    EXPECT_TRUE( equalsClose( 0.0,-0.0      ) );
}

TEST(MathUtils,EqualsCloseNegatives)
{
    EXPECT_TRUE( equalsClose( -2.00, -2.00001 ) );
}

//===========================================================================
// Close equals zero
//===========================================================================
TEST(MathUtils,IsZeroIsTrueIfZero)
{
    EXPECT_TRUE( isZero( 0.00 ) );
}

TEST(MathUtils,IsZeroIsFalseIfNotZero)
{
    EXPECT_FALSE( isZero( 0.1 ) );
}

TEST(MathUtils,IsZeroIsTrueIfWithinBounds)
{
    const int ZeroBounds = 0.000000001;

    EXPECT_TRUE( isZero( 0.0000000009 ) );
    EXPECT_TRUE( isZero(-0.0000000009 ) );
}

TEST(MathUtils,IsZeroIsFalseIfOutsideBounds)
{
    EXPECT_FALSE( isZero( 0.000000002 ) );
    EXPECT_FALSE( isZero(-0.000000002 ) );
}

//===========================================================================
// Close compare to zero
//===========================================================================
TEST(MathUtils,IsNotZeroSimple)
{
    EXPECT_FALSE( Math::isNotZero( 1 ) );
    EXPECT_FALSE( Math::isNotZero( (double) 1.0  ) );
    EXPECT_FALSE( Math::isNotZero( (float)  1.0f ) );
}

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
    int result = castAndClamp<int, int>( 1024 );
    EXPECT_EQ( 1024, result );
}

TEST(MathUtils,CastClampToSammlerTypeDoesNotChangeIfInRange)
{
    char result = castAndClamp<int, char>( 64 );
    EXPECT_EQ( (char) 64, result );
}

TEST(MathUtils,CastClampToUnsignedClampsAtZero)
{
    unsigned int result = castAndClamp<int, unsigned int>( -1024 );
    EXPECT_EQ( (unsigned int) 0, result );
}

TEST(MathUtils,CastClampFromLargerToEdge)
{
    unsigned char result = castAndClamp<int, unsigned char>( 671 );
    EXPECT_EQ( (unsigned char) 127, result );
}

TEST(MathUtils,CastClampToSmallerTypeClampsAtMax)
{
    unsigned char result = castAndClamp<int, unsigned char>( 127 );
    EXPECT_EQ( (unsigned char) 127, result );
}

TEST(MathUtils,CastClampToSmallerSignedTypeClampsAtMax)
{
    signed char result = castAndClamp<int, signed char>( 255 );
    EXPECT_EQ( (signed char) 255, result );
}

