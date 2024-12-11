#include "ServerNode.h"
#include "../Battle/SelectWidget.h"
#include "../../Players/NetPlayer.h"

namespace widget
{
    std::wstring infoToW(ResponseFull info)
    {
        auto a = info.ipv4.to_string();
        return {a.begin(),a.end()};
    }

    ServerNode::ServerNode(ResponseFull info) : info(info),
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
    void ServerNode::onClick(MouseButton button) {
        
        auto context = std::make_unique<boost::asio::io_context>();
        boost::asio::ip::tcp::socket socket(*context, tcp::v4());
        socket.bind(tcp::endpoint(info.ipLocal, clientPort));
        socket.connect(tcp::endpoint(info.ipv4, serverPort));
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
        
        ChangeWidget(std::make_unique<SelectWidget>(rules,
                     std::make_unique<NetPlayer>(rules, std::move(socket), std::move(context), false),!rules.isFirstAttacking()),
                     false);
    };
    void ServerNode::onErrorConnection()
    {

    }
}