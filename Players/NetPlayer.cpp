#include "NetPlayer.h"
#include <boost/asio.hpp>

using boost::asio::write;
using boost::asio::buffer;

NetPlayer::NetPlayer(GameRules rules, boost::asio::ip::tcp::socket&& socket, 
    contextPtr&& context, bool server):
 socket(std::move(socket)), context(std::move(context)) {

};
AttResult NetPlayer::makeMove(PointI x) {
    write(socket, buffer(&x,sizeof(PointI)));
    AttResult res{Results::Miss};
    read(socket, buffer(&res, sizeof(AttResult)));
    return res;
};
std::vector<BattleShip> NetPlayer::showAllShips() {
    return {};
};
std::size_t NetPlayer::getHashGrid() {
    return 0;
};
PointI NetPlayer::getMove() {
    PointI res;
    read(socket, buffer(&res, sizeof(PointI)));
    return res;
};
void NetPlayer::returnResult(AttResult res) {
    write(socket, buffer(&res,sizeof(AttResult)));
};