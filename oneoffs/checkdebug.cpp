/**
 * Checks if debug is enabled using common macros
 */
#include <iostream>

int main( int , char** )
{
    std::cout << "List of debugging defines detected:" << std::endl;
#ifdef DEBUG
    std::cout << "DEBUG" << std::endl;
#endif

#ifdef _DEBUG
    std::cout << "_DEBUG" << std::endl;
#endif

#ifdef NDEBUG
    std::cout << "NDEBUG" << std::endl;
#endif
}
