#include <vector>
#include "BattleShip.h"
#ifndef PlayerH
#define PlayerH
//getHashGrid / HashGridResponse

//loop
//  MakeMove
//  getMove/ReturnResult
//  if -> ShowDestroyed

//showAll


struct Player
{
    virtual ~Player() = default;

    virtual std::size_t getHashGrid() = 0;
    virtual void returnHashGrid(std::size_t val) = 0;

    virtual AttResult makeMove(PointI x) = 0;

    virtual PointI getMove() = 0;
    virtual void returnResult(AttResult Point) = 0; 

    virtual std::vector<BattleShip> showAllShips() = 0;
    virtual void onEnd() {};
    virtual void onDetach() {}; 
};

#endif