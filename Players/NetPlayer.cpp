#include "NetPlayer.h"

NetPlayer::NetPlayer(GameRules rules, boost::asio::ip::address_v4 ip, bool server) {

};
AttResult NetPlayer::makeMove(PointI x) {
    return AttResult{Results::Miss};
};
std::vector<BattleShip> NetPlayer::showAllShips() {
    return {};
};
std::size_t NetPlayer::getHashGrid() {
    return 0;

};
PointI NetPlayer::getMove() {
    return {};
};
void NetPlayer::returnResult(AttResult res) {

};