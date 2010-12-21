#include <string/util.h>
#include <string>

#include <cassert>
#include <stdint.h>

void printHex( std::ostream& stream, const uint8_t *ptr, size_t len )
{
    assert( ptr != NULL && "Data ptr cannot be null" );
    const char * CHARS = "0123456789ABCDEF";

    // print the hex header
    stream << "0x";

    // now print each hex character (two char per byte)
    for ( size_t i = 0; i < len; ++i )
    {
        stream << CHARS[ ptr[i] >> 4    ]
               << CHARS[ ptr[i]  & 0x0F ];
    }
}

void printBinary( std::ostream& stream, const uint8_t *ptr, size_t len )
{
    assert( ptr != NULL && "Data ptr cannot be null" );

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
               << ((v     ) & 0x1)
               << " ";      // bug!
    }
}
