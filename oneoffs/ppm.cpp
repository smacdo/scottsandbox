/**
 * Reads a PPM file
 */
#include <iostream>
#include <string>
#include <ostream>
#include <fstream>

#include <sstream>

#include <cassert>

#include <stdint.h>

struct PPM_Image            // 24 bit image, RGB
{
    size_t cols;
    size_t rows;
    uint8_t pixelbuff;      // size := cols * rows * 3
};

const char * PPM_FileData =
    "P6\n"           \
    "# sample.ppm\n" \
    "4 4\n"          \
    "15\n"           \
    "0 0 0   0 0   0 0 0 0   15 0 15\n" \
    " 0  0  0    0 15  7    0  0  0    0  0  0\n"  \
    " 0  0  0    0  0  0    0 15  7    0  0  0\n"  \
    " 0  0  0    0  0  0    0  0  0   12  0  5\n"  \
    "15  0 15    2 14  0    9  9  9    1  1  1\n";

enum PPM_ParserState
{
    PPM_HEADER,
    PPM_DIMS,
    PPM_DEPTH,
    PPM_PIXELS,
    PPM_INVALID
};

bool loadImage( std::istream& input, PPM_Image& output );
bool isCommentLine( const std::string& input );
void raisePPMError( const std::string& input );

bool loadImage( std::istream& input, PPM_Image& output )
{
    PPM_ParserState state = PPM_HEADER;
    assert( input.good() );

//    size_t rows = 0, cols = 0, maxValue = 0, r = 0, g = 0, b = 0;

    //
    // read each line
    //
    std::string line;

    while ( input.eof() == false && input.good() )
    {
        // Get the next line
        std::getline( input, line );
        
        // Skip any line that starts with a comment
        if ( isCommentLine( line ) )
        {
            continue;
        }

        //
        // Parse the line
        //
        switch ( state )
        {
            case PPM_HEADER:
                // read dimensions
                if ( line != "P6" )
                {
                    raisePPMError("Invalid file - expected 'P6' header");
                    state = PPM_INVALID;
                }

                // move to next state
                state = PPM_DIMS;
                break;

            case PPM_DIMS:
                break;

            case PPM_DEPTH:
                break;

            case PPM_PIXELS:
                break;

            case PPM_INVALID:
                return false;

            default:
                assert( false );
        }


        std::cout << line << std::endl;
    }

    output.rows = output.rows;
    return false;
}

bool isCommentLine( const std::string& input )
{
    size_t len = input.size();
    bool ret   = true;

    for ( size_t i = 0; i < len; ++i )
    {
        char c = input[i];

        if ( c == ' ' || c == '\t' || c == '\r' || c == '\n' )
        {
            continue;
        }
        else if ( c == '#' )
        {
            ret = true;
            break;
        }
        else
        {
            ret = false;
            break;
        }
    }

    return ret;
}

void raisePPMError( const std::string& input )
{
    std::cerr << "ERROR: " << input << std::endl;
    assert( false );
}

int main( int argc, char* argv[] )
{
    std::string filename = "default.ppm";

    if ( argc > 1 )
    {
        filename = argv[1];
    }

    //
    // open file
    //
    PPM_Image     image;
    std::ifstream file( filename.c_str(), std::ifstream::in );

    if (! file.is_open() )
    {
        std::cerr << "Failed to open file " << filename << std::endl;
//        return 1;
    }

    std::stringstream ss( PPM_FileData );

    if (! loadImage( ss, image ) )
    {
        std::cerr << "Failed to load file " << filename << std::endl;
        return 1;
    }

    return 0;
}
