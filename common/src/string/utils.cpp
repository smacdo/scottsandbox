
#include "string/utils.h"

std::string toUpper( const std::string& input )
{
    std::string output( input.size(), ' ');

    std::transform( input.begin(), input.end(),
                    output.begin(),
                    std::toupper() );
}
