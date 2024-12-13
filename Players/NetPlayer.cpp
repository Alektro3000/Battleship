#include "NetPlayer.h"
#include <boost/asio.hpp>

using boost::asio::buffer;
using boost::asio::write;

NetPlayer::NetPlayer(GameRules rules, boost::asio::ip::tcp::socket &&socket,
                     contextPtr &&context, bool server) : socket(std::move(socket)), 
                     context(std::move(context)),
                     rules(rules) {

                                                          };
AttResult NetPlayer::makeMove(PointI x)
{
    write(socket, buffer(&x, sizeof(PointI)));
    AttResult res{Results::Miss};
    read(socket, buffer(&res, sizeof(AttResult)));
    return res;
};
std::vector<BattleShip> NetPlayer::showAllShips()
{
    std::vector<BattleShip> ships(rules.getTotalShipAmount());
    read(socket, buffer(ships.data(), ships.size() * sizeof(BattleShip)),boost::asio::transfer_all());
    return ships;
};
void NetPlayer::onEnd(std::vector<BattleShip> ships)
{
    write(socket, buffer(ships), boost::asio::transfer_all());
}
std::size_t NetPlayer::getHashGrid()
{
    std::size_t res;
    read(socket, buffer(&res, sizeof(std::size_t)));
    return res;
};
void NetPlayer::returnHashGrid(std::size_t val)
{
    write(socket, buffer(&val, sizeof(std::size_t)));
}
PointI NetPlayer::getMove()
{
    PointI res;
    read(socket, buffer(&res, sizeof(PointI)));
    return res;
};
void NetPlayer::returnResult(AttResult res)
{
    write(socket, buffer(&res, sizeof(AttResult)));
};