#include "ServerConnections.h"
#include "MakeServer.h"
#include "../Battle/SelectWidget.h"
#include <future>

namespace widget
{
    class ServerHolder
    {
    public:
        boost::asio::ip::address_v4 getAddress()
        {
            return fetchEndpoint.address().to_v4();
        }
        ServerHolder(boost::asio::io_context &context, std::wstring name, GameRules rules)
            : fetchSocket(context, udp::endpoint(udp::v4(), port)),
              connectAcceptor(context, tcp::endpoint(tcp::v4(), port)),
              connectSocket(context),
              rules(rules), name(name)
        {
            startFetch();
            startAccept();
        }

    private:
        void startAccept()
        {
            connectAcceptor.async_accept(connectSocket, [this](auto error)
                                         { handleConnect(error); });
        }
        void startFetch()
        {
            fetchSocket.async_receive_from(
                boost::asio::buffer(fetchBuffer),
                fetchEndpoint,
                [this](auto error, auto bytes)
                { handleFetch(error, bytes); });
        }
        void handleFetch(const boost::system::error_code &error, std::size_t bytesTransferred)
        {
            if (error)
                return;

            // Fetching
            if (std::equal(std::begin(fetchMessage), std::end(fetchMessage), fetchBuffer.begin()))
            {
                Response val;
                std::copy(name.begin(), name.end(), val.name.begin());
                val.len = name.size();
                fetchSocket.async_send_to(boost::asio::buffer(&val, sizeof(Response)), fetchEndpoint,
                                          [](auto val, auto val1) {});
            }
            // Ready to receive next packet
            startFetch();
        }

        void handleConnect(const boost::system::error_code &error)
        {
            if (error)
            {
                startAccept();
                return;
            }
            // Connecting
            boost::asio::async_read(connectSocket, boost::asio::buffer(connectBuffer), [this](auto err, auto len)
                                          {
                    if(err)
                    {
                        startAccept();
                        return;
                    }
                    if(std::equal(std::begin(connectMessage), std::end(connectMessage), connectBuffer.begin()))
                    {
                        boost::asio::async_write(connectSocket, boost::asio::buffer(&rules,sizeof(GameRules)),[](auto err, auto len){
                            
                        });
                    } });
        }

        udp::socket fetchSocket;
        udp::endpoint fetchEndpoint;
        std::array<char, 40> fetchBuffer;

        tcp::acceptor connectAcceptor;
        tcp::socket connectSocket;
        std::array<char, 40> connectBuffer;

        std::wstring name;
        GameRules rules;
    };

    void MakeServer::makeServer()
    {
        if (isServerEnabled)
            return;
        isServerEnabled = true;
        auto str = text.getChild().getText();
        server = std::async(std::launch::async, [str, this]()
                            {
                boost::asio::io_context context;
                contextPointer = &context;
                ServerHolder holder = ServerHolder(context, str, rules);
                boost::system::error_code err;
                context.run(err);
                contextPointer = nullptr;
                isFutureReady = true;
                return holder.getAddress(); });
    }
    void MakeServer::onResize(RectF newSize)
    {
        Overlay::onResize(newSize);
        if (!isServerEnabled)
            text.onResize(TextBegin.scaled(newSize));
    }
    void MakeServer::onChar(WCHAR letter)
    {
        if (!isServerEnabled)
            text.onChar(letter);
    }

    void MakeServer::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
        Overlay::onRender();
        if (!isServerEnabled)
            text.onRender();

        if (isFutureReady)
        {
            boost::asio::ip::address_v4 ip = server.get();
            ChangeWidget(std::make_unique<SelectWidget>(rules,
                                                        std::make_unique<NetPlayer>(rules, ip, true),
                                                        false),
                         false);
        }
    }
}