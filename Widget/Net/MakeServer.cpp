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
            return queryEndpoint.address().to_v4();
        }
        ServerHolder(boost::asio::io_context &context, std::wstring name, GameRules rules)
            : querySocket(context, udp::endpoint(udp::v4(), port)),
              connectSocket(context, tcp::endpoint(tcp::v4(), port)),
              rules(rules), name(name)
        {
            querySocket.async_receive_from(
                boost::asio::buffer(queryBuffer),
                queryEndpoint,
                [this](auto error, auto bytes)
                { handleQuery(error, bytes); });


            connectSocket.async_read_some(
                boost::asio::buffer(connectBuffer),
                connectEndpoint,
                [this](auto error, auto bytes)
                { handleConnect(error, bytes); });
        }

    private:
        void handleQuery(const boost::system::error_code &error, std::size_t bytesTransferred)
        {
            if (!error)
            {
                // Fetching
                if (std::equal(std::begin(fetchMessage), std::end(fetchMessage), queryBuffer.begin()))
                {
                    Response val;
                    std::copy(name.begin(), name.end(), val.name.begin());
                    val.len = name.size();
                    querySocket.async_send_to(boost::asio::buffer(&val, sizeof(Response)), queryEndpoint,
                                              [](auto val, auto val1) {});
                }
                // Ready to receive next packet
                querySocket.async_receive_from(
                    boost::asio::buffer(queryBuffer),
                    queryEndpoint,
                    [this](auto error, auto bytes)
                    { handleQuery(error, bytes); });
            }
        }
        void handleConnect(const boost::system::error_code &error, std::size_t bytesTransferred)
        {
            if (!error)
            {
                // Connecting
                if (std::equal(std::begin(connectMessage), std::end(connectMessage), connectBuffer.begin()))
                {
                    boost::asio::async_write(connectSocket, boost::asio::buffer(&rules, sizeof(GameRules)),
                                                [](auto val, auto val1) {});
                }
            }
        }

        udp::socket querySocket;
        udp::endpoint queryEndpoint;
        std::array<char, 40> queryBuffer;

        tcp::socket connectSocket;
        tcp::endpoint connectEndpoint;
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
                                                        false));
        }
    }
}