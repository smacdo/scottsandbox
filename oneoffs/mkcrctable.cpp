/**
 * Simple C++ application that computes a 256 CRC32 polynomial lookup
 * table, and formats the output so that it is immediately usable in a C++
 * program.
 *
 * For example on how to use this, please look at the crc32 code. It is
 * currently located in workbench, though it may get moved to shared.
 */
#include <stdio.h>
#include <stdint.h>

const int CRC_TABLE_SIZE = 256;

void dump_table( uint32_t polynomial )
{
    const int ELEMENT_SIZE = 12;
    const int PER_ROW = 80 / ELEMENT_SIZE;

    for ( int i = 0; i < CRC_TABLE_SIZE; ++i )
    {
        uint32_t crc = i;

        for ( int j = 0; j < 8; ++j )
        {
            if ( crc & 1 )
            {
                crc = (crc >> 1) ^ polynomial;
            }
            else
            {
                crc >>= 1;
            }
        }

        printf(" %#010x", crc);

        // end of line?
        if ( i != CRC_TABLE_SIZE - 1 )
        {
            printf(",");
        }

        if ( ((i+1) % PER_ROW == 0) || (i == CRC_TABLE_SIZE - 1) )
        {
            printf("\n");
        }
    }
}


int main( int , char** )
{
    dump_table( 0xEDB88320 );
}
