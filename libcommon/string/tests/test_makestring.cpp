#include <googletest/googletest.h>
#include <string/stringutil.h>

TEST(Common,String_MakeString)
{
    EXPECT_EQ( "blah", MakeString() << "blah" );
}
