#include "ServerNode.h"
#include "ServerList.h"
#include "../Battle/SelectWidget.h"
#include "../../Players/NetPlayer.h"

namespace widget
{
    std::wstring infoToW(ResponseFull info)
    {
        auto a = info.ipv4.to_string();
        return {a.begin(),a.end()};
    }

    ServerNode::ServerNode(ServerList* server, ResponseFull info) : parent(server), info(info),
                                                Stack(
                                                    Builder::makeOverlay<Padder<TextBox>, Padder<TextBox>>(
                                                        {RectF{{0, 0}, {0.4, 1}}, Builder::makeText(infoToW(info))
                                                                                                .addPadding()
                                                                                                .build()},
                                                        {RectF{{0.4, 0}, {1, 1}}, Builder::makeText(info.response.getString())
                                                                                                .addPadding()
                                                                                                .build()})
                                                        .addPadding()
                                                        .setBorder()
                                                        .build()) {};
    void ServerNode::onClickDown(MouseButton button) {
        
        auto context = std::make_unique<boost::asio::io_context>();
        boost::asio::ip::tcp::socket socket(*context, tcp::v4());
        socket.bind(tcp::endpoint(info.ipLocal, clientPort));
        
        //Connection with timeout
        boost::asio::deadline_timer timer(*context, boost::posix_time::seconds(1));
        bool connected = false;
        socket.async_connect(tcp::endpoint(info.ipv4, serverPort),[&](auto err){
            connected = true;
            timer.cancel();
        });
        timer.async_wait([&](auto _){
            socket.cancel();
        });
        context->run();

        if(!connected)
        {
            onErrorConnection();
            return;
        }
        boost::system::error_code err;
        boost::asio::write(socket, boost::asio::buffer(connectMessage),err);
        
        if(err)
        {
            onErrorConnection();
            return;
        }
        
        GameRules rules{};
        
        boost::system::error_code errc;
        boost::asio::read(socket,boost::asio::buffer(&rules, sizeof(rules)),errc);
        
        if(errc)
        {
            onErrorConnection();
            return;
        }
        
        pushWidget(std::make_unique<SelectWidget>(rules,
                     std::make_unique<NetPlayer>(rules, std::move(context), std::move(socket), false),!rules.isFirstAttacking()),
                     false);
    };
    void ServerNode::onErrorConnection()
    {
        
    }
}