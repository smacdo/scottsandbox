#include <googletest/googletest.h>
#include <string/stringutil.h>

using namespace StringUtil;

TEST(Common,String_StartsWith)
{
    EXPECT_TRUE( startsWith( "hello there", "h" ) );
    EXPECT_TRUE( startsWith( "hello there", "hello" ) );
    EXPECT_TRUE( startsWith( "hello there", "hello there" ) );

    EXPECT_FALSE( startsWith( "hello there", "el" ) );
}

TEST(Common,String_EndsWith)
{
/*    EXPECT_TRUE( endsWith( "file.jpg", "jpg" ) );
    EXPECT_TRUE( endsWith( "file.jpg", ".jpg" ) );
    
    EXPECT_FALSE( endsWith( "file.jpg", ".jp" ) );
    EXPECT_FALSE( endsWith( "file.jpg", ".jpG" ) );
    EXPECT_FALSE( endsWith( "file.jpg", "file" ) );*/
}

TEST(Common,String_LeftTrim)
{
    EXPECT_EQ( "scott", ltrim( "        scott" ) );
    EXPECT_EQ( "sc o  tt \t ", ltrim( "sc o  tt \t " ) );
    EXPECT_EQ( "sc o  tt \t ", ltrim( "  sc o  tt \t " ) );

}

TEST(Common,String_RightTrim)
{

}

TEST(Common,String_Trim)
{

}

TEST(Common,String_ToUpper)
{

}

TEST(Common,String_MakeSuffix)
{

}

TEST(Common,String_Replace)
{

}

TEST(Common,String_PrintHex_ByteArray)
{

}

TEST(Common,String_PrintHex_Object)
{

}

TEST(Common,String_PrintBinary_ByteArray)
{

}

TEST(Common,String_PrintBinary_Object)
{

}

