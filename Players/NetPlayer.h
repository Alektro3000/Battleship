#include "Player.h"
#include <thread>
#include <numeric>
#include <random>
#include <optional>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>

struct NetPlayer final : Player
{
    using contextPtr = std::unique_ptr<boost::asio::io_context>;
private:
    GameRules _rules;
    boost::asio::ip::tcp::socket socket;
    contextPtr context;
public:
    NetPlayer(GameRules rules, boost::asio::ip::tcp::socket&& socket, contextPtr&& context, bool server);
    AttResult makeMove(PointI x) override;
    std::vector<BattleShip> showAllShips() override;
    std::size_t getHashGrid() override;
    PointI getMove() override;
    void returnResult(AttResult res) override;
    void returnHashGrid(std::size_t val) override {};
};