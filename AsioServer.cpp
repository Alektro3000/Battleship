
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    boost::asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 3190));
    
    for (;;)
    {
      boost::asio::streambuf buff;

      std::array<char, 40> recv_buf;
      udp::endpoint remote_endpoint(boost::asio::ip::address_v4::broadcast(),3190);
      
      auto len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
      std::string message = make_daytime_string();
      std::cout << std::string(recv_buf.begin(),recv_buf.begin() + len );

      socket.send_to(boost::asio::buffer(message),
          remote_endpoint, 0);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}