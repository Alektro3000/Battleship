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
    
    
    constexpr static RectF TextDesc = {{0.05, 0.3}, {0.4, 0.38}};
    constexpr static RectF Text = {{0.05, 0.4}, {0.4, 0.48}};
    constexpr static RectF RulesSelectPos = {{0.05, 0.5}, {0.7, 0.9}};
    constexpr static RectF ButtonPos = {{0.7, 0.88}, {0.98, 0.99}};

    MakeServer::MakeServer() : Overlay(
        {ButtonPos, Builder::makeText(L"Создать сервер").addButton([this](auto _) { makeServer(); }).build()},
        {RulesSelectPos, RulesSelect{}},
        {TextDesc, Builder::makeText(L"Название сервера:").build()},
        {Text, Builder::makeText(L"").setEditableText().addPadding().setBorder().build()}) {};
    
    void MakeServer::makeServer()
    {
        if (isServerEnabled)
            return;
        auto str = getWidget<3>().getChild().getText();
        if(str.size() == 0)
            return;
        isServerEnabled = true;
        getWidget<2>().setText(L"Имя сервера:");
        
        rules = getWidget<1>().getRules();
        server = std::async(std::launch::async, [str, this]() -> SocketContext
                            {
                auto context = std::make_unique<boost::asio::io_context>();
                contextPointer = &(*context);
                ServerHolder holder = ServerHolder(*context, str, rules);
                boost::system::error_code err;
                context->run(err);
                if(contextPointer == nullptr)
                    return {std::optional<tcp::socket>{},nullptr};
                contextPointer = nullptr;
                isFutureReady = true;
                return std::pair(std::move(holder).getSocket(), std::move(context));  });
    }
    
    void MakeServer::onRender()
    {
        Overlay::onRender();

        if (isFutureReady)
        {
            auto ip = server.get();
            pushWidget(std::make_unique<SelectWidget>(rules,
                                                        std::make_unique<NetPlayer>(rules, std::move(ip.second), std::move(*ip.first), true), rules.isFirstAttacking()),
                         false);
        }
    }
    void MakeServer::onChar(wchar_t key)
    {
        if(!isServerEnabled)
            Overlay::onChar(key);
    }
    void MakeServer::onClickDown(MouseButton mouse)
    {
        if(!isServerEnabled)
            Overlay::onClickDown(mouse);
    }
    void MakeServer::onClickUp(MouseButton mouse)
    {
        if(!isServerEnabled)
            Overlay::onClickUp(mouse);
    }
}