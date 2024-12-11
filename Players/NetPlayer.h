#include "Player.h"
#include <thread>
#include <numeric>
#include <random>
#include <optional>
#include <boost/asio/ip/address_v4.hpp>

struct NetPlayer final : Player
{
private:
    GameRules _rules;
public:
    NetPlayer(GameRules rules, boost::asio::ip::address_v4 ip, bool server);
    AttResult makeMove(PointI x) override;
    std::vector<BattleShip> showAllShips() override;
    std::size_t getHashGrid() override;
    PointI getMove() override;
    void returnResult(AttResult res) override;
    void returnHashGrid(std::size_t val) override {};
};