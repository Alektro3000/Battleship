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
    PCPlayer(GameRules rules) : playerHits(rules.getSize().x * rules.getSize().y)
    {
        _rules = rules;
        BuildShipLocations();
    }
    Results MakeMove(PointI x) override
    {
        auto damagedShip = std::find_if(ships.begin(),ships.end(), 
                [x](BattleShip y) {return y.IntersectionPosition(x) != -1;});
        if(damagedShip == ships.end())
            return Results::Miss;
        auto i = damagedShip - ships.begin();
        auto damagePos = damagedShip->IntersectionPosition(x);
        if((shipHits[i] & (1<<damagePos)) != 0)
        {
            //Same location shot;
            return (shipHits[i] == ((1<<damagedShip->getLength()) - 1)) ? Results::Destroy : Results::Hit;
        }
        shipHits[i] |= 1<<damagePos;
        return (shipHits[i] == ((1<<damagedShip->getLength()) - 1)) ? Results::Destroy : Results::Hit;
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
            for(int j = 0; j < _rules.getShipAmount(i); j++)
            {
                BattleShip a = {PointI(x, y), i};
                x += i+1;
                if (x + i > _rules.getSize().x)
                {
                    y += 2;
                    x = 0;
                }
                ships.emplace_back(a);
                shipHits.emplace_back(0);
            }
    }
    PointI target{-1};
    PointI GetMove() override;
    void ReturnResult(Results res) override
    {
        playerHits[_rules.flatIndex(target)] = res;
        if(res == Results::Hit)
        {
            std::array corner {PointI{1,1},PointI{-1,1},PointI{-1,-1},PointI{1,-1}};
            for(auto q : corner)
                if(_rules.isValidIndex(target + q))
                    playerHits[_rules.flatIndex(target+q)] = Results::Miss;

        }
        target = {-1};
    }; 
};