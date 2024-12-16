#include "../WinMacros.h"
#include <boost/asio.hpp>
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <thread>

#ifndef ServerH
#define ServerH

namespace widget
{
  constexpr const char fetchMessage[] = "Finding server for Battleship";
  constexpr const char connectMessage[] = "Connecting to Battleship server";
  constexpr const boost::asio::ip::port_type serverPort = 3190;
  constexpr const boost::asio::ip::port_type clientPort = 3191;

  using boost::asio::ip::tcp;
  using boost::asio::ip::udp;

  struct Response
  {
    std::array<WCHAR, 32> name;
    std::size_t len;
    std::wstring getString()
    {
      return std::wstring(name.begin(), name.begin() + len);
    }
  };

  struct ResponseFull
  {
    Response response;
    boost::asio::ip::address_v4 ipv4;
    boost::asio::ip::address_v4 ipLocal;
  };

}
#endif