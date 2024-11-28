#include "Grid.h"

struct VisualBattleGrid : VisualGrid
{
    std::optional<class BattleShip> getIntersectionShip(PointF point) override;

    void draw(class ID2D1HwndRenderTarget* drawTarget) override;
    void removeShip(BattleShip ship) override;
    void addShip(BattleShip ship) override;
    bool canShipBeAdded(BattleShip ship) override;
    
private:
    std::vector<class BattleShip> ships;
};