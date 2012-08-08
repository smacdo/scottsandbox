#include <iostream>
#include <iomanip>

void generate_lut();

const unsigned int LOOKUP_SIZE = 128;
const char ENCRYPTION_TABLE[LOOKUP_SIZE] =
{
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
    16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
    32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
    48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  65,  66,
    67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  91,  92,  93,  94,  95,
    96, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  97,  98,
    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 123, 124, 125, 126, 127
};

int main( int argc, char* argv[] )
{
    int c;

    for ( ; c = fgetc(stdin); c != EOF )
    {
        fputc( stdin, c );
    }

    std::cout << std::endl;
}

void generate_lut()
{
    for ( size_t j = 0; j < 128; ++j )
    {
        size_t i = j;

        if ( i >= 'A' && i <= 'M' )
        {
            i += 13;
        }
        else if ( i > 'M' && i <= 'Z' )
        {
            i -= 13;
        }
        else if ( i >= 'a' && i <= 'm' )
        {
            i += 13;
        }
        else if ( i > 'm' && i <= 'z' )
        {
            i -= 13;
        }

        std::cout << std::setw(3) << i << ",";

        if ( (j+1) % 16  == 0 )
        {
            std::cout << std::endl;
        }
        else
        {
            std::cout << " ";
        }
    }

    std::cout << std::endl;
}
