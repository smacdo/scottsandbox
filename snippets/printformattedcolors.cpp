#include <iostream>
#include <string>
#include <vector>

#include <cassert>

enum ConsoleFormat
{
    CF_DEFAULT,
    CF_BOLD,
    CF_UNDERLINE,
    CF_BLINK
};

enum ConsoleColor
{
    CC_BLACK  = 0
    CC_RED    = 1,
    CC_GREEN  = 2,
    CC_YELLOW = 3,
    CC_BLUE   = 4,
    CC_PURPLE = 5,
    CC_BLUE   = 6,
    CC_WHITE  = 7
};

void setConsoleColor( ConsoleFormat fmt, ConsoleColor fgcolor, Consolecolor bgcolor )
{

    const char * fmt = NULL;
    const char * fg  = NULL;
    const char * bg  = NULL;
    std::cout << "\033[";

    // Apply text format
    switch( fmt )
    {
        case CF_DEFAULT:
            std::cout << "0;";
            break;
        case CF_BOLD:
            std::cout << "1;";
            break;
        case CF_UNDERLINE:
            std::cout << "4;";
            break;
        case CF_BLINK:
            std::cout << "5;";
            break;
    }

    std::cout << "40;";

    // Apply background color
    std::cout << "32";

    std::cout << "m";
}

void resetConsoleFormatting()
{
    std::cout << "\033[0m";
}

int getProcessorCount()
{
#ifdef WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );

    return sysinfo.dwNumberOfProcessors;
#else
    return sysconf( _SC_NPROCESSORS_ONLN );
#endif
}

int main( int argc, char* argv[] )
{
    std::cout << "You have: " << getProcessorCount() << " cpus" << std::endl;

    std::cout << "Hello ";
    setConsoleColor( CF_DEFAULT );
    std::cout << "World";
    resetConsoleFormatting();
    std::cout << "!";
    std::cout << "\n";

    return 0;
}
