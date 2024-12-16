#include "../WinMacros.h"
#include "Player.h"
#include <thread>
#include <numeric>
#include <random>
#include <optional>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>


#ifndef NetPlayerH
#define NetPlayerH

struct NetPlayer final : Player
{
    using contextPtr = std::unique_ptr<boost::asio::io_context>;
private:
    GameRules rules;
    contextPtr context;
    boost::asio::ip::tcp::socket socket;
    
public:
    NetPlayer(GameRules rules, contextPtr&& context, boost::asio::ip::tcp::socket&& socket, bool server);
    AttResult makeMove(PointI x) override;
    std::vector<BattleShip> showAllShips() override;
    void onEnd(std::vector<BattleShip> ships) override;
    std::size_t getHashGrid() override;
    PointI getMove() override;
    void returnResult(AttResult res) override;
    void returnHashGrid(std::size_t val) override;
    void onDetach() override; 
};
#endif