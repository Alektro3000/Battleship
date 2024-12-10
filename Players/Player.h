#include <vector>
#include "BattleShip.h"
#ifndef PlayerH
#define PlayerH
//getHashGrid
//MakeMove
//getMove/ReturnResult

//showAll
//showDestroyed
struct Player
{
    virtual ~Player() = default;

    virtual std::vector<BattleShip> showAllShips() = 0;
    virtual BattleShip showDestroyedShip() = 0;
    virtual Results makeMove(PointI x) = 0;

    virtual PointI getMove() = 0;
    virtual void returnResult(Results Point) = 0; 

    virtual std::size_t getHashGrid() = 0;

    virtual void onDetach() {}; 
};

#endif