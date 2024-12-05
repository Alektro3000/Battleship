#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/lockfree/queue.hpp>
#include <winsock2.h>
#include <iphlpapi.h>

struct Response
{
  std::array<wchar_t, 32> name;
  int len;
};

struct ResponseFull
{
  Response response;
  boost::asio::ip::address_v4 ipv4;
};
using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct netIps
{
    boost::asio::ip::address_v4 ip;
    boost::asio::ip::address_v4 broadcast;
};

std::vector<netIps> queryDevices() {
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    AdapterInfo = new IP_ADAPTER_INFO;
    if (AdapterInfo == nullptr) {
        std::cerr << "Error allocating memory needed to call GetAdaptersinfo\n";
        return {};
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the dwBufLen variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(AdapterInfo);
        //Reaaloc if we don't have enough memory
        AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
        if (AdapterInfo == nullptr) {
            std::cerr << "Error allocating memory needed to call GetAdaptersinfo\n";
            return {};
        }
    }
    std::vector<netIps> ans;
    if( GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR ){
      
      const IP_ADAPTER_INFO *pAdapter = AdapterInfo;
        while (pAdapter != nullptr) {
          if (pAdapter->Type == MIB_IF_TYPE_ETHERNET
          || pAdapter->Type == IF_TYPE_IEEE80211) {
          const IP_ADDR_STRING *pAddr = &(pAdapter->IpAddressList);
          while (pAddr != nullptr)
          {
            auto val = boost::asio::ip::address::from_string(pAddr->IpAddress.String).to_v4();
            auto mask = boost::asio::ip::address::from_string(pAddr->IpMask.String).to_v4();
            if(!val.is_unspecified())
            {
              auto broad = (mask.to_ulong() & val.to_ulong()) + (~mask.to_uint());
              boost::asio::ip::address_v4 broadip = boost::asio::ip::make_address_v4(broad);
              ans.emplace_back(val,broadip);
            }
            pAddr = pAddr->Next;
          }
          pAdapter = pAdapter->Next;
        }
      }
    }
    free(AdapterInfo);
    return ans;
}

class ResponseCollector {
public:
    std::vector<ResponseFull> responses;
    ResponseCollector(boost::asio::io_context& io_context, udp::socket& socket)
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
#ifdef AdditionalLogging
            std::cout << "Received: " << bytes_transferred << " ";
            std::wcout << std::wstring(recv_buffer_[0].name.begin(), recv_buffer_[0].name.begin()+recv_buffer_[0].len) << std::endl;
            std::cout << "from " << remote_endpoint_.address() << std::endl;
#endif
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

std::vector<ResponseFull> queryServers(boost::asio::io_context& io_context, netIps ip)
{
    udp::socket socket(io_context,udp::v4());
    socket.set_option(boost::asio::socket_base::broadcast(true));    
    
    udp::endpoint senderEndpoint(ip.broadcast, 3190);

    socket.send_to(boost::asio::buffer(make_daytime_string()), senderEndpoint);
    socket.set_option(boost::asio::socket_base::broadcast(false));    
    
    ResponseCollector val(io_context, socket);
    io_context.run();
    return val.responses;
}

std::vector<ResponseFull> queryServers()
{
  
    auto devices = queryDevices();
    std::vector<std::future<std::vector<ResponseFull> > > tasks;
    for(auto ip:devices)
    {
      tasks.push_back(std::async([ip](){
        boost::asio::io_context io_context;
        auto resp = queryServers(io_context,ip);
#ifdef AdditionalLogging
        std::cout << "Ended quering: " << ip.ip.to_string() << std::endl;
#endif
        return resp;
      }));
    }
    std::vector<ResponseFull> responses;
    for(auto& task:tasks)
    {
      auto val = task.get();
      std::copy(val.begin(),val.end(),std::back_inserter(responses));
    }

#ifdef AdditionalLogging
    std::cout << "Total Answer" << std::endl;
#endif
    for(auto response:responses)
    {
      std::wcout << std::wstring(response.response.name.begin(), response.response.name.begin() + response.response.len) << std::endl;
      std::cout << "From: " <<  response.ipv4.to_string() <<std::endl;
    }
     
}