#include "BattleGrid.h"


#ifndef OpponentGridH
#define OpponentGridH


namespace widget
{
    struct OpponentGrid final : BattleGrid
    {
        OpponentGrid(PointI size, int maxShipAmount) : BattleGrid(size), ships(maxShipAmount) {};
        std::optional<BattleShip> getIntersectionShipCoord(PointI point) const override;
        OpponentGrid(OpponentGrid&& other) = default;
        OpponentGrid& operator =(OpponentGrid&& other) = default;

        void removeShip(BattleShip ship) override;
        //Can be used only from one thread
        void addShip(BattleShip ship) override;
        bool canShipBeAdded(BattleShip ship) const override;
        void onRender() override;
        auto begin() const noexcept { return ships.cbegin(); }
        auto end() const noexcept { return ships.cend(); }

    private:
        std::vector<std::atomic<BattleShip> > ships;
        //Can be changed only from one thread
        std::size_t size = 0;
    };
}

#endif
