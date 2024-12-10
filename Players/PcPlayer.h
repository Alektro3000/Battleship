#include "Player.h"
#include <thread>
#include <numeric>
#include <random>
#include <optional>

struct PCPlayer final : Player
{
private:
    GameRules _rules;
    std::vector<BattleShip> ships;
    std::vector<unsigned int> shipHits;
    std::vector<Results> playerHits;
    int counter = 0;
    std::random_device rd;
    std::mt19937 mt{rd()};
    PointI target{1, 2}; // First Shot
    PointI prevHit{-1};
    bool isWalking = false;
    std::optional<BattleShip> lastDestroyed;
    void buildShipLocations();
public:
    PCPlayer(GameRules rules) : playerHits(rules.getSize().x * rules.getSize().y)
    {
        _rules = rules;
        buildShipLocations();
    }
    Results makeMove(PointI x) override
    {
        auto damagedShip = std::find_if(ships.begin(), ships.end(),
                                        [x](BattleShip y)
                                        { return y.IntersectionPosition(x) != -1; });
        if (damagedShip == ships.end())
            return Results::Miss;
        auto i = damagedShip - ships.begin();
        shipHits[i] |= damagedShip->getHitMask(x);
        if(damagedShip->isDestroyed(shipHits[i]))
            lastDestroyed = *damagedShip;
        return damagedShip->isDestroyedRes(shipHits[i]);
    }
    std::vector<BattleShip> showAllShips() override;
    BattleShip showDestroyedShip() override;
    std::size_t getHashGrid() override;
    PointI getMove() override;
    void returnResult(Results res) override;
};