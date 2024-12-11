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
                                                        .build()) {};
    void ServerNode::onClick(MouseButton button) {
        
        boost::asio::io_context context;
        boost::asio::ip::tcp::socket sock(context, tcp::endpoint(info.ipv4, port));
        boost::system::error_code err;
        boost::asio::write(sock, boost::asio::buffer(connectMessage),err);
        
        if(err)
        {
            onErrorConnection();
            return;
        }
        
        GameRules rules{};
        
        boost::system::error_code errc;
        boost::asio::read(sock,boost::asio::buffer(&rules, sizeof(rules)),errc);
        
        if(errc)
        {
            onErrorConnection();
            return;
        }
        
        ChangeWidget(std::make_unique<SelectWidget>(rules,
                     std::make_unique<NetPlayer>(rules, info.ipv4, true),
                     false));
    };
    void ServerNode::onErrorConnection()
    {

    }
}