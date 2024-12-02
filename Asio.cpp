#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <winsock2.h>
#include <iphlpapi.h>

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}


struct Response
{
  std::array<wchar_t, 32> name;
  int len;
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


std::string get_local_ip(boost::asio::io_service& io_service) 
{ 
  try { 
    udp::resolver resolver(io_service); 
    udp::resolver::query query(boost::asio::ip::host_name(), ""); 
    udp::resolver::iterator iter = resolver.resolve(query); 
    udp::resolver::iterator end; 

    while (iter != end) 
    { 
      boost::asio::ip::udp::endpoint ep = *iter++; 
      if (ep.address().is_v4()) { std::cout << ep.address().to_string() << std::endl; } 
      } 
  } 
  catch (std::exception& e) { 
        std::cerr << "Exception: " << e.what() << std::endl; 
  } 
  return "";
}

std::vector<std::string> queryServers(boost::asio::io_service& io_service, netIps ip)
{
    udp::socket socket(io_service,udp::v4());
    socket.set_option(boost::asio::socket_base::broadcast(true));    
    

    udp::endpoint senderEndpoint(ip.broadcast, 3190);

    socket.send_to(boost::asio::buffer(make_daytime_string()), senderEndpoint);

    Response reply[1];
    boost::system::error_code error;
    udp::endpoint responseEndpoint(boost::asio::ip::address_v4::any(),3190);
    std::vector<std::string> ip_addresses;

    while (true) {
        size_t len = socket.async_receive_from(boost::asio::buffer(reply), responseEndpoint,
          boost::bind(&handle_receive, this, boost::asio::placeholders::error, 
          boost::asio::placeholders::bytes_transferred));
        if (error == boost::asio::error::would_block || error == boost::asio::error::message_size) {
            break; // Stop receiving if no more responses or buffer overflow
        } else if (!error) {
            std::string ip = responseEndpoint.address().to_string();
            ip_addresses.push_back(ip);
            std::cout << "Received reply from " << ip << std::endl;
            std::cout << "Reply: ";
            std::wcout << std::wstring{reply->name.begin(),reply->name.begin()+reply->len} << std::endl;
        }
    }
    return ip_addresses;
}


int main(int argc, char* argv[])
{
  try
  {
    auto a =  queryDevices();
    boost::asio::io_service io_service;
    for(auto q:a)
    {
      queryServers(io_service,q);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}