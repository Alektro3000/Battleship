#include "Grid.h"

#ifndef VisualBattleGridH
#define VisualBattleGridH

namespace widget
{
    struct VisualBattleGrid final : VisualGrid
    {
        VisualBattleGrid(PointI size) : VisualGrid(size) , shots(size.x * size.y) {};
        std::optional<BattleShip> getIntersectionShipCoord(PointI point) const override;

        void removeShip(BattleShip ship) override;
        void addShip(BattleShip ship) override;
        bool canShipBeAdded(BattleShip ship) const override;
        void onRender() override;
        auto begin() const noexcept { return ships.cbegin(); }
        auto end() const noexcept { return ships.cend(); }
        std::atomic<Results>& getResult(PointI pos);

    private:
        std::vector<BattleShip> ships;
        std::vector<std::atomic<Results>> shots;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
        SolidBrush redBrush{D2D1::ColorF(D2D1::ColorF::Red)};
        void RenderVal(PointI pos, Results res);
    };
}
#endif