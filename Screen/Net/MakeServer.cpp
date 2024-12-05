#include "MakeServer.h"
#include <future>
#include <boost/asio.hpp>
using boost::asio::ip::udp;

/*
class Server {
public:
    Server(boost::asio::io_context& io_context, udp::socket& socket)
        : socket_(socket),
        deadline_(io_context) {
        startReceive();
    }

private:
    void startReceive() {
        deadline_.expires_from_now(boost::posix_time::seconds(1));
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_),
            remote_endpoint_,
           [this](auto error, auto bytes){handleReceive(error,bytes);});
        deadline_.async_wait([this](const auto& error){checkDeadline();});
    }

    void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            deadline_.expires_from_now(boost::posix_time::seconds(4));
            responses.push_back({recv_buffer_[0], remote_endpoint_.address().to_v4()});
            // Ready to receive next packet
            startReceive(); 
        }
    }
    void checkDeadline()
    {
      //Check for false invocation
      if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
      {
        socket_.cancel();
        socket_.close();
        //Disable timer
        deadline_.expires_at(boost::posix_time::pos_infin);
      }
    }
    udp::socket& socket_;
    udp::endpoint remote_endpoint_;
    std::array<Response, 2> recv_buffer_;
    boost::asio::deadline_timer deadline_;
};
*/


void holdServer(std::stop_token stop_token, std::wstring name)
{
    boost::asio::io_context io_context;
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 3190));
    
    while(!stop_token.stop_requested())
    {
      std::array<char,128> buff;

      udp::endpoint remote_endpoint;
      
      auto len = socket.receive_from(boost::asio::buffer(buff), remote_endpoint);
      std::array<Response, 1> val;
      val[0].len = name.size();
      assert(val[0].len <= 32);
      std::copy(name.begin(),name.end(),val[0].name.begin());
      boost::system::error_code a;
      socket.send_to(boost::asio::buffer(val), remote_endpoint, 0, a);
    }

    socket.cancel();
    socket.close();
}

void ServerScreen::makeServer()
{

}
void ServerScreen::onResize(RectF newSize)
{
    ScreenOverlay::onResize(newSize);
    text.onResize(TextBegin.scaled(newSize));
}
void ServerScreen::onChar(WCHAR letter)
{
    text.onChar(letter);
}

void ServerScreen::onRender()
{
    GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
    ScreenOverlay::onRender();
    text.onRender();
}