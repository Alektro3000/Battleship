#include "Player.h"
#include <thread>
#include <numeric>
#include <random>

struct PCPlayer : Player
{
    GameRules _rules;
    std::vector<BattleShip> ships;
    std::vector<int> shipHits;
    std::vector<Results> playerHits;
    int counter = 0;
    std::random_device rd;
    std::mt19937 mt{rd()};
    PointI target{1, 2}; // First Shot
    PointI prevHit{-1};
    bool isWalking = false;
    PCPlayer(GameRules rules) : playerHits(rules.getSize().x * rules.getSize().y)
    {
        _rules = rules;
        BuildShipLocations();
    }
    Results MakeMove(PointI x) override
    {
        auto damagedShip = std::find_if(ships.begin(), ships.end(),
                                        [x](BattleShip y)
                                        { return y.IntersectionPosition(x) != -1; });
        if (damagedShip == ships.end())
            return Results::Miss;
        auto i = damagedShip - ships.begin();
        shipHits[i] |= damagedShip->getHitMask(x);
        return damagedShip->isDestroyedRes(shipHits[i]);
    }
    std::vector<BattleShip> ShowShips() override
    {
        return ships;
    }
    void BuildShipLocations();
    PointI GetMove() override;
    void ReturnResult(Results res) override;
};