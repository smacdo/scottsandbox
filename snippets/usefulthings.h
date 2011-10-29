/**
 * This software is released as-is for general public use. It may be used
 * without permission for any reason whatsoever as long as my name
 * (attribution) is given in comment form near where the code snippet
 * itself is used, and my name is listed in documentation somewhere.
 * 
 * I claim no responsibility for damages that may occur to any person,
 * place, or thing through the direct or indirect use, or misuse of this
 * product.
 *
 * Basically, if you use a chunk of my code please do something like:
 *
 *  // thisfunction originally from Scott MacDonald
 *  //  <url>
 *  <your code snippet>
 *
 *
 * LICENSE DISCLAIMER:
 *  Here's how licensing works. All code is dual licensed - if use the
 *  whole file, or a substantial portion of it then you must reproduce
 *  the above BSD license. 
 *
 *  If you use a code snippet, one or several smaller utility methods,
 *  feel free to skip the license and attribute the work to me in comment
 *  form directly above the function. Additionally, you must give me some
 *  form of attribution and reproduce my copyright somewhere in your
 *  documentation. README is fine.
 *
 *  Make sense? Use one of my files wholesale (or near wholesale), keep
 *  the header. Copy paste parts of my work? Just the simple attribution
 *  is enough. Please don't make me come after you. 
 *
 *  If you're worried about this, then keep the license header intact and
 *  CONTACT me via e-mail. I promise to answer any questions - after all,
 *  I put this code up here to be useful for everyone. I just don't want
 *  to get ripped off unfairly :)
 * 
 */
template<typename C, typename T>
bool contains( const C& container, const T& val )
{
    return ::std::find( container.begin(), container.end(), val ) !=
           container.end();
}

template<typename T>
std::string toString( const T& input )
{
    std::ostringstream stream;
    stream << input;
    return stream.str();
}

inline std::string rtrim( const std::string& input )
{
    input.erase(
        std::find_if(
            input.rbegin(),
            input.rend(),
            std::not1( std::ptr_fun<int,int>( std::isspace ) )
        ).base(),
        s.end()
    );
}

inline std::string ltrim( const std::string& input )
{
    input.erase(
        input.begin(),
        std::find_if(
            input.begin(),
            input.end(),
            std::not1( std::ptr_fun<int,int>( std::isspace ) )
        ).base()
    );  
}

std::vector<std::string> split( const std::string& input, char delim )
{
    std::vector<std::string> output;
    std::stringstream        stream( input );
    std::string              item;

    while ( std::getline( stream, item, delim ) )
    {
        output.push_back( item );
    }

    return output;
}

template<typename T>
inline void clear( std::vector<T>& input )
{
    std::vector<T>().swap( input );
}

template<typename T>
inline void trim( std::vector<T>& input, size_t nsize )
{
    if ( nsize < input.size() )
    {
        std::vector<T>( input.begin(), input.begin() + nsize ).swap(input);
    }
    else
    {
        std::vector<T>( input ).swap( input );
    }
}

template<typename In, typename Out, typename Pred>
Out copy_if( In first, In last, Out res, Pred Pr )
{
    while ( first != last )
    {
        if ( Pr( *first ) )
        {
            *res++ = *first;
        }

        ++first;
    }

    return res;
}

template<typename T, std::size_t size>
inline T* begin( T (&array)[size] )
{
    return array;
}

template<typename T, std::size_t size>
inline T& end( T (&array)[size] )
{
    return array + size;
}

template<typename T>
inline void deletePointers( std::vector<T*>& input )
{
    for ( std::vector<T>::iterator itr  = input.begin();
                                   itr != input.end();
                                 ++itr )
    {
        delete *itr;
    }

    input.clear();
}

// does not work for text files
//  http://www.cplusplus.com/reference/clibrary/cstdio/ftell/
//     probably should use stat/fstat/GetFileSize
size_t filesize( FILE * file )
{
    if ( fseek( file, 0, SEEK_END ) != 0 )
    {
        // ERROR, probably shouldn't just silently return -1...
        return -1;
    }

    size_t sz = ftell( file );

    if ( sz < 0 )
    {
        return -1;  // same deal
    }
    
    if ( fseek( file, 0, SEEK_SET ) != 0 )
    {
        return -1;
    }

    return sz;
}

