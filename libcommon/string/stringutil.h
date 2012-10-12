#ifndef SCOTT_COMMON_STRING_UTIL_H
#define SCOTT_COMMON_STRING_UTIL_H

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <iterator>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////

namespace StringUtil
{
    // Check if a string starts with another string
    bool startsWith( const std::string& word,
                     const std::string& prefix );

    // Check if a string ends with another string
    bool endsWith( const std::string& word,
                   const std::string& postfix );

    std::string ltrim( const std::string& word,
                       const std::string& chars = " \t\n" );
    std::string rtrim( const std::string& word,
                       const std::string& chars = " \t\n" );
    std::string trim( const std::string& word,
                      const std::string& chars = " \t\n" );

    // Convert string to upper case
    std::string toUpper( const std::string& input );
    
    // Append a numeric value to a string
    std::string makeSuffix( const std::string& base, long suffix );

    // Take an input string and replace each occurrence of findStr with
    // replaceStr.
    std::string replace( const std::string& input,
                         const std::string& findStr,
                         const std::string& replaceStr );

    // Output byte array as hexadecimal
    std::ostream& printHex( std::ostream& stream,
                            const uint8_t *ptr,
                            size_t len );

    template<typename T>
    std::ostream& printHex( std::ostream& stream, const T& obj )
    {
        const uint8_t * pBase = reinterpret_cast<const uint8_t*>(&obj);
        return printHex( stream, pBase, sizeof(obj) );
    }

    // Output byte array as binary
    std::ostream& printBinary( std::ostream& stream,
                               const uint8_t *ptr,
                               size_t len );

    template<typename T>
    std::ostream& printBinary( std::ostream& stream, const T& obj )
    {
        const uint8_t *pBase = reinterpret_cast<const uint8_t*>(&obj);
        return printBinary( stream, pBase, sizeof(obj) );
    }

    /**
     * Attempts to lexically convert the given object into a STL
     * string. This asssumes that the objec's type supports stream
     * out operation.
     *
     * Great for converting base types (such as ints, floats) into
     * strings
     *
     * \param  object   The object to cast to a string
     */
    template<typename T>
    std::string ToString( const T& object )
    {
        std::stringstream ss;
        ss << object;

        return ss.str();
    }

    template<>
    std::string ToString<bool>( const bool& object );

    /**
     * Attempt to lexically convert a string value into the requested
     * type. An object must implement the stream in ( >> ) operator for
     * this method to work.
     *
     * All builtins function fine with this method without any extra
     * code.
     *
     * \return Returns true if the conversion succeeded, false otherwise
     */
    template<typename T>
    inline T FromString( const std::string& input, bool *pStatus )
    {
        std::stringstream ss;
        T output;

        // Perform the lexical conversion
        ss << input;
        ss >> output;

        // Get the return status of the lexical conversion
        if ( pStatus != NULL )
        {
            *pStatus = (! ss.fail() );
        }

        return output;
    }
}

/////////////////////////////////////////////////////////////////////////
// MakeString class                                                    //
/////////////////////////////////////////////////////////////////////////

/**
 * Utility class that simplifies the process of generating and formatting
 * anonymous strings. You would typically use this class when generating
 * a string to pass into a method.
 *
 * \code
 * int blah = 42;
 * f( MakeString() << "This is " << 42 "!" );
 * \endcode
 */
class MakeString
{
public:
    template<typename T>
    MakeString& operator << ( const T& data )
    {
        mBuffer << data;
        return *this;
    }

    operator std::string() const
    {
        return mBuffer.str();
    }

private:
    std::ostringstream mBuffer;
};

/////////////////////////////////////////////////////////////////////////
// Sequence lister                                                     //
/////////////////////////////////////////////////////////////////////////
template<typename T> class SequenceLister;

/// Forward function used for SequenceLister
template<typename Iter>
inline std::ostream& operator << ( std::ostream& os,
                                   const SequenceLister<Iter>& sl )
{
    for ( Iter itr = sl.first; itr != sl.last; ++itr )
    {
        if ( itr != sl.first )
        {
            os << sl.delim;
        }

        os << *itr;
    }

    return os;
}

/**
 * Utility to pretty-print containers
 *
 * \code
 * std::cout << print_sequence( container, ", " ) << std::endl
 * \endcode
 */
