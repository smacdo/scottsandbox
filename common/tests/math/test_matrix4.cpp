/**
 * unit tests for templated matrix 4x4
 * (template type=float)
 */
#include <googletest/googletest.h>
#include <math/matrix.h>

const float DELTA = 0.00001;

TEST(Math,Matrix_4x4_NoValueCtor)
{
    Mat4 m();
    EXPECT_TRUE( true );   // no checks, just here to make sure compile
}

TEST(Math,Matrix_4x4_ValueCtor)
{
    Mat4 m( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );

    // first row [ 2, 4, 6, 9 ]
    EXPECT_FLOAT_EQ( 2.0, m.at(0,0) );
    EXPECT_FLOAT_EQ( 4.0, m.at(0,1) );
    EXPECT_FLOAT_EQ( 6.0, m.at(0,2) );
    EXPECT_FLOAT_EQ( 9.0, m.at(0,3) );

    // second row [ 1.2, 3.0, 5.0, 7.0 ]
    EXPECT_FLOAT_EQ( 1.2, m.at(1,0) );
    EXPECT_FLOAT_EQ( 3.0, m.at(1,1) );
    EXPECT_FLOAT_EQ( 5.0, m.at(1,2) );
    EXPECT_FLOAT_EQ( 7.0, m.at(1,3) );

    // third row [ 9.5, 1.5, 1.0, 0.0 ]
    EXPECT_FLOAT_EQ( 9.5, m.at(2,0) );
    EXPECT_FLOAT_EQ( 1.5, m.at(2,1) );
    EXPECT_FLOAT_EQ( 1.0, m.at(2,2) );
    EXPECT_FLOAT_EQ( 0.0, m.at(2,3) );

    // fourth row [ 2.8, 9.8, 6.6, 8.8 ]
    EXPECT_FLOAT_EQ( 2.8, m.at(3,0) );
    EXPECT_FLOAT_EQ( 9.8, m.at(3,1) );
    EXPECT_FLOAT_EQ( 6.6, m.at(3,2) );
    EXPECT_FLOAT_EQ( 8.8, m.at(3,3) );
}

TEST(Math,Matrix_4x4_ValuePtrCtor)
{
    float va[16] = { 2.0, 1.2, 9.5, 2.8,
                     4.0, 3.0, 1.5, 9.8,
                     6.0, 5.0, 1.0, 6.6,
                     9.0, 7.0, 0.0, 8.8 };

    Mat4 m( (float*) va );

    // first row [ 2, 4, 6, 9 ]
    EXPECT_FLOAT_EQ( 2.0, m.at(0,0) );
    EXPECT_FLOAT_EQ( 4.0, m.at(0,1) );
    EXPECT_FLOAT_EQ( 6.0, m.at(0,2) );
    EXPECT_FLOAT_EQ( 9.0, m.at(0,3) );

    // second row [ 1.2, 3.0, 5.0, 7.0 ]
    EXPECT_FLOAT_EQ( 1.2, m.at(1,0) );
    EXPECT_FLOAT_EQ( 3.0, m.at(1,1) );
    EXPECT_FLOAT_EQ( 5.0, m.at(1,2) );
    EXPECT_FLOAT_EQ( 7.0, m.at(1,3) );

    // third row [ 9.5, 1.5, 1.0, 0.0 ]
    EXPECT_FLOAT_EQ( 9.5, m.at(2,0) );
    EXPECT_FLOAT_EQ( 1.5, m.at(2,1) );
    EXPECT_FLOAT_EQ( 1.0, m.at(2,2) );
    EXPECT_FLOAT_EQ( 0.0, m.at(2,3) );

    // fourth row [ 2.8, 9.8, 6.6, 8.8 ]
    EXPECT_FLOAT_EQ( 2.8, m.at(3,0) );
    EXPECT_FLOAT_EQ( 9.8, m.at(3,1) );
    EXPECT_FLOAT_EQ( 6.6, m.at(3,2) );
    EXPECT_FLOAT_EQ( 8.8, m.at(3,3) );
}

TEST(Math,Matrix_4x4_CopyCtor)
{
    Mat4 m( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );

    Mat4 r( m );

    EXPECT_EQ( m, r );
}

