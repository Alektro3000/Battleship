#include "Player.h"

struct PCPlayer : Player
{
    GameRules _rules;
    std::vector<BattleShip> Ship;
    std::vector<int> ShipHits;
    PCPlayer(GameRules rules)
    {
        _rules = rules;
        BuildShipLocations();
    }
    Results MakeMove(PointI x) override
    {
        auto q = std::find_if(Ship.begin(),Ship.end(), 
                [x](BattleShip y) {return y.IntersectionPosition(x) != -1;});
        if(q == Ship.end())
            return Results::Miss;
        auto n = Ship.begin() - q;
        ShipHits[n]++;
        return ShipHits[n] == q->getLength() ? Results::Destroy : Results::Hit;
    }
    virtual std::vector<BattleShip> ShowShips() override
    {
        return Ship;
    }
    void BuildShipLocations()
    {
        for(int i = 0; i<_rules.getMaxShipLength(); i++)
        {
            int am = _rules.getShipAmount(i);
        }
    }
};