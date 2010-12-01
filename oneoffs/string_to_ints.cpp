#include <iostream>
#include <string>

int main( int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        std::cerr << "usage: [string]" << std::endl;
    }

    std::string input = argv[1];

    for ( size_t i = 0; i < input.size(); ++i )
    {
        std::cout << input[i] << ": "
                  << static_cast<unsigned int>( input[i] )
                  << std::endl;
    }
}