TEST(Math,Matrix_4x4_Assignment)
{
    Mat4 m( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );
    Mat4 r;
    
    // Make sure empty matrix != value constructed matrix
    EXPECT_NE( m, r );

    // Assign it, then check for equality
    r = m;
    EXPECT_EQ( m, r );
}

TEST(Math,Matrix_4x4_Addition)
{
    Mat4 a(  3.2, -0.0, 4.0, 2.0,
            -1.0,  2.0, 1.2, 0.0,
             3.0,  1.0, 7.0, 2.5,
            -2.0,  4.0, 5.0, 6.0 );

    Mat4 b(  1.1, -2.0, 1.5, 3.0,
             1.0,  1.2, 2.2,-2.2,
             4.0,  0.0, 1.5, 0.2,
             1.9, -4.0, 2.5,-1.0 );

    Mat4 v(  4.3, -2.0, 5.5, 5.0,
             0.0,  3.2, 3.4,-2.2,
             7.0,  1.0, 8.5, 2.7,
            -0.1,  0.0, 7.5, 5.0 );

    Mat4 r( a + b );

    EXPECT_EQ( v, r );
}

TEST(Math,Matrix_4x4_SelfAddition)
{
    Mat4 a(  3.2, -0.0, 4.0, 2.0,
            -1.0,  2.0, 1.2, 0.0,
             3.0,  1.0, 7.0, 2.5,
            -2.0,  4.0, 5.0, 6.0 );

    Mat4 b(  1.1, -2.0, 1.5, 3.0,
             1.0,  1.2, 2.2,-2.2,
             4.0,  0.0, 1.5, 0.2,
             1.9, -4.0, 2.5,-1.0 );

    Mat4 v(  4.3, -2.0, 5.5, 5.0,
             0.0,  3.2, 3.4,-2.2,
             7.0,  1.0, 8.5, 2.7,
            -0.1,  0.0, 7.5, 5.0 );

    a += b;

    EXPECT_EQ( a, v );
    EXPECT_NE( b, v );
    EXPECT_NE( a, b );
}

TEST(Math,Matrix_4x4_Subtraction)
{
    Mat4 a(  3.2, -0.0, 4.0, 2.0,
            -1.0,  2.0, 1.2, 0.0,
             3.0,  1.0, 7.0, 2.5,
            -2.0,  4.0, 5.0, 6.0 );

    Mat4 b(  1.1, -2.0, 1.5, 3.0,
             1.0,  1.2, 2.2,-2.2,
             4.0,  0.0, 1.5, 0.2,
             1.9, -4.0, 2.5,-1.0 );

    Mat4 v(  2.1,  2.0, 2.5,-1.0,
            -2.0,  0.8,-1.0, 2.2,
            -1.0,  1.0, 5.5, 2.3,
            -3.9,  8.0, 2.5, 7.0 );

    Mat4 r( a - b );

    EXPECT_EQ( v, r );
}

TEST(Math,Matrix_4x4_SelfSubtraction)
{
    Mat4 a(  3.2, -0.0, 4.0, 2.0,
            -1.0,  2.0, 1.2, 0.0,
             3.0,  1.0, 7.0, 2.5,
            -2.0,  4.0, 5.0, 6.0 );

    Mat4 b(  1.1, -2.0, 1.5, 3.0,
             1.0,  1.2, 2.2,-2.2,
             4.0,  0.0, 1.5, 0.2,
             1.9, -4.0, 2.5,-1.0 );

    Mat4 v(  2.1,  2.0, 2.5,-1.0,
            -2.0,  0.8,-1.0, 2.2,
            -1.0,  1.0, 5.5, 2.3,
            -3.9,  8.0, 2.5, 7.0 );
    a -= b;

    EXPECT_EQ( a, v );
    EXPECT_NE( a, b );
    EXPECT_NE( b, v );
}

TEST(Math,Matrix_4x4_Multiplication)
{
    Mat4 a( 0.0, 1.0, 3.0, 5.0,
            2.0, 3.0, 8.0, 9.0,
            3.0, 4.0, 1.0, 2.0,
            7.0, 0.0, 6.0, 6.0 );

    Mat4 b( 2.0, 1.0, 4.0, 8.0,
            9.0, 2.0, 1.0, 5.0,
            7.0, 6.0, 6.0, 7.0,
            9.0, 5.0, 4.0, 3.0 );

    Mat4 v(  75.0,  45.0,  39.0,  41.0,
            168.0, 101.0,  95.0, 114.0,
             67.0,  27.0,  30.0,  57.0,
            110.0,  73.0,  88.0, 116.0 );

    EXPECT_EQ( v, a * b );
}

