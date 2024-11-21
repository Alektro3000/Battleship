#include <iostream>
#include <functional>
#include <thread>
#include <boost/asio.hpp>
#include <array>
/*
using boost::asio::ip::tcp;
class tcp_server
{
  io_context_
  acc
public:
  tcp_server(boost::asio::io_context &io_context)
      : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }
  void start_accept()
  {
    tcp_connection::pointer new_connection =
        tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
                           std::bind(&tcp_server::handle_accept, this, new_connection,
                                     boost::asio::placeholders::error));
  }
  void handle_accept(tcp_connection::pointer new_connection,
                     const boost::system::error_code &error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
  typedef std::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context &io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket &socket()
  {
    return socket_;
  }
  void start()
  {
    message_ = make_daytime_string();
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                             std::bind(&tcp_connection::handle_write, shared_from_this()));
  }

private:
  tcp_connection(boost::asio::io_context &io_context)
      : socket_(io_context)
  {
  }

  void handle_write()
  {
  }

  tcp::socket socket_;
  std::string message_;
};
int main(int argc, char *argv[])
{

  boost::asio::io_context io_context;
  tcp_server(io_context);
  try
  {
    tcp::resolver resolver(io_context);

    tcp::resolver::results_type endpoints =
        resolver.resolve(argv[1], "daytime");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      std::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
*/