#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define TRACE(x) LogEntry(LOG_TRACE,(x),__FILE__,__func__,__LINE__).stream()
#define DEBUG(x) LogEntry(LOG_DEBUG,(x),__FILE__,__func__,__LINE__).stream()
#define INFO(x) LogEntry(LOG_INFO,(x),__FILE__,__func__,__LINE__).stream()
#define NOTICE(x) LogEntry(LOG_NOTICE,(x),__FILE__,__func__,__LINE__).stream()
#define WARN(x) LogEntry(LOG_WARNING,(x),__FILE__,__func__,__LINE__).stream()
#define ERROR(x) LogEntry(LOG_ERROR,(x),__FILE__,__func__,__LINE__).stream()
#define FATAL(x) LogEntry(LOG_FATAL,(x),__FILE__,__func__,__LINE__).stream()

enum LogLevel
{
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_NOTICE,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
};

// A null stream
struct LoggerNullStream : std::ostream
{
    struct nullbuf : std::streambuf
    {
        int overflow( int c )
        {
            return traits_type::not_eof(c);
        }
    } m_sbuf;

    LoggerNullStream()
        : std::ios( &m_sbuf ),
          std::ostream( &m_sbuf )
    {
    }
};

struct LogStreams
{
    LogStreams( std::ostream& css, std::ostream& fss )
        : consolestream( css ), filestream( fss )
    {
    }

    std::ostream& consolestream;
    std::ostream& filestream;
};

class LogManager
{
public:
    LogManager()
        : m_bConsole( true ),
          m_bFile( false ),
          m_file(),
          m_null(),
          m_streams( m_null, m_null ),
          m_outputpath(),
          m_filename()
    {
        updateStreams();
    }

    ~LogManager()
    {
        // Close the file stream if it is still open
        if ( m_file.is_open() )
        {
            m_file.close();
        }
    }

    static LogManager& instance()
    {
        static LogManager lm;
        return lm;
    }

    static void setOuputDirectory( const std::string& path )
    {
        // Set the file name
        instance().m_filename = path;
    }

    static std::ostream& getConsoleStream()
    {
        return instance().getConsoleStreamInner();
    }

    static std::ostream& getFileStream()
    {
        return instance().getFileStreamInner();
    }

private:
    void updateStreams()
    {
        if ( m_bConsole )
        {
            if ( m_bFile )
            {
                m_streams = LogStreams( std::cout, m_file );
            }
            else
            {
                m_streams = LogStreams( std::cout, m_null );
            }
        }
        else
        {
            if ( m_bFile )
            {
                m_streams = LogStreams( m_null, m_file );
            }
            else
            {
                m_streams = LogStreams( m_null, m_null );
            }
        }
    }

    std::ostream& getConsoleStreamInner()
    {
        return m_streams.consolestream;
    }

    std::ostream& getFileStreamInner()
    {
        return m_streams.filestream;
    }

private:
    bool m_bConsole;
    bool m_bFile;
    std::ofstream m_file;
    LoggerNullStream m_null;
    LogStreams m_streams;
    std::string m_outputpath;
    std::string m_filename;
};

class LogEntry
{
public:
    LogEntry( LogLevel level )
        : m_level( level ),
          m_location( "app" ),
          m_file( NULL ),
          m_func( NULL ),
          m_line( -1 )
    {
    }

    LogEntry( LogLevel level, const char* location )
        : m_level( level ),
          m_location( location ),
          m_file( NULL ),
          m_func( NULL ),
          m_line( -1   )
    {
    }

    LogEntry( LogLevel level,
              const char * file,
              const char * func,
                    int    line )
        : m_level( level ),
          m_location( "app" ),
          m_file( file ),
          m_func( func ),
          m_line( line )
    {
    }

    LogEntry( LogLevel level,
              const char * location,
              const char * file,
              const char * func,
                    int    line )
        : m_level( level ),
          m_location( location ),
          m_file( file ),
          m_func( func ),
          m_line( line )
    {
    }

    ~LogEntry()
    {
        finalizeConsole();
    }

    std::ostream& stream()
    {
        return m_stream;
    }

private:
    void finalizeConsole()
    {
        //
        // Console output
        //
        std::ostream& css = LogManager::getConsoleStream();
        std::ostream& fss = LogManager::getFileStream();
        
        css << "["
            << getLogLevelStr(m_level) << ", "
            << m_location << "]: "
            << m_stream.str()
            << std::endl;

        //
        // File output
        //
    }
    
    void formatLogEntry()
    {
        m_stream << "[" << getLogLevelStr(m_level) << "; "
                 << m_location << "]: ";
    }

    const char * getLogLevelStr( LogLevel level ) const
    {
        switch( level )
        {
            case LOG_TRACE:
                return "TRACE";
            case LOG_DEBUG:
                return "DEBUG";
            case LOG_INFO:
                return "INFO ";
            case LOG_NOTICE:
                return "NOTE ";
            case LOG_WARNING:
                return "WARN ";
            case LOG_ERROR:
                return "ERROR";
            case LOG_FATAL:
                return "FATAL";
            default:
                return "UNKWN";
        }
    }

private:
    LogLevel     m_level;
    const char * m_location;
    const char * m_file;
    const char * m_func;
    int m_line;
    std::stringstream m_stream;
};

int main( int argc, char* argv[] )
{
    std::cout << "hello world" << std::endl;

    //LogEntry entry(LOG_TRACE);
    TRACE("app") << "this" << " is a " << "test";
    //entry.finalize();
}
