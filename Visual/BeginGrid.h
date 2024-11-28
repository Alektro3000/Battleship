#include "Grid.h"

struct VisualBeginGrid : VisualGrid
{
    VisualBeginGrid(GameRules rules);
    std::optional<class BattleShip> getIntersectionShip(PointF point) override;

    void draw(class ID2D1HwndRenderTarget* drawTarget) override;
    void removeShip(BattleShip ship) override;
    void addShip(BattleShip ship) override;
    bool canShipBeAdded(BattleShip ship) override;
private:
    std::vector<std::pair<BattleShip, int> > ships;
};
