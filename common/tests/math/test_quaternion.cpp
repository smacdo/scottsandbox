/**
 * Unit tests for templated vector3 class. These tests use float32 for the
 * numeric type, but can be adjusted to double precision.
 */
#include <googletest/googletest.h>
#include <math/quaternion.h>

TEST(Math, Quaternion_DefaultContructor)
{
    const Quat q;
    EXPECT_TRUE( true );      // test here just to make sure it compiles
}

TEST(Math, Quaternion_SizeTest)
{
    // Verify that the quaternion is 16 bytes in size (4 * 4)
    const Quat q( 1.0, 2.0, 3.0, 4.0 );
    EXPECT_EQ( 16u, sizeof(q) );
}

TEST(Math, Quaternion_ValueConstructor)
{
    Quat q( 3.0, 2.0, -4.0, 6.0 );

    EXPECT_FLOAT_EQ(  3.0, q.x() );
    EXPECT_FLOAT_EQ(  2.0, q.y() );
    EXPECT_FLOAT_EQ( -4.0, q.z() );
    EXPECT_FLOAT_EQ(  6.0, q.w() );
}

TEST(Math, Quaternion_CopyConstructor)
{
    const Quat a( 1.0, -2.0, 0.2, 3.5 );
    const Quat q( a );

    EXPECT_FLOAT_EQ(  1.0, q.x() );
    EXPECT_FLOAT_EQ( -2.0, q.y() );
    EXPECT_FLOAT_EQ(  0.2, q.z() );
    EXPECT_FLOAT_EQ(  3.5, q.w() );
}

TEST(Math, Quaternion_ConstIndexOperatorRead)
{
    const Quat q( 1.0, -2.0, 0.2, 3.5 );

    EXPECT_FLOAT_EQ(  1.0, q[0] );
    EXPECT_FLOAT_EQ( -2.0, q[1] );
    EXPECT_FLOAT_EQ(  0.2, q[2] );
    EXPECT_FLOAT_EQ(  3.5, q[3] );
}

TEST(Math, Quaternion_IndexOperatorWrite)
{
    Quat q( 1.0, -2.0, 0.2, 3.5 );

    q[0] =  2.0;
    q[1] =  0.2;
    q[2] = -1.0;
    q[3] =  2.5;

    EXPECT_FLOAT_EQ(  2.0, q.x() );
    EXPECT_FLOAT_EQ(  0.2, q.y() );
    EXPECT_FLOAT_EQ( -1.0, q.z() );
    EXPECT_FLOAT_EQ(  2.5, q.w() );

}

TEST(Math, Quaternion_Assignment)
{
    Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 3.0, 4.0, 5.0, 6.0 );
    
    a = b;

    EXPECT_NE( Quat( 1.0, 2.0, 3.0, 4.0 ), a );
    EXPECT_EQ( Quat( 3.0, 4.0, 5.0, 6.0 ), a );
}

TEST(Math, Quaternion_Equality)
{
    const Quat a( 1.5, -0.2, 0.0, 1.0 );
    const Quat b( 1.5,  0.2, 0.0, 0.7 );
    const Quat c( 1.5, -0.2, 0.1, 1.0 );
    const Quat d( 1.5,  0.0,-0.2, 0.2 );
    const Quat e( 0.0,  1.5,-0.2, 0.2 );
    const Quat f( 1.5, -0.2, 0.0, 1.0 );

    EXPECT_EQ( a, f );
    EXPECT_EQ( a, a );
    EXPECT_FALSE( a == b );
    EXPECT_FALSE( a == c );
    EXPECT_FALSE( a == d );
    EXPECT_FALSE( a == e );
    EXPECT_TRUE( a == f );
    EXPECT_TRUE( a == a );
}

TEST(Math, Quaternion_Inequality)
{
    const Quat a( 1.5, -0.2, 0.0, 1.0 );
    const Quat b( 1.5,  0.2, 0.0, 0.7 );
    const Quat c( 1.5, -0.2, 0.1, 1.0 );
    const Quat d( 1.5,  0.0,-0.2, 0.2 );
    const Quat e( 0.0,  1.5,-0.2, 0.2 );
    const Quat f( 1.5, -0.2, 0.0, 1.0 );

    EXPECT_TRUE( a != b );
    EXPECT_TRUE( a != c );
    EXPECT_TRUE( a != d );
    EXPECT_TRUE( a != e );
    EXPECT_FALSE( a != f );
    EXPECT_FALSE( a != a );
}

TEST(Math, Quaternion_Multiply)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );
    const Quat r = a * b;

    EXPECT_EQ( r, Quat( 6.0, 10.0, 18.0, 5.5 ) );
}

TEST(Math, Quaternion_SelfMultiply)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );

    Quat r  = a;
         r *= b;  

    EXPECT_EQ( r, Quat( 6.0, 10.0, 18.0, 5.5 ) );
}

TEST(Math, Quaternion_X)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );

    EXPECT_EQ( a.x(), 1.0 );
    EXPECT_EQ( b.x(), 1.5 );
}

TEST(Math, Quaternion_Y)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );

    EXPECT_EQ( a.y(), 2.0 );
    EXPECT_EQ( b.y(), 2.5 );
}


TEST(Math, Quaternion_Z)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );

    EXPECT_EQ( a.z(), 3.0 );
    EXPECT_EQ( b.z(), 3.5 );
}


TEST(Math, Quaternion_W)
{
    const Quat a( 1.0, 2.0, 3.0, 4.0 );
    const Quat b( 1.5, 2.5, 3.5, 4.5 );

    EXPECT_EQ( a.w(), 4.0 );
    EXPECT_EQ( b.w(), 4.5 );
}

TEST(Math,Quaternion_Magnitude)
{
    const Quat q( 1.0, 2.0, 3.0, 4.0 );
    EXPECT_FLOAT_EQ( 0.0, magnitude( q ) );
}

TEST(Math,Quaternion_Conjugate)
{
    const Quat q( 1.0, 2.0, 3.0, 4.0 );
    const Quat c = conjugate( q );

    EXPECT_EQ( c, Quat( 1.0, 2.0, 3.0, 4.0 ) );
}

TEST(Math,Quaternion_Inverse)
{
    const Quat q( 1.0, 2.0, 3.0, 4.0 );
    const Quat c = inverse( q );

    EXPECT_EQ( c, Quat( 1.0, 2.0, 3.0, 4.0 ) );
}

TEST(Math, Quaternion_Zero)
{
    const Quat q = Quat::ZERO;

    EXPECT_EQ( q.x(), 0.0 );
    EXPECT_EQ( q.y(), 0.0 );
    EXPECT_EQ( q.z(), 0.0 );
    EXPECT_EQ( q.w(), 0.0 );
}

TEST(Math, Quaternion_Identity)
{
    const Quat q = Quat::IDENTITY;

    EXPECT_EQ( q.x(), 0.0 );
    EXPECT_EQ( q.y(), 0.0 );
    EXPECT_EQ( q.z(), 0.0 );
    EXPECT_EQ( q.w(), 1.0 );
}
