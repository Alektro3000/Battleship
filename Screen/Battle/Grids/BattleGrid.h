#include "Grid.h"

#ifndef VisualBattleGridH
#define VisualBattleGridH

struct VisualBattleGrid : VisualGrid
{
    VisualBattleGrid(PointI size) : VisualGrid(size) {};
    std::optional<BattleShip> getIntersectionShipCoord(PointI point) override;

    void removeShip(BattleShip ship) override;
    void addShip(BattleShip ship) override;
    bool canShipBeAdded(BattleShip ship) override;
    void onRender()  override;
    
private:
    std::vector<BattleShip> ships;
};
#endif