// Fun utility class that allows using strings, but auto-converts
// to c-style strings.
//
//  very useful for when you have function( char* ), and would like
//  to do function( MkStr() << "hello world " << 42 )
//
// This works because the temporary MkString class created must last
// until the lexically enclosing expression is fully evaluated. So it
// will not be destructed until after function( ) is called
class MkStr
{
public:
    template<typename T>
    MkStr& operator << ( const T& input )
    {
        buffer << input;
        return *this;
    }

    operator std::string() const
    {
        return buffer.str();
    }

    operator const char *() const
    {
        input = buffer.str();
        return input.c_str();
    }

private:
    std::ostringstream buffer;
    std::string input;
};

std::string loadfile( const std::string& filename )
{
    std::ifstream file( filename.c_str(), std::ios::ate );
    size_t size = file.tellg();
    file.seekg( 0, std::ios::beg );

    scoped_array<char> buffer( new char[size] );

    file.read( buffer.get(), size );

    return buffer.get();
}


#define SHOW(x) std::cerr << #X " = '" << (X) << "'" << std::endl;

#define DEPRECATED __attribute__((deprecated))
#define NOSTATE    __attribute__((__const__))
#define MUSTCHECK  __attribute__((warn_unused_result))
#define PACKED     __attribute__((packed))
#define ALIGNPAGESIZE __attribute__((packed,aligned(PAGE_SIZE)))

//#define likely(x)   __builtin_expect(!!(x), 1)
//#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x)     __builtin_expect((x),1)
#define unlikely(x)   __builtin_expect((x),0)

#define prefetch(x)   __builtin_prefetch((x))
#define prefetch_r(x) __builtin_prefetch((x), 0);
#define prefetch_w(x) __builtin_prefetch((x), 1);

// trigger gdb breakpoints
asm("int $0x3\n");
__builtin_trap
__debugbreak

/**
 * More usedful GCC builtins
 */
__builtin_clz( unsigned int );  // number of leading 0's
__builtin_ctz( ... );           // number of trailing 0's
__builtin_popcount( .. );       // number of 1s



const std::string LOWER_CASE_ALPHABET =



#ifndef SEQUENCE_LISTER_H
#define SEQUENCE_LISTER_H

#include <iostream>
#include <string>
#include <iterator>

template<typename X> class sequence_lister;     // Forward reference

template<typename InIter>
inline std::ostream& operator<<(std::ostream& os, sequence_lister<InIter> const& sl) {
//  copy(sl._first, sl._last, ostream_iterator<typename InIter::value_type>(os, sl._delim));
    for (InIter i = sl._first; i != sl._last; ++i) {
        if (i != sl._first) {
            os << sl._delim;
        }

        os << *i;
    }

    return os;
}

template<typename InIter>
class sequence_lister {
public:
    sequence_lister(InIter first, InIter last, char const* delim = "") :
        _first(first),
        _last(last),
        _delim(delim)
    {}

    // Also allow construction from any container supporting begin() and end()
    template<typename Cont>
    sequence_lister(Cont& cont, char const* delim = "") :
        _first(cont.begin()),
        _last(cont.end()),
        _delim(delim)
    {}

    sequence_lister(sequence_lister const& x) :
        _first(x._first),
        _last(x._last),
        _delim(x._delim)
    {}

    sequence_lister& operator=(sequence_lister const& x) {
        _first = x._first;
        _last = x._last;
        _delim = x._delim;
    }

    friend std::ostream& operator<< <>(std::ostream& os, sequence_lister<InIter> const& sl);

private:
    InIter _first, _last;
    char const* _delim;
};

template<typename InIter>
inline sequence_lister<InIter> list_sequence(InIter first, InIter last, char const* delim = "") {
    return sequence_lister<InIter>(first, last, delim);
}

template<typename Cont>
inline sequence_lister<typename Cont::const_iterator> list_sequence(Cont& cont, char const* delim = "") {
    return sequence_lister<typename Cont::const_iterator>(cont, delim);
}
#endif  // SEQUENCE_LISTER_H
