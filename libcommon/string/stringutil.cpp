#include <string/stringutil.h>
#include <common/assert.h>
#include <string>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

namespace StringUtil
{

/**
 * Checks if a word starts with the given prefix. Returns true if "word"
 * starts with "prefix".
 *
 * \param  word    The word to check
 * \param  prefix  The prefix to check
 */
bool startsWith( const std::string& word, const std::string& prefix )
{
    CORE_ASSERT( prefix.size() <= word.size(), "Prefix must be shorter than the word" );
    bool check = false;

    for ( size_t i = 0; i < prefix.size() && (!check); ++i )
    {
        check = ( word[i] == prefix[i] );
    }

    return check;
}

/**
 * Check if a string ends with the given word value
 *
 * \param  word    The word to check
 * \param  suffix  The suffix to check
 */
bool endsWith( const std::string& word, const std::string& suffix )
{
    return false;
}

std::string ltrim( const std::string& input,
                   const std::string& trimChars )
{
    std::string::size_type pos = input.find_first_not_of( trimChars );
    return std::string( input, pos, input.size() - pos );
}

std::string rtrim( const std::string& input,
                   const std::string& trimChars )
{
    std::string::size_type pos = input.find_last_not_of( trimChars );
    return std::string( input, pos );
}

std::string trim( const std::string& input,
                  const std::string& trimChars )
{
    return ltrim( rtrim( input, trimChars ), trimChars );
}

std::vector<std::string> split( const std::string& input, char delim )
{
    std::vector<std::string> output;
    std::stringstream ss( input );
    std::string item;

    while ( std::getline( ss, item, delim ) )
    {
        output.push_back( item );
    }

    return output;
}

/**
 * Loads a text file from disk and returns it's contents as a STL string.
 * This method can optionally indicate the success or failure of the
 * attempted operation.
 *
 * \param  filename   Path to the file
 * \param  pStatus    Optional boolean pointer, indicates success/fail
 * \return            Contents of the file
 */
std::string loadfile( const std::string& filename, bool * pStatus )
{
    using namespace std;
    string contents;
    bool status = true;

    ifstream ifs( filename.c_str(), ios::in | ios::binary | ios::ate );

    // Check if we were able to succesfully open the file before
    // proceeding.
    if ( ifs.good() )
    {
        // Find out the size of the file
        ifstream::pos_type fileSize = ifs.tellg();
        ifs.seekg( 0, ios::beg );

        // Slurp the file into a temporary byte array
        vector<char> bytes( fileSize );
        ifs.read( &bytes[0], fileSize );

        // Convert the byte array into a temporary string and then swap
        // the return string
        std::string temp( &bytes[0], fileSize );

        contents.swap( temp );
    }

    // Let the caller know the results of the file load
    if ( pStatus != NULL )
    {
        *pStatus = ifs.good();
    }

    return contents;
}

/**
 * Loads a text file into an array of strings. Each entry corresponds to
 * a line of text from the file
 */
std::vector<std::string> loadFileIntoArray( const std::string& filename,
                                            bool *pStatus )
{
    std::vector<std::string> output;

    // Open the file, and read all the lines from it
    std::ifstream file( filename.c_str(), std::ios::ate );

    if ( file.good() )
    {
        std::string line;

        while ( std::getline( file, line ) )
        {
            output.push_back( line );
        }

        // if caller provided a status pointer, indicate if the file read
        // was successful
        if ( pStatus != NULL )
        {
            *pStatus = ( file.fail() == false );
        }
    }
    else
    {
        // File failed to load
        if ( pStatus != NULL )
        {
            *pStatus = false;
        }
    }

    return output;
}

/**
 * Takes an input string, a string to find (findStr) and another string
 * (replaceStr) to replace all occurrences of findStr.
 *
 * \param  input       The input string to process
 * \param  findStr     The string that will be replaced when found
 * \param  replaceStr  The string to replace findStr with
 */
std::string replace( const std::string& input,
                     const std::string& findStr,
                     const std::string& replaceStr )
{
    std::string output( input );
    
    std::string::size_type pos   = input.find( findStr );
    std::string::size_type fSize = findStr.size(),
                           rSize = replaceStr.size();

    while ( pos != std::string::npos )
    {
        output.replace( pos, fSize, replaceStr );

        pos += rSize;
        pos  = output.find( findStr, pos );
    }

    return output;
}

std::ostream& printHex( std::ostream& stream,
                        const uint8_t *pBytes,
                        size_t len )
{
    const char * CHARS = "0123456789ABCDEF";

    // bail out of the pointer is null
    if ( pBytes == NULL )
    {
        stream << "(null)" << std::endl;
    }
    else
    {
        // print the hex header
        stream << "0x";

        // now print each hex character (two char per byte)
        for ( size_t i = 0; i < len; ++i )
        {
            stream << CHARS[ pBytes[i] >> 4    ]
                   << CHARS[ pBytes[i]  & 0x0F ];
        }
    }

    return stream;
}

std::ostream& printBinary( std::ostream& stream,
                           const uint8_t *ptr,
                           size_t len )
{
    // Bail out if the pointer is null
    if ( ptr == NULL )
    {
        stream << "(null";
        return stream;
    }
    else
    {
        // print the binary header
        stream << "0b";

        // print each bit of the data stream, allowing for a space every
        // 8 digits
        for ( size_t i = 0; i < len; ++i )
        {
            uint8_t v = ptr[i];

            stream <<  (v >> 7)
                << ((v >> 6) & 0x1)
                << ((v >> 5) & 0x1)
                << ((v >> 4) & 0x1)
                << ((v >> 3) & 0x1)
                << ((v >> 2) & 0x1)
                << ((v >> 1) & 0x1)
                << ((v     ) & 0x1);

            if ( i != len )
            {
                stream << " ";
            }
        }
    }

    return stream;
}

template<>
std::string ToString<bool>( const bool& object )
{
    if ( object )
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

} // end namespace
