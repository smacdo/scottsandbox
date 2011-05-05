/**
 * Unit tests for template 3d vector class
 */
#include <googletest/googletest.h>
#include <math/vector.h>

// NOTES: Const is used to make sure const methods are actually const

TEST(Vector3, DefaultContructor)
{
    const Vec3 v;
    EXPECT_TRUE( true );      // test here just to make sure it compiles
}

TEST(Vector3, ValueConstructor)
{
    const Vec3 v( 1.0, -2.0, 0.2 );

    EXPECT_FLOAT_EQ(  1.0, v[0] );
    EXPECT_FLOAT_EQ( -2.0, v[1] );
    EXPECT_FLOAT_EQ(  0.2, v[2] );
}

TEST(Vector3, ConstructByPointerConstructor)
{
    float values[3] = { 2.0, 5.0, -3.0 };
    Vec3 v( (float*) values );

    EXPECT_EQ( v[0], 2.0 );
    EXPECT_EQ( v[1], 5.0 );
    EXPECT_EQ( v[2],-3.0 );
}

TEST(Vector3, CopyConstructor)
{
    const Vec3 a( 1.0, -2.0, 0.2 );
    const Vec3 b( a );

    EXPECT_FLOAT_EQ( b[0], 1.0 );
    EXPECT_FLOAT_EQ( b[1], -2.0 );
    EXPECT_FLOAT_EQ( b[2], 0.2 );
}

TEST(Vector3, VectorIndexOperatorRead)
{
    const Vec3 v( 1.0, -2.0, 0.2 );

    EXPECT_FLOAT_EQ( v[0], 1.0 );
    EXPECT_FLOAT_EQ( v[1], -2.0 );
    EXPECT_FLOAT_EQ( v[2], 0.2 );
}

TEST(Vector3, VectorIndexOperatorWrite)
{
    Vec3 v( 1.0, -2.0, 0.2 );

    v[0] = 2.0;
    v[1] = 0.2;
    v[2] = -1.0;

    EXPECT_EQ( Vec3( 2.0, 0.2, -1.0 ), v );
}

TEST(Vector3, VectorAt)
{
    const Vec3 v( 5.0, 6.0, 1.0 );

    EXPECT_EQ( v.at(0), 5.0 );
    EXPECT_EQ( v.at(1), 6.0 );
    EXPECT_EQ( v.at(2), 1.0 );
}

TEST(Vector3, VectorAxisAccessors)
{
    const Vec3 v( 5.0, 6.0, 1.0 );

    EXPECT_EQ( v.x(), 5.0 );
    EXPECT_EQ( v.y(), 6.0 );
    EXPECT_EQ( v.z(), 1.0 );
}

TEST(Vector3, VectorPointerAccess)
{
    const Vec3 v( 1.0, 2.0, 3.0 );
    const float * ptr = v.ptr();

    EXPECT_EQ( ptr[0], 1.0 );
    EXPECT_EQ( ptr[1], 2.0 );
    EXPECT_EQ( ptr[2], 3.0 );
}

TEST(Vector3, VectorPointerIllegalWrites)
{
    // Testing to make sure const circumvention works. 
    // DO NOT EVER DO THIS UNLESS YOU HAVE A GOOD REASON AND EVEN 
    // THEN DO NOT DO THIS
    const Vec3 v( 1.0, 2.0, 3.0 );
    float * ptr = const_cast<float*>( v.ptr() );

    ptr[0] = 5.0;
    ptr[1] = 6.0;
    ptr[2] = 7.0;

    EXPECT_EQ( Vec3( 5.0, 6.0, 7.0 ), v );
}

TEST(Vector3, VectorAssignment)
{
    Vec3 a( 1.0, 2.0, 3.0 );
    const Vec3 b( 3.0, 4.0, 5.0 );
    
    EXPECT_NE( a, b );

    a = b;

    EXPECT_EQ( a, b );
}

