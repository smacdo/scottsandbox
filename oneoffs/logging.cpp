/**
 * Experiments into writing a more flexible and easy to use loggging system
 * in C++. Requirements for the log system (in descending order of priority)
 *
 *   1. It "just works" (Drop it in, and start using it)
 *   2. Painless logging (Emitting log data should be as easy as std::cerr)
 *   3. Multiple output streams (Console, file output)
 *   4. Speed
 *   5. Flexibility
 */
#ifndef SCOTT_LOGGING_H
#define SCOTT_LOGGING_H

#include <streambuf>
#include <ostream>
#include <string>

/**
 * Log level indicates what the severity level is of the log entry that is
 * being written out
 */
enum ELogLevel
{
    ELOGLEVEL_TRACE,
    ELOGLEVEL_DEBUG,
    ELOGLEVEL_INFO,
    ELOGLEVEL_NOTICE,
    ELOGLEVEL_WARN,
    ELOGLEVEL_ERROR,
    ELOGLEVEL_FATAL
};

/**
 * A custom debugging stream buffer implementation. This forms the core of
 * the new logging system
 */
template<typename Char, typename Traits = std::char_traits<char> >
class DebugStreambuf : public std::basic_streambuf<Char, Traits>
{
public:
    typedef typename std::basic_streambuf<Char, Traits>::traits_type traits_type;
    typedef typename std::basic_streambuf<Char, Traits>::int_type int_type;

public:
    DebugStreambuf();
    DebugStreambuf( std::basic_streambuf<Char, Traits> *pConsoleBuffer,
                    std::basic_filebuf<Char,Traits> *pFileBuffer );
    virtual ~DebugStreambuf();

    void write( const std::basic_string<Char,Traits>& contents );
    void setConsoleThreshold( ELogLevel level );
    void setFileThreshold( ELogLevel level );
    void setLogLevel( ELogLevel level );
    void setModule( const char* moduleName );

    void setConsole( std::basic_streambuf<Char, Traits> *pConsoleBuffer );
    void setFile( std::basic_filebuf<Char,Traits> *pFileBuffer );

    // make these protected later on
    void endLogEntry();

protected:
    std::streamsize xsputn( const Char* pSequence, std::streamsize size );

    // Returns a textual version of the log level
    const char* getLogLevelString( ELogLevel level ) const;

    bool writeEntryHeaderConsole() const;
    bool writeEntryHeaderFile() const;

private:
    // Disable the copy constructor
    DebugStreambuf( const DebugStreambuf& );

    // Disable the assignment operator
    DebugStreambuf& operator = ( const DebugStreambuf& );

private:
    std::basic_streambuf<Char,Traits> *mpConsoleBuffer;
    std::basic_filebuf<Char,Traits> *mpFileBuffer;

    // The default severity of a log entry
    const ELogLevel mDefaultLogLevel;

    // The default module name for a log entry
    const char * mDefaultModuleName;

    // The minimum threshold that must be met for log entry to written to
    // the user console
    ELogLevel mConsoleThreshold;

    // The minimum threshold that must be met for a log entry to be written
    // to the file log
    ELogLevel mFileThreshold;

    // Flag if we are writing the start of a new log entry
    bool mAtLineStart;

    // The severity level of the current log entry
    ELogLevel mLogLevel;

    // Module name for the current log entry
    const char * mModuleName;
};

class LogStream : public std::ostream
{
public:
    LogStream( std::ostream *pConsoleStream, std::ofstream* pFileStream );
    ~LogStream();

    // probably make these private
    void endLogEntry();

private:
    DebugStreambuf<char>* mpStreambuf;
    std::ostream *mpConsoleStream;
    std::ofstream *mpFileStream;
};

#endif

/////////////////////////////////////////////////////////////////////////
// Implmentation
/////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <ctime>
#include <cassert>

#include <ostream>
#include <fstream>

/**
 * The debug stream buffer constructor. Takes a pointer to a stream buffer
 * that acts as a backing store.
 */
template<typename C, typename T>
DebugStreambuf<C,T>::DebugStreambuf()
    : mpConsoleBuffer( NULL ),
      mpFileBuffer( NULL ),
      mDefaultLogLevel( ELOGLEVEL_INFO ),
      mDefaultModuleName( "App" ),
      mConsoleThreshold( ELOGLEVEL_INFO ),
      mFileThreshold( ELOGLEVEL_DEBUG ),
      mAtLineStart( true ),
      mLogLevel( mDefaultLogLevel ),
      mModuleName( mDefaultModuleName )
{
}

/**
 * The debug stream buffer constructor. Takes a pointer to a stream buffer
 * that acts as a backing store.
 */
