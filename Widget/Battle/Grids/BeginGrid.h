#include "Grid.h"

#ifndef BeginGridH
#define BeginGridH

namespace widget
{

    struct BeginGrid final : Grid
    {
        BeginGrid(GameRules rules);
        std::optional<BattleShip> getIntersectionShipCoord(PointI point) const override;

        void removeShip(BattleShip ship) override;
        void addShip(BattleShip ship) override;
        bool canShipBeAdded(BattleShip ship) const override;
        void onRender() override;

        bool isButtonBegin(PointF coords);

        auto begin() noexcept { return ships.begin(); }
        auto end() noexcept { return ships.end(); }
    private:
        std::vector<std::pair<BattleShip, int>> ships;
    };
}
#endif