/*** BROKEN
template<typename Itr>
class SequenceLister
{
public:
    SequenceLister( Itr first_, Itr last_, const char * delim_ = ", " )
        : mFirst( first_ ),
          mLast( last_ ),
          mDelim( delim_ )
    {
    }

    // Constructor supporting generic containers that implement begin()
    // and end()
    template<typename Container>
    SequenceLister( Container& container, const char * delim_ = ", " )
        : mFirst( container.begin() ),
          mLast( container.end() ),
          mDelim( delim_ )
    {
    }

    friend std::ostream& operator << <>( std::ostream& os,
                                         const SequenceLister<Itr>& sl );

private:
    // Copy construct disabled
    SequenceLister( const SequenceLister& sl );

    // Assignment disabled
    SequenceLister& operator = ( const SequenceLister& sl );

private:
    Itr mFirst;
    Itr mLast;
    const char * mDelim;
};

template<typename Itr>
inline SequenceLister<Itr> printSequence( Itr first,
                                          Itr second,
                                          const char * delim = ", " )
{
    return SequenceLister<Itr>( first, second, delim );
}

template<typename Container>
inline SequenceLister<typename Container:const_iterator:> printSequence(
        const Container& container,
        const char * delim = ", " )
{
    return SequenceLister<typename Container::const_iterator>(
            container,
            delim
    );
}

**/

/////////////////////////////////////////////////////////////////////////
// TransferString                                                      //
/////////////////////////////////////////////////////////////////////////
/**
 * Utilty class that assists when calling legacy functions
 * (Especially with windows api). It allows you to declare a normal
 * STL string, and then use this class to automate passing a fixed size
 * buffer into a method and upon completion copies it to your STL string.
 *
 * An example is worth a thousand words, so here it is:
 * *****
 * std::string str;
 * GetApplicationName( TransferString(str, MAX_SIZE), MAX_SIZE );
 * *****
 *
 * TransferString passes a writeable cstring buffer to the function, and
 * then upon destruction it copies the value into str.
 *
 * Inspiration from:
 * http://stackoverflow.com/questions/469696/what-is-your-most-useful-c-c-utility/473786#473786
 * 
 */
template<class T>
class TransferStringImpl
{
    typename std::basic_string<T>& mStringOut;
    typename std::vector<T>        mInputBuffer;

public:
    /**
     * Create a new string buffer
     *
     * \param  str   STL string to copy output to when finished
     * \param  size  Maximum buffer size to allocate
     */
    TransferStringImpl( std::basic_string<T>& str, size_t size )
        : mStringOut( str ),
          mInputBuffer( size + 1 )
    {
        // Set last element of buffer to be zero (null terminated)
        bufferPtr()[size] = static_cast<T>( 0 );
    }

    /**
     * Destructor. Calls commit which will copy the inputted c-string into
     * the output STL string
     */
    ~TransferStringImpl()
    {
        commit();
    }

    /**
     * Get a pointer to the internal c-string buffer
     *
     * \return  Pointer to the internal c-string buffer
     */
    T *bufferPtr()
    {
        return &mInputBuffer[0];
    }

    /**
     * Implicit conversion operator. Allows this instance to be implicitly
     * converted in a c-string.
     */
    operator T *()
    {
        return bufferPtr();
    }

    /**
     * Commits the temporary c-string buffer into a user specified STL string
     * for preservation. This is where the magic happens - When this method
     * is called, the input buffer (a c-string) is copied into a STL string
     * that the user specified. Ownership of the string contents is
     * transferred, and the original c-string buffer is destroyed.
     */
    void commit()
    {
        // Refuse to do anything if buffer size is zero
        if ( mInputBuffer.size() == 0 )
        {
            return;
        }

        // Get the length of the cstring, and copy it to provided STL string
        size_t len = std::char_traits<T>::length( bufferPtr() );
        mStringOut.assign( bufferPtr(), len );

        // Not sure why the author is trying to resize the buffer to be
        // zero. Will keep this here until I can understand what he was
        // trying to accomplish (if anything)
        abort();
    }

    // Not at all sure why the author had an abort method included. I'll
    // include it verbatim just in case there's justification for it
    // down the road
    void abort()
    {
        mInputBuffer.resize( 0 );
    }
};

template<typename T>
inline TransferStringImpl<T> TransferString( typename std::basic_string<T>& str,
                                             size_t size )
{
    return TransferStringImpl<T>( str, size );
}

#endif