TEST(Vector3, VectorNegation)
{
    Vec3 a( 2.0, 5.0, -1.0 );
    a = -a;

    EXPECT_EQ( Vec3( -2.0, -5.0, 1.0 ), a );
}

TEST(Vector3, VectorScaling)
{
    const Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b = a * 2.0;

    EXPECT_EQ( Vec3( 6.0, 10.0, 18.0 ), b );
}

TEST(Vector3, VectorSelfScaling)
{
    Vec3 a( 3.0, 5.0, 9.0 );
    a *= 2.0;

    EXPECT_EQ( Vec3( 6.0, 10.0, 18.0 ), a );
}

TEST(Vector3, VectorAddition)
{
    const Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b(-4.0, 6.0, 3.0 );

    EXPECT_EQ( Vec3( -1.0, 11.0, 12.0 ), a + b );
}

TEST(Vector3, VectorSelfAdditon)
{
          Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b(-4.0, 6.0, 3.0 );

    a += b;

    EXPECT_EQ( Vec3( -1.0, 11.0, 12.0 ), a );
}

TEST(Vector3, VectorSubtraction)
{
    const Vec3 a( 5.0, 2.0, -4.0 );
    const Vec3 b(-1.0, 3.0,  8.0 );
    const Vec3 c = a - b;
    
    EXPECT_EQ(   6.0, c[0] );
    EXPECT_EQ(  -1.0, c[1] );
    EXPECT_EQ( -12.0, c[2] );
}

TEST(Vector3, VectorSelfSubtraction)
{
    Vec3       a( 5.0, 2.0, -4.0 );
    const Vec3 b(-1.0, 3.0,  8.0 );
    a -= b;
    
    EXPECT_EQ(   6.0, a[0] );
    EXPECT_EQ(  -1.0, a[1] );
    EXPECT_EQ( -12.0, a[2] );
}

TEST(Vector3, SimpleDotProduct2D)
{
    const Vec3 a( 60, 80, 0 );
    const Vec3 b( 30, 40, 0 );

    EXPECT_EQ( 5000.0, dot( a, b ) );
}

TEST(Vector3, SimpleDotProduct3D)
{
    const Vec3 a( 60, 80, 100 );
    const Vec3 b( 30, 40, 50 );

    EXPECT_EQ( 10000.0, dot( a, b ) );
}

TEST(Vector3, VectorDotOppositeIsNegative)
{
    // Tests axiom that dot product of opposite vectors is the
    // negative
    const Vec3 a( 60, 80, 100 );
    const Vec3 b( 30, 40, 50 );
    const Vec3 c( -30, -40, -50 );

    EXPECT_EQ( -1 * dot(a,b), dot(a,c) );
}

TEST(Vector3, VectorDotSameOrientIsLengthMul)
{
    // Tests axiom that dot product of two vectors with the same
    // oriention is the product of their lengths
    const Vec3 a( 2.0, 2.0, 2.0 );
    const Vec3 b( 4.0, 4.0, 4.0 );

    float dotVal = dot( a, b );
    float lenVal = a.length() * b.length();

    EXPECT_FLOAT_EQ( dotVal, lenVal );
}

TEST(Vector3, VectorPerpendicularDotsAreZero)
{
    const Vec3 a( 25.0, 50.0, -25.0 );
    const Vec3 b( -125.0, 50.0, -25.0 );

    EXPECT_EQ( dot( a, b ), 0.0f );
    EXPECT_FLOAT_EQ( 90.0f, angleBetween( a, b ) );
}

TEST(Vector3, VectorLength)
{
    const Vec3 a( 1.0, 2.0, 3.0 );

    EXPECT_FLOAT_EQ( 3.741657, a.length() );
}

TEST(Vector3, VectorSquaredLength)
{
    const Vec3 a( 1.0, 2.0, 3.0 );
    EXPECT_EQ( 14.0, a.lengthSquared() );
}

TEST(Vector3, ZeroVector)
{
    const Vec3 a( 0.0, 0.0, 0.0 );
    const Vec3 z = Vec3::ZeroVector();

    EXPECT_TRUE( a == z );
}

