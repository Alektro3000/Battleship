#define BOOST_ASIO_ENABLE_HANDLER_TRACKING 1
#define UNICODE
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
  constexpr const boost::asio::ip::port_type port = 3190;

  using boost::asio::ip::tcp;
  using boost::asio::ip::udp;

  struct Response
  {
    std::array<wchar_t, 32> name;
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
  };

  // Collects responses on socket for curtain time
  class ResponseCollector
  {
  public:
    std::vector<ResponseFull> responses;
    ResponseCollector(boost::asio::io_context &io_context, udp::socket &socket)
        : socket_(socket),
          deadline_(io_context) 
    {
      startReceive();
    }

  private:
    void startReceive()
    {
      deadline_.expires_from_now(boost::posix_time::seconds(1));
      socket_.async_receive_from(
          boost::asio::buffer(recv_buffer_),
          remote_endpoint_,
          [this](auto error, auto bytes)
          { handleReceive(error, bytes); });
      deadline_.async_wait([this](const auto &error)
                           { checkDeadline(); });
    }

    void handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred)
    {
      if (!error)
      {
        deadline_.expires_from_now(boost::posix_time::seconds(4));
        responses.push_back({recv_buffer_[0], remote_endpoint_.address().to_v4()});
        // Ready to receive next packet
        startReceive();
      }
    }
    void checkDeadline()
    {
      // Check for false invocation
      if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
      {
        socket_.cancel();
        socket_.close();
        // Disable timer
        deadline_.expires_at(boost::posix_time::pos_infin);
      }
    }
    udp::socket &socket_;
    udp::endpoint remote_endpoint_;
    std::array<Response, 2> recv_buffer_;
    boost::asio::deadline_timer deadline_;
  };

  std::vector<ResponseFull> queryServers(boost::asio::io_context&);

  class ServerHolder
  {
  public:
    ServerHolder(boost::asio::io_context &io_context, std::stop_token stop_token, std::wstring name)
        : socket_(io_context, udp::endpoint(udp::v4(), port)),
          deadline_(io_context), name(name),
          stop_token(stop_token)
    {
      startReceive();
    }

  private:
    void startReceive()
    {
      socket_.async_receive_from(
          boost::asio::buffer(recv_buffer_),
          remote_endpoint_,
          [this](auto error, auto bytes)
          { handleReceive(error, bytes); });
      deadline_.expires_from_now(boost::posix_time::millisec(10000));
      deadline_.async_wait([this](const auto &error)
                           { checkDeadline(); });
    }

    void handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred);
    void checkDeadline()
    {
      // Check for stop request
      if (stop_token.stop_requested())
      {
        socket_.cancel();
        socket_.close();
      }
      else
      {
        deadline_.cancel();
        deadline_.expires_from_now(boost::posix_time::millisec(10000));
        deadline_.async_wait([this](const auto &error)
                             { checkDeadline(); });
      }
    }
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 40> recv_buffer_;
    std::stop_token stop_token;
    boost::asio::deadline_timer deadline_;
    std::wstring name;
  };

  void holdServer();
}
#endif