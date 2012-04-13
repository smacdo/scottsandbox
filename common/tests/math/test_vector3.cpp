/**
 * Unit tests for template 3d vector class
 */
#include <googletest/googletest.h>
#include <math/vector.h>

// NOTES: Const is used to make sure const methods are actually const

TEST(Math, Vector3_DefaultContructor)
{
    const Vec3 v;
    EXPECT_TRUE( true );      // test here just to make sure it compiles
}

TEST(Math, Vector3_ValueConstructor)
{
    const Vec3 v( 1.0, -2.0, 0.2 );

    EXPECT_FLOAT_EQ(  1.0, v.x() );
    EXPECT_FLOAT_EQ( -2.0, v.y() );
    EXPECT_FLOAT_EQ(  0.2, v.z() );
}

TEST(Math, Vector3_ConstructByPointerConstructor)
{
    float values[3] = { 2.0, 5.0, -3.0 };
    Vec3 v( (float*) values );

    EXPECT_EQ( v[0], 2.0 );
    EXPECT_EQ( v[1], 5.0 );
    EXPECT_EQ( v[2],-3.0 );
}

TEST(Math, Vector3_CopyConstructor)
{
    const Vec3 a( 1.0, -2.0, 0.2 );
    const Vec3 b( a );

    EXPECT_FLOAT_EQ( b[0], 1.0 );
    EXPECT_FLOAT_EQ( b[1], -2.0 );
    EXPECT_FLOAT_EQ( b[2], 0.2 );
}

TEST(Math, Vector3_IndexOperatorRead)
{
    const Vec3 v( 1.0, -2.0, 0.2 );

    EXPECT_FLOAT_EQ( v[0], 1.0 );
    EXPECT_FLOAT_EQ( v[1], -2.0 );
    EXPECT_FLOAT_EQ( v[2], 0.2 );
}

TEST(Math, Vector3_IndexOperatorWrite)
{
    Vec3 v( 1.0, -2.0, 0.2 );

    v[0] = 2.0;
    v[1] = 0.2;
    v[2] = -1.0;

    EXPECT_EQ( Vec3( 2.0, 0.2, -1.0 ), v );
}

TEST(Math, Vector3_AxisAccessors)
{
    const Vec3 v( 5.0, 6.0, 1.0 );

    EXPECT_EQ( v.x(), 5.0 );
    EXPECT_EQ( v.y(), 6.0 );
    EXPECT_EQ( v.z(), 1.0 );
}

TEST(Math, Vector3_PointerAccess)
{
    const Vec3 v( 1.0, 2.0, 3.0 );
    const float * ptr = v.const_ptr();

    EXPECT_EQ( ptr[0], 1.0 );
    EXPECT_EQ( ptr[1], 2.0 );
    EXPECT_EQ( ptr[2], 3.0 );
}

TEST(Math, Vector3_PointerWrites)
{
    Vec3 v( 1.0, 2.0, 3.0 );
    float * ptr = v.ptr();

    ptr[0] = 5.0;
    ptr[1] = 6.0;
    ptr[2] = 7.0;

    EXPECT_EQ( Vec3( 5.0, 6.0, 7.0 ), v );
}

TEST(Math, Vector3_Assignment)
{
    Vec3 a( 1.0, 2.0, 3.0 );
    const Vec3 b( 3.0, 4.0, 5.0 );
    
    EXPECT_NE( a, b );

    a = b;

    EXPECT_EQ( a, b );
}

TEST(Math, Vector3_Negation)
{
    Vec3 a( 2.0, 5.0, -1.0 );
    a = -a;

    EXPECT_EQ( Vec3( -2.0, -5.0, 1.0 ), a );
}

TEST(Math, Vector3_Scaling)
{
    const Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b = a * 2.0;

    EXPECT_EQ( Vec3( 6.0, 10.0, 18.0 ), b );
}

TEST(Math, Vector3_SelfScaling)
{
    Vec3 a( 3.0, 5.0, 9.0 );
    a *= 2.0;

    EXPECT_EQ( Vec3( 6.0, 10.0, 18.0 ), a );
}

TEST(Math, Vector3_Addition)
{
    const Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b(-4.0, 6.0, 3.0 );

    EXPECT_EQ( Vec3( -1.0, 11.0, 12.0 ), a + b );
}

TEST(Math, Vector3_SelfAdditon)
{
          Vec3 a( 3.0, 5.0, 9.0 );
    const Vec3 b(-4.0, 6.0, 3.0 );

    a += b;

    EXPECT_EQ( Vec3( -1.0, 11.0, 12.0 ), a );
}

TEST(Math, Vector3_Subtraction)
{
    const Vec3 a( 5.0, 2.0, -4.0 );
    const Vec3 b(-1.0, 3.0,  8.0 );
    const Vec3 c = a - b;
    
    EXPECT_EQ(   6.0, c[0] );
    EXPECT_EQ(  -1.0, c[1] );
    EXPECT_EQ( -12.0, c[2] );
}

TEST(Math, Vector3_SelfSubtraction)
{
    Vec3       a( 5.0, 2.0, -4.0 );
    const Vec3 b(-1.0, 3.0,  8.0 );
    a -= b;
    
    EXPECT_EQ(   6.0, a[0] );
    EXPECT_EQ(  -1.0, a[1] );
    EXPECT_EQ( -12.0, a[2] );
}

TEST(Math, Vector3_SimpleDotProduct2D)
{
    const Vec3 a( 60, 80, 0 );
    const Vec3 b( 30, 40, 0 );

    EXPECT_EQ( 5000.0, dot( a, b ) );
}

