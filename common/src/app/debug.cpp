#include <app/debug.h>
#include <app/windowsinclude.h>

#include <iostream>
#include <string>
#include <sstream>

using namespace Debug;

void Break()
{
#ifdef WINDOWS
    DebugBreak();
#else
    // ...
#endif
}

bool IsDebuggerPresent()
{
    // define the _WIN32_WINNT macro as 0x0400
#ifdef WINDOWS
    return IsDebuggerPresent();
#else
    // ...
#endif
}

void PrintString( const std::stringstream& stream )
{
    const std::string message = stream.str();
    PrintString( message.c_str() );
}

void PrintString( const std::string& message )
{
    PrintString( message.c_str() );
}

void PrintPrint( const char * message )
{
#ifdef WINDOWS
    OutputDebugStringA( message );
#else
    std::cerr << "<DEBUG> : " << message << std::endl;
#endif
}
