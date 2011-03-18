#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace boost::asio;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
public:
    typedef boost::shared_ptr<TcpConnection> pointer;

    static pointer create( io_service& ioService )
    {
        return pointer( new TcpConnection( ioService ) );
    }

    tcp::socket& socket()
    {
        return m_socket;
    }

    void start()
    {
        m_message = "hello there";

        boost::asio::async_write(
            m_socket,
            boost::asio::buffer( m_message ),
            boost::bind( &TcpConnection::handleWrite,
                         shared_from_this(),
                         boost::asio::placeholders::error,
                         boost::asio::placeholders::bytes_transferred )
        );
    }

private:
    TcpConnection( boost::asio::io_service& ioService )
        : m_socket( ioService )
    {
    }

    void handleWrite( const boost::system::error_code& error,
                      size_t bytesTransferred )
    {
    }

private:
    tcp::socket m_socket;
    std::string m_message;
};

class ChatServer
{
public:
    ChatServer( io_service& ioService )
        : m_acceptor( ioService, tcp::endpoint( tcp::v4(), 13 ) )
    {
        startAccept();
    }

private:
    void startAccept()
    {
        tcp_connection::pointer connection =
            tcp_connection::create( m_acceptor.io_service() );

        m_acceptor.async_accept( connection->socket,
            boost::Bind( &ChatServer::handleAccept,
                         this,
                         connection,
                         placeholders::error )
        );
    }

    void handleAccept( tcp_connection::pointer newConnection,
                       const boost::system::error_code& error )
    {
        if ( error )
        {
            std::cerr << "Had error ,bailing" << std::endl;
        }

        newConnection->start();
        startAccept();
    }
};

int main( int argc, char* argv[] )
{
    try
    {
        io_service io_service;
        ChatServer server( io_service );

        io_service.run();
    }
    catch ( std::exception& ex )
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
