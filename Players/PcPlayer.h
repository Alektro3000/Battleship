#include "Player.h"

struct PCPlayer : IPlayer
{
    GameRules _rules;
    std::vector<BattleShip> Ship;
    std::vector<int> ShipHits;
    void Init(GameRules rules) override
    {
        _rules = rules;
        BuildShipLocations();
    }
    void SetBeginPosition(std::vector<BattleShip> Ships) override
    {
        Ship = Ships;
        std::fill(ShipHits.begin(),ShipHits.end(),0);
        ShipHits.resize(Ship.size(),0);
    }
    Results MakeMove(Point x) override
    {
        auto q = std::find_if(Ship.begin(),Ship.end(), 
                [x](BattleShip y) {return y.IntersectionPosition(x) != -1;});
        if(q == Ship.end())
            return Results::Miss;
        auto n = Ship.begin() - q;
        ShipHits[n]++;
        return ShipHits[n] == q->getLength() ? Results::Destroy : Results::Hit;
    }
    void BuildShipLocations()
    {
        for(int i = 0; i<_rules.getMaxShipLength(); i++)
        {
            int am = _rules.getShipAmount(i);
        }
    }
};