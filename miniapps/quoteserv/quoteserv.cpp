#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int PORT = 4200;

typedef std::vector<std::string> QuoteList;

bool loadQuoteList( const std::string& file, QuoteList& output );

int main()
{
    //
    // Load quote list
    //
    QuoteList quotes;

    if (! loadQuoteList( "quotes.txt", quotes ) )
    {
        std::cerr << "Failed to load list of quotes. Exiting." << std::endl;
        return 1;
    }

    //
    // Enter server mode
    //
    try
    {
        boost::asio::io_service io_service;

        // Listen for connections on requested port
        tcp::acceptor acceptor( io_service, tcp::endpoint(tcp::v4(),PORT) );

        // Handle requests as they come in (one at a time)
        while ( true )
        {
            tcp::socket socket( io_service );
            accetor.accept( socket );

            std::string message = "hello there";

            boost::system::error_code ignored_error;
            boost::asio::write(
                    socket,
                    boost::asio::buffer(message),
                    boost::asio::transfer_all(),
                    ignored_error
            );

        }
    }
    catch ( std::exception& e )
    {
        std::cerr << "FATAL EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}


bool loadQuoteList( const std::string& filepath, QuoteList& output )
{
    std::ifstream file( filepath.c_str(), std::ios_base::in );
    std::string line;

    // Did the file open?
    if ( filepath.good() == false )
    {
        return false;
    }

    // Read each line and append to quote list
    //  (not very efficient, but we do this once on startup)
    while ( std::getline( file, line, '\n' ) )
    {
        output.push_back( line );
    }

    return true;
}
