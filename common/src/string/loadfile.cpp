#include <string/util.h>

#include <fstream>
#include <string>
#include <vector>

/**
 * Super quick utility method to load a string from a file
 */
bool loadfile( const std::string& filename, std::string& output )
{
    // Open requested file
    std::ifstream file( filename.c_str(), std::ios::ate );

    // Make sure we were able to load the file
    if (! file.good() ) return false;

    // Get the size of the file
    size_t size = file.tellg();

    // Read file into buffer
    char * buffer = new char[ size ];
    file.read( buffer, size );

    // Convert it to a string and return it
    output = std::string( buffer );

    delete[] buffer;
    return true;

    // -- simple way of doing it --
    // output = std::string( std::istreambuf_iterator<char>(file),
    //                       std::istreambuf_iterator<char>()      );
}


bool loadFileIntoArray( const std::string& filename,
                        std::vector<std::string>& output )
{
    std::ifstream file( filename.c_str(), std::ios::ate );
    std::string line;

    while ( std::getline( file, line ) )
    {
        output.push_back( line );
    }

    return true;
}