TEST(Math,Matrix_4x4_Multiplication2)
{
        Mat4 a(  0.2,  1.0,  0.5, -1.2,
                 0.6, -1.4,  0.0, -2.0,
                -0.1, -1.1,  0.3, -0.0,
                 1.0,  0.5,  0.2,  0.8 );

        Mat4 b(  0.4,  0.5,  0.6, 1.0,
                 0.2,  1.0,  1.5, 1.2,
                 0.8,  1.4,  0.1, 0.1,
                 0.6,  1.8,  0.9, 1.1 );

        Mat4 v(  -1.0/25.0,  -9.0/25.0,    59.0/100.0,   13.0/100.0,
                 -31.0/25.0, -47.0/10.0,  -177.0/50.0,  -82.0/25.0,
                 -1.0/50.0,  -73.0/100.0, -42.0/25.0,   -139.0/100.0,
                  57.0/50.0,  68.0/25.0,   209.0/100.0,  5.0/2.0 );

        EXPECT_EQ( v, a * b );
}

TEST(Math,Matrix_4x4_Transpose)
{
    Mat4 a(  1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16 );

    Mat4 v(  1,  5,  9, 13,
             2,  6, 10, 14,
             3,  7, 11, 15,
             4,  8, 12, 16 );

    EXPECT_EQ( v, a );
}

TEST(Math,Matrix_4x4_SelfEquality)
{
    Mat4 a;

    EXPECT_EQ( a, a );
}

TEST(Math,Matrix_4x4_Equality)
{
    Mat4 m( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );
    Mat4 n( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );

    EXPECT_EQ( m, n );
    EXPECT_FALSE( m != n );

    // sanity
    n = ( m + n );

    EXPECT_FALSE( m == n );
    EXPECT_TRUE( m != n );
}


TEST(Math,Matrix_4x4_SelfInequalityFalse)
{
    Mat4 m;

    EXPECT_FALSE( m != m );
}

TEST(Math,Matrix_4x4_Inequality)
{
    Mat4 m( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );
    Mat4 n( 2.0, 4.0, 6.0, 9.0, 
            1.2, 3.0, 5.0, 7.0,
            9.5, 1.5, 1.0, 0.0,
            2.8, 9.8, 6.6, 8.8 );

    EXPECT_FALSE( (m != n) );
    EXPECT_TRUE ( (m == n) );

    // sanity
    n = m + n;

    EXPECT_NE( m, n );
}

TEST(Math,Matrix_4x4_ZeroMatrix)
{
    Mat4 m( 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0 );
    Mat4 n = m.ZeroMatrix();

    EXPECT_EQ( m, n );

    EXPECT_TRUE( m.isZeroMatrix() );
    EXPECT_TRUE( n.isZeroMatrix() );
}

TEST(Math,Matrix_4x4_MakeIdentityMatrix)
{
    Mat4 a( 1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0 );
    Mat4 b = Mat4::IdentityMatrix();

    EXPECT_EQ( a, b );
    EXPECT_TRUE( a.isIdentityMatrix() );
}

TEST(Math,Matrix_4x4_IsIdentityMatrix)
{
    Mat4 a( 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 0.0 );
    Mat4 b( 1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0 );

    EXPECT_TRUE(! a.isIdentityMatrix() );
    EXPECT_TRUE(  b.isIdentityMatrix() );
}

TEST(Math,Matrix_4x4_SwapMatrix)
{
    Mat4 a( 1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 0.0,10.0,11.0,
            12.0,13.0,14.0,15.0 );

    Mat4 b(16.0,17.0,18.0,19.0,
            20.0,21.0,22.0,23.0,
            24.0,26.0,26.0,27.0,
            28.0,29.0,30.0,31.0 );

    Mat4 x( a );
    Mat4 y( b );

    // pre-condition (x==a, y==b, x!=y)
    EXPECT_EQ( a, x );
    EXPECT_EQ( b, y );
    EXPECT_NE( x, y );

    // swap and check post condition (x==b, y==a, x!=y)
    swap( x, y );

    EXPECT_EQ( b, x );
    EXPECT_EQ( a, y );
    EXPECT_NE( x, y );
}

