#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void show_file( const fs::path& filepath )
{
    std::cout << filepath << std::endl;
}

void show_directory( const fs::path& dirpath )
{
    fs::directory_iterator itr( dirpath );
    fs::directory_iterator endItr;

    for ( ; itr != endItr; ++itr )
    {
        if ( fs::is_regular_file( itr->status() ) )
        {
            show_file( itr->path() );
        }
        else if ( fs::is_directory( itr->status() ) )
        {
            show_directory( itr->path() );
        }
    }
}
       

int main( int argc, char* argv[] )
{
    // Get the file to examine
    std::string filename;

    if ( argc < 2 )
    {
        std::cerr << "Usage: ./filesystem [filepath]" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        filename = argv[1];
    }

    // Create a boost path
    fs::path filepath( filename );

    // Does the file exist?
    if (! fs::exists(filepath) )
    {
        std::cerr << "File does not exist: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    // Is this a file or a directory?
    if ( fs::is_regular_file( filepath ) )
    {
        show_file( filepath );
    }
    else if ( fs::is_directory( filepath ) ) 
    {
        show_directory( filepath );
    }

    return EXIT_SUCCESS;
}
