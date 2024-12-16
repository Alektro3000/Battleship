#include "ServerList.h"

namespace widget
{
    struct netIps
    {
        boost::asio::ip::address_v4 ip;
        boost::asio::ip::address_v4 broadcast;
    };

    // Collects responses on socket for curtain time
    class ResponseCollector
    {
    public:
        std::vector<ResponseFull> responses;
        ResponseCollector(boost::asio::io_context &io_context, udp::socket &socket)
            : socket_(socket),
              deadline_(io_context, boost::posix_time::seconds(1))
        {
            startReceive();
            deadline_.async_wait([this](const auto &error)
                                 { checkDeadline(error); });
        }

    private:
        void startReceive()
        {
            socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_),
                remote_endpoint_,
                [this](auto error, auto bytes)
                { handleReceive(error, bytes); });
        }

        void handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred)
        {
            if (error)
                return;

            // If it's not from us
            if (!(remote_endpoint_.address() == socket_.local_endpoint().address() && bytes_transferred != sizeof(Response)))
                responses.push_back({
                    recv_buffer_[0],
                    remote_endpoint_.address().to_v4(),
                    socket_.local_endpoint().address().to_v4(),
                });
            // Ready to receive next packet
            startReceive();
        }
        void checkDeadline(const boost::system::error_code &error)
        {
            if(error)
                return;
            // Check for false invocation
            if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
            {
                socket_.cancel();
                socket_.close();
                // Disable timer
                deadline_.cancel();
            }
        }
        boost::asio::deadline_timer deadline_;
        udp::socket &socket_;
        udp::endpoint remote_endpoint_;
        std::array<Response, 2> recv_buffer_;
    };

    // return all ip addresses of current Computer and appropriate broadcast ip
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

    // finds servers in selected subnet
    std::vector<ResponseFull> queryServers(netIps ip)
    {
        boost::asio::io_context io_context;
        udp::socket socket(io_context,udp::endpoint(ip.ip, clientPort) );

        socket.set_option(boost::asio::socket_base::broadcast(true));
        udp::endpoint senderEndpoint(ip.broadcast, serverPort);
        boost::system::error_code err;
        socket.send_to(boost::asio::buffer(fetchMessage), senderEndpoint, {}, err);
        if (err)
        {
            std::cerr << err.to_string() << " Error failed to send to " << ip.broadcast.to_string() << std::endl;
            // If failed to send broadcast message there is no need to wait for response
            return {};
        }

        socket.set_option(boost::asio::socket_base::broadcast(false));
        ResponseCollector val(io_context, socket);

        boost::system::error_code ignoredError;
        io_context.run(ignoredError);

        if (ignoredError)
            return {};

        return val.responses;
    }

    // finds servers
    std::vector<ResponseFull> queryServers()
    {
        auto devices = queryDevices();
        // Asynchronously send messages for all subnets
        std::vector<std::future<std::vector<ResponseFull>>> tasks;
        for (auto ip : devices)
            tasks.push_back(std::async([ip]()
                                       { return queryServers(ip); }));

        // Gather info in one
        std::vector<ResponseFull> responses;
        for (auto &task : tasks)
        {
            auto val = task.get();
            std::copy(val.begin(), val.end(), std::back_inserter(responses));
        }
        return responses;
    }

    ServerList::ServerList() : Overlay(
                                   {RectF{{0.7, 0.92}, {0.99, 0.98}},
                                    Builder::makeText(L"Создать сервер")
                                        .addPadding(RectF{{10, 3}})
                                        .setBorder()
                                        .addButton(
                                            [this](auto _)
                                            { pushWidget(std::make_unique<MakeServer>()); })
                                        .build()},
                                   {RectF{{0.5, 0.92}, {0.68, 0.98}},
                                    Builder::makeText(L"Обновить")
                                        .addPadding(RectF{{10, 3}})
                                        .setBorder()
                                        .addButton(
                                            [this](auto _)
                                            { updateServers(); })
                                        .build()},
                                   {RectF{{0.1}, {0.9}},
                                    Builder::makeList<ServerNode>(10, 0, 0.1)
                                        .addPadding(RectF{{10, 10}})
                                        .setBorder()
                                        .build()})
    {
        updateServers();
    }
    void ServerList::updateServers()
    {
        if (!quering.valid())
        {
            isUpdating = true;
            isFutureReady = false;
            quering = std::async(std::launch::async, [this]()
                                 {
                                    auto a = queryServers(); 
                                    isFutureReady = true;
                                    return a; });
        }
    }
    void ServerList::onResize(RectF newSize)
    {
        Overlay::onResize(newSize);
        NoServerBox.onResize(RectF{{0.1, 0.1}, {0.9, 0.9}}.scaled(newSize));
    }
    void ServerList::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
        Overlay::onRender();

        if (isUpdating && isFutureReady)
        {
            std::vector<ResponseFull> servers = quering.get();
            auto &serverNodes = getWidget<2>().getChild().childs;
            serverNodes.clear();
            std::transform(servers.begin(), servers.end(), std::back_insert_iterator(serverNodes), [this](auto val)
                           { return ServerNode{this, val}; });
            getWidget<2>().getChild().update();
            isUpdating = false;
        }

        if (getWidget<2>().getChild().childs.size() == 0)
            NoServerBox.onRender();

        if (isUpdating)
        {
            Context::getInstance().getRenderTarget()->FillRectangle(makeD2DRectF(RectF{{0.1, 0.1}, {0.9, 0.9}}.scaled(getPosition())), halfOpacity);
        }
    }
}