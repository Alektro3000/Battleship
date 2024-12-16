#include "Player.h"
#include <thread>
#include <numeric>
#include <random>
#include <optional>

struct PCPlayer final : Player
{
private:
    GameRules rules;
    std::vector<BattleShip> ships;
    std::vector<unsigned int> shipHits;
    std::vector<Results> playerHits;
    int counter = 0;
    std::mt19937 mt{std::random_device{}()};
    PointI target{-1}; // First Shot
    PointI prevHit{-1};
    bool isWalking = false;
    void buildShipLocations();
    auto findClear(PointI point);
public:
    PCPlayer(GameRules rules) : playerHits(rules.getSize().x * rules.getSize().y), rules(rules)
    {
        buildShipLocations();
    }
    AttResult makeMove(PointI x) override;
    std::vector<BattleShip> showAllShips() override;
    std::size_t getHashGrid() override;
    PointI getMove() override;
    void returnResult(AttResult res) override;
    void returnHashGrid(std::size_t val) override {};
    void onEnd(std::vector<BattleShip>) override {};
};