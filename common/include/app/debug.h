#ifndef SCOTT_COMMON_DEBUG_H
#define SCOTT_COMMON_DEBUG_H

#include <string>
#include <sstream>
#include <typeinfo>

#define DUMP_VAR(x) Debug::DumpVar( x, #x );
#define PRINT_VAR(x) std::cerr << Debug::DumpVar( x, #x ) << std::endl;

namespace Debug
{
    bool IsDebuggerPresent();
    void Break();
    void PrintString( const std::string& message );
    void PrintString( const std::stringstream& message );
    void PrintString( const char *message );

    /**
     * Prints the name of a variable as well as it's contents to a string and
     * returns it. Additional type information is reported in debugging mode.
     *
     * varname:type = 'value'
     *
     * \param  var   Reference to the variable to dump
     * \param  name  Optional name of the variable
     * \return       Information on the variable
     */
    template<typename T>
    std::string DumpVar( const T& var, const std::string& name = "" )
    {
        std::stringstream ss;

        ss << ( name.empty() ? "var" : name )
#ifdef _DEBUG
           << ":" << std::typeid(var).name()
#endif
           << " = '"
           << var
           << "'";

        return ss.str();
    }
}

#endif