template<typename C, typename T>
DebugStreambuf<C,T>::DebugStreambuf( std::basic_streambuf<C,T>* pConsoleBuffer,
                                     std::basic_filebuf<C,T>* pFileBuffer )
    : mpConsoleBuffer( pConsoleBuffer ),
      mpFileBuffer( pFileBuffer ),
      mDefaultLogLevel( ELOGLEVEL_INFO ),
      mDefaultModuleName( "App" ),
      mConsoleThreshold( ELOGLEVEL_INFO ),
      mFileThreshold( ELOGLEVEL_DEBUG ),
      mAtLineStart( true ),
      mLogLevel( mDefaultLogLevel ),
      mModuleName( mDefaultModuleName )
{
}

/**
 * Destructor
 */
template<typename C, typename T>
DebugStreambuf<C,T>::~DebugStreambuf()
{
}

/**
 * Set the minimum severity level required before a log entry gets written
 * to the user's console
 */
template<typename C, typename T>
void DebugStreambuf<C,T>::setConsoleThreshold( ELogLevel logLevel )
{
    mConsoleThreshold = logLevel;
}

/**
 * Set the minimum severity level required before a log entry gets written
 * to the log file
 */
template<typename C, typename T>
void DebugStreambuf<C,T>::setFileThreshold( ELogLevel logLevel )
{
    mFileThreshold = logLevel;
}

/**
 * Set the debug stream's console output to be a pointer to the given stream
 * buffer. Passing in NULL will disable the console output.
 *
 * \param  pConsoleBuffer  Pointer to a basic_streambuf
 */
template<typename C, typename T>
void DebugStreambuf<C,T>::setConsole( std::basic_streambuf<C,T> *pConsoleBuffer )
{
    mpConsoleBuffer = pConsoleBuffer;
}

/**
 * Sets the debug stream's file output to be a pointer to the given stream
 * buffer. Passing in NULL will disable the debug log's file output.
 *
 * \param  pFileBuffer  Pointer to a basic_filebuf
 */
template<typename C, typename T>
void DebugStreambuf<C,T>::setFile( std::basic_filebuf<C,T> *pFileBuffer )
{
    mpFileBuffer = pFileBuffer;
}

/**
 * Overriden stream handling method. xsputn is called by the underlying
 * basic_streambuf class whenever the streambuf receives input data.
 *
 * The method takes all of character output, appends any neccesary log entry
 * text and then routes everything to our console and file stream_bufs.
 *
 * \param  pSequence  The character sequence to write
 * \param  size       The size of the character sequence
 * \return            Number of characters written. Zero if there was a problem
 */
template<typename C, typename T>
std::streamsize DebugStreambuf<C,T>::xsputn( const C* pSequence, std::streamsize size )
{
    bool didWrite = true;
    bool isAtLineStart = mAtLineStart;

    //
    // Write log data to the user's console
    //
    if ( mpConsoleBuffer != NULL && mLogLevel >= mConsoleThreshold )
    {
        // Write out the log header to the console
        if ( mAtLineStart )
        {
            writeEntryHeaderConsole();
            isAtLineStart = false;
        }

        // Now write the actual string to the console
        mpConsoleBuffer->sputn( pSequence, size );
    }

    //
    // Now write the log contents to the file log
    //
    if ( mpFileBuffer != NULL && mLogLevel >= mFileThreshold )
    {
        // Write out the log header to the console
        if ( mAtLineStart )
        {
            writeEntryHeaderFile();
            isAtLineStart = false;
        }

        // Now write the actual string to the console
        mpFileBuffer->sputn( pSequence, size );
    }

    // Make sure we if we wrote a log entry that we don't do so again until
    // the start of a new log entry
    mAtLineStart = isAtLineStart;
    return (didWrite ? size : 0u );
}

/**
 * Writes a string directly to the debug buffer
 */
template<typename C, typename T>
void DebugStreambuf<C,T>::write( const std::basic_string<C,T>& str )
{
    sputn( str.c_str(), str.size() );
}

/**
 * Writes the log entry's "header" text out to the console. Thsi text
 * consists of the current date/time, entry module and severity level.
 *
 * \return  True if the header was written to the console, false otherwise
 */
template<typename C, typename T>
bool DebugStreambuf<C,T>::writeEntryHeaderConsole() const
{
    assert( mpConsoleBuffer != NULL );

    // Get the string form of the log level as well as the current
    // time before writing the log entry header
    size_t TIME_STR_LEN = 10;
    char timeString[TIME_STR_LEN];

    time_t currentTime = std::time( NULL );

    std::strftime( timeString, TIME_STR_LEN,
                   "%H:%M:%S",
                   localtime(&currentTime) );

    // Generate the log entry header before we hand it over to our
    // underlying buffer object
    size_t BUFFER_LEN = 32;
    char bufferString[BUFFER_LEN];

    int length = snprintf( bufferString, BUFFER_LEN,
                           "%8s [%c %-12s] ",
                           timeString,
                           getLogLevelString( mLogLevel )[0],
                           mModuleName );

    // Now write the header text out to the underlying buffer
    mpConsoleBuffer->sputn( bufferString, length );
    return true;
}

