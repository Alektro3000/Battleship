#include "Grid.h"

#ifndef VisualBattleGridH
#define VisualBattleGridH

namespace widget
{
    struct VisualBattleGrid final : VisualGrid
    {
        VisualBattleGrid(PointI size) : VisualGrid(size) {};
        std::optional<BattleShip> getIntersectionShipCoord(PointI point) const override;

        void removeShip(BattleShip ship) override;
        void addShip(BattleShip ship) override;
        bool canShipBeAdded(BattleShip ship) const override;
        void onRender() override;
        auto begin() const noexcept { return ships.cbegin(); }
        auto end() const noexcept { return ships.cend(); }

    private:
        std::vector<BattleShip> ships;
    };
}
#endif