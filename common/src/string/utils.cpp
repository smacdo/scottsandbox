#include "string/util.h"
#include <algorithm>
#include <cctype>

std::string toUpper( const std::string& input )
{
    std::string output( input.size(), ' ');

    std::transform( input.begin(), input.end(),
                    output.begin(),
                    ::toupper );
}
