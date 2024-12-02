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
    void BuildShipLocations()
    {
        int x = 0;
        int y = 0;
        for (int i = 1; i <= _rules.getMaxShipLength(); i++)
            for (int j = 0; j < _rules.getShipAmount(i); j++)
            {
                BattleShip a = {PointI(x, y), i};
                x += i + 1;
                if (x + i > _rules.getSize().x)
                {
                    y += 2;
                    x = 0;
                }
                ships.emplace_back(a);
                shipHits.emplace_back(0);
            }
    }
    PointI GetMove() override;
    void ReturnResult(Results res) override
    {
        playerHits[_rules.flatIndex(target)] = res;
        std::array adj{PointI{1, 0}, PointI{0, 1}, PointI{-1, 0}, PointI{0, -1}};
            std::array corner{PointI{1, 1}, PointI{-1, 1}, PointI{-1, -1}, PointI{1, -1}};
        if (res == Results::Hit)
        {
            for (auto q : corner)
                if (_rules.isValidIndex(target + q))
                    playerHits[_rules.flatIndex(target + q)] = Results::Miss;
            if (prevHit != PointI{-1})
            {
                if (_rules.isValidIndex(target + (target - prevHit).sgn()))
                {
                    target = target + (target - prevHit).sgn();

                    isWalking = true;
                }
                else
                    target = prevHit + (prevHit - target).sgn();
            }
            else
            {
                prevHit = target;
                for (auto q : adj)
                    if (_rules.isValidIndex(target + q) &&
                        playerHits[_rules.flatIndex(target + q)] == Results::Clear)
                    {
                        target = target + q;
                        break;
                    }
            }
            return;
        }
        if (res == Results::Miss)
        {
            if (isWalking)
            {
                target = prevHit + (prevHit - target).sgn();
                return;
            }

            if (prevHit != PointI{-1})
            {
                for (auto q : adj)
                    if (_rules.isValidIndex(prevHit + q) &&
                        playerHits[_rules.flatIndex(prevHit + q)] == Results::Clear)
                        target = prevHit + q;
                return;
            }
        }
        if (res == Results::Destroy)
        {
            for (auto q : adj)
                if (_rules.isValidIndex(prevHit + q) &&
                    playerHits[_rules.flatIndex(prevHit + q)] == Results::Clear)
                    playerHits[_rules.flatIndex(prevHit + q)] = Results::Miss;
            prevHit = {-1};
            isWalking = false;

            for (auto q : adj)
                if (_rules.isValidIndex(target + q) &&
                    playerHits[_rules.flatIndex(target + q)] == Results::Clear)
                    playerHits[_rules.flatIndex(target + q)] = Results::Miss;
            for (auto q : corner)
                if (_rules.isValidIndex(target + q))
                    playerHits[_rules.flatIndex(target + q)] = Results::Miss;
        }
        target = {-1};
    };
};