#ifndef SCOTT_COMMON_DEBUG_H
#define SCOTT_COMMON_DEBUG_H

#include <string>
#include <sstream>
#include <typeinfo>

namespace Debug
{
    bool IsDebuggerPresent();
    void Break();
    void PrintString( const std::string& message );
    void PrintString( const std::stringstream& message );
    void PrintString( const char *message );

    template<typename T>
    void PrintVar( const T& var, const std::string& name = "*" )
    {
        std::stringstream ss;
        ss << "<" << name << ";" 
           << std::typeid(var).name() << "> = "
           << var;

        PrintString( ss );
    }
}

#endif
