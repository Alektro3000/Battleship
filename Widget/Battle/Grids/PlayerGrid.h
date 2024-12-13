#include "BattleGrid.h"


#ifndef PlayerGridH
#define PlayerGridH


namespace widget
{
    struct PlayerGrid final: BattleGrid
    {
        PlayerGrid(PointI size) : BattleGrid(size) {};
        std::optional<BattleShip> getIntersectionShipCoord(PointI point) const override;
        PlayerGrid(PlayerGrid&& other) = default;
        PlayerGrid& operator =(PlayerGrid&& other) = default;

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