TEST(Math, Vector3_SimpleDotProduct3D)
{
    const Vec3 a( 60, 80, 100 );
    const Vec3 b( 30, 40, 50 );

    EXPECT_EQ( 10000.0, dot( a, b ) );
}

TEST(Math, Vector3_DotOppositeIsNegative)
{
    // Tests axiom that dot product of opposite vectors is the
    // negative
    const Vec3 a( 60, 80, 100 );
    const Vec3 b( 30, 40, 50 );
    const Vec3 c( -30, -40, -50 );

    EXPECT_EQ( -1 * dot(a,b), dot(a,c) );
}

TEST(Math, Vector3_DotSameOrientIsLengthMul)
{
    // Tests axiom that dot product of two vectors with the same
    // oriention is the product of their lengths
    const Vec3 a( 2.0, 2.0, 2.0 );
    const Vec3 b( 4.0, 4.0, 4.0 );

    float dotVal = dot( a, b );
    float lenVal = length( a ) * length( b );

    EXPECT_FLOAT_EQ( dotVal, lenVal );
}

TEST(Math, Vector3_PerpendicularDotsAreZero)
{
    const Vec3 a( 25.0, 50.0, -25.0 );
    const Vec3 b( -125.0, 50.0, -25.0 );

    EXPECT_EQ( dot( a, b ), 0.0f );
    EXPECT_FLOAT_EQ( 90.0f, angleBetween( a, b ) );
}

TEST(Math, Vector3_Length)
{
    const Vec3 a( 1.0, 2.0, 3.0 );
    EXPECT_FLOAT_EQ( 3.741657, length( a ) );
}

TEST(Math, Vector3_LengthZero)
{
    EXPECT_FLOAT_EQ( 0.0f, length( TVector3<float>::ZeroVector() ) );
}

TEST(Math, Vector3_SquaredLength)
{
    const Vec3 a( 1.0, 2.0, 3.0 );
    EXPECT_EQ( 14.0, lengthSquared( a ) );
}

TEST(Math, Vector3_SquareLengthZero)
{
    EXPECT_FLOAT_EQ( 0.0f, lengthSquared( TVector3<float>::ZeroVector() ) );
}

TEST(Math, Vector3_AngleBetween)
{
    Vec3 a( 3.0,  5.0, 6.0 );
    Vec3 b( 1.0, -2.0, 4.0 );

    EXPECT_FLOAT_EQ( 63.679333f, angleBetween( a, b ) );
}

TEST(Math, Vector3_ZeroVector)
{
    const Vec3 a( 0.0, 0.0, 0.0 );
    const Vec3 z = Vec3::ZeroVector();

    EXPECT_TRUE( a == z );
}

TEST(Math, Vector3_Normalization)
{
    const Vec3 a( 3.0, 1.0, 2.0 );
    const Vec3 n = normalized( a );

    EXPECT_EQ( Vec3( 0.801784, 0.267261, 0.534522 ), n );
}

TEST(Math, Vector3_RotateAroundXZero)
{
    const Vec3 a( 4.0, -5.0, 2.5 );
    EXPECT_EQ( a, rotateAroundX( a, 0.0f ) );
}

TEST(Math, Vector3_RotateAroundX)
{
    // First trial
    const Vec3 a( 0.0, 1.0, 0.0 );
    EXPECT_EQ( Vec3( 0.0,  0.707106,  0.707106 ), rotateAroundX( a, 45.0f ) );
    EXPECT_EQ( Vec3( 0.0,  0.0,       1.0 ),      rotateAroundX( a, 90.0f ) );
    EXPECT_EQ( Vec3( 0.0, -1.0,       0.0 ),      rotateAroundX( a, 180.0f ) );
    EXPECT_EQ( Vec3( 0.0, -0.707106, -0.707106 ), rotateAroundX( a, 225.0f ) );
    EXPECT_EQ( Vec3( 0.0,  0.0,      -1.0 ),      rotateAroundX( a, 270.0f ) );
    EXPECT_EQ( Vec3( 0.0,  0.707106, -0.707106 ), rotateAroundX( a, 315.0f ) );

    // Second trial... moving shouldn't effect rotation at all
    const Vec3 b( 2.0, 1.0, 0.0 );
    EXPECT_EQ( Vec3( 2.0,  0.707106,  0.707106 ), rotateAroundX( b, 45.0f ) );
    EXPECT_EQ( Vec3( 2.0,  0.0,       1.0 ),      rotateAroundX( b, 90.0f ) );
    EXPECT_EQ( Vec3( 2.0, -1.0,       0.0 ),      rotateAroundX( b, 180.0f ) );
    EXPECT_EQ( Vec3( 2.0, -0.707106, -0.707106 ), rotateAroundX( b, 225.0f ) );
    EXPECT_EQ( Vec3( 2.0,  0.0,      -1.0 ),      rotateAroundX( b, 270.0f ) );
    EXPECT_EQ( Vec3( 2.0,  0.707106, -0.707106 ), rotateAroundX( b, 315.0f ) );
}

TEST(Math, Vector3_RotateAroundYZero)
{
    const Vec3 a( 4.0, -5.0, 2.5 );
    EXPECT_EQ( a, rotateAroundY( a, 0.0f ) );
}

TEST(Math, Vector3_RotateAroundY)
{
}

TEST(Math, Vector3_RotateAroundZZero)
{
    const Vec3 a( 4.0, -5.0, 2.5 );
    EXPECT_EQ( a, rotateAroundZ( a, 0.0f ) );
}

TEST(Math, Vector3_RotateAroundAxisZero)
{
    const Vec3 a( 4.0, -5.0, 2.5 );
    EXPECT_EQ( a, rotateAround( a, Vec3( 1.0, 1.0, 1.0 ), 0.0f ) );
}
