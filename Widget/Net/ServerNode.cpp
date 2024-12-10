#include "ServerNode.h"
#include "../Battle/SelectWidget.h"
#include "../../Players/PcPlayer.h"

namespace widget
{
    ServerNode::ServerNode(ResponseFull info) : info(info),
                                                Stack(
                                                    Builder::makeOverlay<Padder<TextBox>, Padder<TextBox>>(
                                                        {RectF{{0, 0}, {0.4, 1}}, Builder::makeText(L"text")
                                                                                                .addPadding()
                                                                                                .setBorder()
                                                                                                .build()},
                                                        {RectF{{0.4, 0}, {1, 1}}, Builder::makeText(info.response.getString())
                                                                                                .addPadding()
                                                                                                .setBorder()
                                                                                                .build()})
                                                        .addPadding()
                                                        .setBorder()
                                                        .build()) {};
    void ServerNode::onClick(MouseButton button) {
        
        boost::asio::io_context context;
        boost::asio::ip::udp::socket val(context, udp::v4());
        val.send_to(boost::asio::buffer(connectMessage), udp::endpoint(info.ipv4, port));
        
        boost::asio::ip::tcp::socket sock(context, tcp::endpoint(info.ipv4, port));
        
        GameRules rules{};
        boost::system::error_code err;
        boost::asio::read(sock,boost::asio::buffer(&rules, sizeof(rules)), err);
        

        ChangeWidget(std::make_unique<SelectWidget>(rules,
                     std::make_unique<PCPlayer>(rules),
                     false));
    };
}