#ifndef SCOTT_STRING_CASTS
#define SCOTT_STRING_CASTS

#include <string>
#include <sstream>

namespace String
{
    template<typename T>
    inline std::string ToString( const T& object )
    {
        std::stringstream ss;
        ss << object;

        if ( ss.bad() ) // is this correct?
        {
            raiseError("Cast Failed")
                << "Failed to cast type to string: '"
                << object
                << "'";

            return "<cast failed>";
        }
        else
        {
            return ss.str();
        }
    }

    template<typename T>
    inline bool FromStringCast( const std::string& input, const T& output )
    {
        std::stringstream ss;
        ss << input;
        ss >> output;

        if ( ss.bad() )
        {
            raiseError("Cast Failed")
                << "Failed to cast from string to type: '"
                << input
                << "'";

            return false;
        }
        else
        {
            return true;
        }
    }

    template<typename ToT, typename FromT>
    inline ToT LexicalCast( const FromT& input )
    {
        ToT out;
        std::stringstream ss;

        ss << input;
        ss >> out;

        return out;
    }
}

#endif
