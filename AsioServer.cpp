
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

struct Response
{
  std::array<wchar_t, 32> name;
  int len;
};
Response makeResponse()
{
  return {L"HI!",3};
}

int main()
{
  try
  {
    boost::asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 3190));
    
    for (;;)
    {
      std::array<char,128> buff;

      udp::endpoint remote_endpoint(boost::asio::ip::address_v4::broadcast(),3190);
      
      auto len = socket.receive_from(boost::asio::buffer(buff), remote_endpoint);
      Response resp = makeResponse();
      std::array<Response, 1> val;
      val[0] = resp;
      socket.send_to(boost::asio::buffer(val), remote_endpoint, 0);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}