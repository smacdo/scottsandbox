
/**
 * Super quick utility method to load a string from a file
 */
bool LoadFileAsString( const std::string& filename, std:string& output )
{
    std::ifstream file( filename.c_str() );
    
    if ( file.good() )
    {
        output = std::string( std::istreambuf_iterator<char>(file),
                              std::istreambuf_iterator<char>()      );
        return true;
    }
    else
    {
        return false;
    }
}
