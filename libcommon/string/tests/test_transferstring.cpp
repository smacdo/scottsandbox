#include <googletest/googletest.h>
#include <string/stringutil.h>
#include <string>

TEST(StringUtil,TransferString_Construct)
{
    std::string output;
    output = TransferString( output, 512 );
}
