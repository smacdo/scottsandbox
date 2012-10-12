#include <googletest/googletest.h>
#include <common/bits.h>

using namespace BitUtils;

TEST(Common,Bits_SetBit)
{
    int i = 0;

    set( i, 0 );
    EXPECT_EQ( 1, i );
}

TEST(Common,Bits_ClearBit)
{

}

TEST(Common,Bits_FlipBit)
{

}

TEST(Common,Bits_TestBit)
{

}
