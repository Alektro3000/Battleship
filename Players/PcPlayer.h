#include "Player.h"

struct PCPlayer : IPlayer
{
    
    void Init(GameRules rules) override
    {
        
    }
    void SetBeginPosition(std::vector<BattleShip> Ships) override
    {

    }
    Results MakeMove(Point x) override
    {
        return Results::Miss;
    }
};