#include "ServerConnections.h"

namespace widget
{
  struct netIps
  {
    boost::asio::ip::address_v4 ip;
    boost::asio::ip::address_v4 broadcast;
  };

  std::vector<netIps> queryDevices()
  {
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    AdapterInfo = new IP_ADAPTER_INFO;
    if (AdapterInfo == nullptr)
    {
      std::cerr << "Error allocating memory needed to call GetAdaptersinfo\n";
      return {};
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the dwBufLen variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
    {
      free(AdapterInfo);
      // Reaaloc if we don't have enough memory
      AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
      if (AdapterInfo == nullptr)
      {
        std::cerr << "Error allocating memory needed to call GetAdaptersinfo\n";
        return {};
      }
    }
    std::vector<netIps> ans;
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
    {

      const IP_ADAPTER_INFO *pAdapter = AdapterInfo;
      while (pAdapter != nullptr)
      {
        if (pAdapter->Type == MIB_IF_TYPE_ETHERNET || pAdapter->Type == IF_TYPE_IEEE80211)
        {
          const IP_ADDR_STRING *pAddr = &(pAdapter->IpAddressList);
          while (pAddr != nullptr)
          {
            auto val = boost::asio::ip::address::from_string(pAddr->IpAddress.String).to_v4();
            auto mask = boost::asio::ip::address::from_string(pAddr->IpMask.String).to_v4();
            if (!val.is_unspecified())
            {
              auto broad = (mask.to_ulong() & val.to_ulong()) + (~mask.to_uint());
              boost::asio::ip::address_v4 broadip = boost::asio::ip::make_address_v4(broad);
              ans.emplace_back(val, broadip);
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


  std::vector<ResponseFull> queryServers(boost::asio::io_context &io_context, netIps ip)
  {
    udp::socket socket(io_context, udp::v4());
    socket.set_option(boost::asio::socket_base::broadcast(true));

    udp::endpoint senderEndpoint(ip.broadcast, port);

    socket.send_to(boost::asio::buffer(fetchMessage), senderEndpoint);
    socket.set_option(boost::asio::socket_base::broadcast(false));

    ResponseCollector val(io_context, socket);
    boost::system::error_code ignoredError;
    io_context.run(ignoredError);
    if(ignoredError)
      return {};
    return val.responses;
  }

  std::vector<ResponseFull> queryServers(boost::asio::io_context& io_context)
  {
    auto devices = queryDevices();
    std::vector<std::future<std::vector<ResponseFull>>> tasks;
    for (auto ip : devices)
    {
      tasks.push_back(std::async([ip, &io_context]()
                                 {
        auto resp = queryServers(io_context,ip);
        return resp; }));
    }
    std::vector<ResponseFull> responses;
    for (auto &task : tasks)
    {
      auto val = task.get();
      std::copy(val.begin(), val.end(), std::back_inserter(responses));
    }
    return responses;
  }

  void ServerHolder::handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred)
  {
    if (!error)
    {
      if (std::equal(std::begin(fetchMessage), std::end(fetchMessage), recv_buffer_.begin()))
      {
        Response val;
        std::copy(name.begin(), name.end(), val.name.begin());
        val.len = name.size();
        socket_.async_send_to(boost::asio::buffer(&val, sizeof(Response)), remote_endpoint_,
                              [](auto val, auto val1) {});
      }
      if (std::equal(std::begin(connectMessage), std::end(connectMessage), recv_buffer_.begin()))
      {
        
      }
      // Ready to receive next packet
      socket_.async_receive_from(
          boost::asio::buffer(recv_buffer_),
          remote_endpoint_,
          [this](auto error, auto bytes)
          { handleReceive(error, bytes); });
    }
  }
}