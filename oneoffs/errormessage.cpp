#include <iostream>
#include <string>
#include <sstream>

#include <boost/utility.hpp>

class ErrorStreamObj : boost::noncopyable
{
public:
    ErrorStreamObj();
    ErrorStreamObj( const char* filename, unsigned int lineNumber );
    ErrorStreamObj( const ErrorStreamObj&& obj );
    ~ErrorStreamObj();

    template<typename T>
    std::ostream& operator << ( const T& obj )
    {
        mErrorStream << obj;
        return mErrorStream;
    }

private:
    std::ostringstream mErrorStream;
    const char * mFilename;
    unsigned int mLineNumber;
};

///////////////////////////////////////////////////////////////////////////
ErrorStreamObj::ErrorStreamObj()
    : mErrorStream(),
      mFilename( NULL ),
      mLineNumber( 0 )
{
}

ErrorStreamObj::ErrorStreamObj( const char* filename, unsigned int line )
    : mErrorStream(),
      mFilename( filename ),
      mLineNumber( line )
{

}

ErrorStreamObj::ErrorStreamObj( const ErrorStreamObj&& obj )
    : mErrorStream(),
      mFilename( obj.mFilename ),
      mLineNumber( obj.mLineNumber )
{
}

ErrorStreamObj::~ErrorStreamObj()
{
    std::cerr << "ERROR: " << mErrorStream.str() << std::endl;
}

ErrorStreamObj reportError()
{
    return ErrorStreamObj();
}

int main( int argc, char* argv[] )
{
    reportError() << "This is a huge mistake " << 42;
}
