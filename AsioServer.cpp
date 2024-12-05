
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
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}