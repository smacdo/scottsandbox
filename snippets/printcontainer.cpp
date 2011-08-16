#include <vector>
#include <iostream>
#include <sstream>

template<typename T>
std::string dumpContainer( const T& container, std::string sep )
{
    typename T::const_iterator itr;
    std::stringstream sstream;

    for ( itr = container.begin(); itr != container.end(); ++itr )
    {
        // insert a seperator char for all but the first element
        if ( itr != container.begin() )
        {
            sstream << sep;
        }

        sstream << *itr;
    }

    return sstream.str();
}

int main( int argc, char* argv[] )
{
    std::vector<int> values = { 1, 2, 3, 5 };
    std::cout << dumpContainer( values, ", " ) << std::endl;
}

