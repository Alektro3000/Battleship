#include "ServerConnections.h"
#include "MakeServer.h"
#include "../Battle/SelectWidget.h"
#include <future>

namespace widget
{
    class ServerHolder
    {
    public:
        boost::asio::ip::tcp::socket getSocket() &&
        {
            return std::move(connectSocket);
        }
        ServerHolder(boost::asio::io_context &context, std::wstring name, GameRules rules)
            : fetchSocket(context, udp::endpoint(udp::v4(), serverPort)),
              connectAcceptor(context, tcp::endpoint(tcp::v4(), serverPort)),
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
                return;
            }
            // Connecting
            boost::asio::async_read(connectSocket, boost::asio::buffer(connectBuffer, sizeof(connectMessage)), [this](auto err, auto len)
                                    {
                    if(err)
                    {
                        startAccept();
                        return;
                    }
                    if(std::equal(std::begin(connectMessage), std::end(connectMessage), connectBuffer.begin()))
                    {
                        boost::asio::async_write(connectSocket, 
                            boost::asio::buffer(&rules,sizeof(GameRules)),
                            [this](auto err, auto len){
                            fetchSocket.cancel();
                            fetchSocket.close();


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
        server = std::async(std::launch::async, [str, this]() -> std::pair<tcp::socket, std::unique_ptr<boost::asio::io_context> >
                            {
                auto context = std::make_unique<boost::asio::io_context>();
                contextPointer = &(*context);
                ServerHolder holder = ServerHolder(*context, str, rules);
                boost::system::error_code err;
                context->run(err);
                contextPointer = nullptr;
                isFutureReady = true;
                return std::pair(std::move(holder).getSocket(), std::move(context));  });
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
            auto ip = server.get();
            ChangeWidget(std::make_unique<SelectWidget>(rules,
                                                        std::make_unique<NetPlayer>(rules, std::move(ip.first), std::move(ip.second),true), rules.isFirstAttacking()),
                         false);
        }
    }
}