/**
 * Writes the log entry's "header" text out to the file log. This text
 * consists of the current date/time, entry module and severity level.
 *
 * \return  True if the header was written to the file, false otherwise
 */
template<typename C, typename T>
bool DebugStreambuf<C,T>::writeEntryHeaderFile() const
{
    assert( mpFileBuffer != NULL );

    // Get the string form of the log level as well as the current
    // time before writing the log entry header
    size_t TIME_STR_LEN = 20;
    char timeString[TIME_STR_LEN];

    time_t currentTime = std::time( NULL );

    std::strftime( timeString, TIME_STR_LEN,
                   "%Y.%m.%d-%H:%M:%S",
                   localtime(&currentTime) );

    // Generate the log entry header before we hand it over to our
    // underlying buffer object
    size_t BUFFER_LEN = 48;
    char bufferString[BUFFER_LEN];

    int length = snprintf( bufferString, BUFFER_LEN,
                           "%s %s %s ",
                           timeString,
                           getLogLevelString( mLogLevel ),
                           mModuleName );

    // Now write the header text out to the underlying buffer
    mpFileBuffer->sputn( bufferString, length );
    return true;
}

/**
 * Returns the string name of a given severity level
 */
template<typename C, typename T>
const char* DebugStreambuf<C,T>::getLogLevelString( ELogLevel level ) const
{
    switch ( level )
    {
        case ELOGLEVEL_TRACE:
            return "TRACE";
        case ELOGLEVEL_DEBUG:
            return "DBG";
        case ELOGLEVEL_INFO:
            return "INFO";
        case ELOGLEVEL_NOTICE:
            return "NOTE";
        case ELOGLEVEL_WARN:
            return "WARN";
        case ELOGLEVEL_ERROR:
            return "ERROR";
        case ELOGLEVEL_FATAL:
            return "FATAL";
        default:
            return "???";
    }
}

/**
 * Called when the logger has finished emitting a single log entry. This
 * method then resets our internal state in preparation for the next log
 * entry
 */
template<typename C,typename T>
void DebugStreambuf<C,T>::endLogEntry()
{
    // Reset current log entry state
    mAtLineStart = true;
    mLogLevel    = mDefaultLogLevel;
    mModuleName  = mDefaultModuleName;

    // Sync output
    if ( mpConsoleBuffer != NULL )
    {
        mpConsoleBuffer->sputc( '\n' );
        mpConsoleBuffer->pubsync();
    }

    if ( mpFileBuffer != NULL )
    {
        mpFileBuffer->sputc( '\n' );
        mpFileBuffer->pubsync();
    }
}



#include <iostream>

LogStream::LogStream( std::ostream *pConsoleStream,
                      std::ofstream *pFileStream )
    : std::ostream( new DebugStreambuf<char> ),
      std::ios( 0 ),
      mpStreambuf( NULL ),
      mpConsoleStream( pConsoleStream ),
      mpFileStream( pFileStream )
{
    // i really don't like having to do this
    mpStreambuf = dynamic_cast<DebugStreambuf<char>* >( rdbuf() );
    assert( mpStreambuf != NULL && "Something really evil just happened" );

    // Assign the output streams to our internal debug stream
    if ( pConsoleStream != NULL )
    {
        mpStreambuf->setConsole( pConsoleStream->rdbuf() );
    }

    if ( pFileStream != NULL )
    {
        mpStreambuf->setFile( pFileStream->rdbuf() );
    }
}

LogStream::~LogStream()
{
    // Destroy the log stream's internal stream buffer
    delete mpStreambuf;

    // Now make sure all of our streams have been closed properly
    if ( mpConsoleStream != NULL )
    {
        (*mpConsoleStream) << std::endl;
    }

    if ( mpFileStream != NULL )
    {
        mpFileStream->close();
    }
}

void LogStream::endLogEntry()
{
    mpStreambuf->endLogEntry();   
}

/////////////////////////////////////////////////////////////////////////
// Sample code
/////////////////////////////////////////////////////////////////////////
#include <iostream>

int main( int argc, char* argv[] )
{
    // Create a new log file for use
    std::string logname = "logfile.txt";
    std::ofstream logfile( logname.c_str(), std::ios::out );

    logfile << "# Version : 1.0"                               << std::endl
            << "# Fields  : date time severity module data..." << std::endl
            << "# Software: $APPLICATION$"                     << std::endl
            << "# Date    : $DATE$ $TIME$"                     << std::endl
            << "# Remark  : Application log dump"              << std::endl;
    LogStream s( &std::cerr, &logfile );

    s << "oh hello there" << "what am i\n saying";
    s.endLogEntry();

    s << "this is a second log";
    s.endLogEntry();

    s << 42;
    s.endLogEntry();
}
