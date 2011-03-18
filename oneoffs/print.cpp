#include <iostream>
#include <vector>
#include <string>

#include <typeinfo>

template<typename T>
void printContainer( const T& container,
                     std::ostream& outputStream,
                     const std::string divider )
{
    bool isFirstElement = true;
    typename T::const_iterator itr;

    for ( itr = container.begin(); itr != container.end(); ++itr )
    {
        if ( isFirstElement )
        {
            isFirstElement = false;
        }
        else
        {
            outputStream << divider;
        }

        outputStream << *itr;
    }
}

template<typename T>
std::ostream& print( const T& value,
                     std::ostream& outputStream,
                     const std::string& name = "-" )
{
    outputStream << "<"     << name
                 << ":"     << typeid(value).name()
                 << "> = '" << value
                 << "'";

    return outputStream;
}

#define DEBUG_PRINT(x) print(x, std::cerr, #x)

int main( int, char ** )
{
    std::vector<int> vec;
    vec.push_back( 42 );
    vec.push_back( 0 );
    vec.push_back( 22 );

    printContainer( vec, std::cout, ", " );

    std::cout << std::endl;


    std::string myName = "ohhaithere";
    //print( myName, std::cout, "myName" );
    DEBUG_PRINT( myName );
    std::cout << std::endl;

    int answer = 42;
    DEBUG_PRINT( answer ) << std::endl;
}
