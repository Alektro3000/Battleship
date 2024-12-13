#include "OpponentGrid.h"
namespace widget
{

    std::optional<BattleShip> OpponentGrid::getIntersectionShipCoord(PointI point) const
    {
        auto ship = std::find_if(ships.begin(), ships.end(), [point, this](BattleShip ship)
                                 { return ship.IntersectionPosition(point) != -1; });
        if (ship != ships.end())
            return *ship;
        return {};
    }
    void OpponentGrid::removeShip(BattleShip ship)
    {
        throw std::exception("Can't delete ship from opponent field");
    }
    void OpponentGrid::addShip(BattleShip ship)
    {
        ships[size++] = ship;
    }
    bool OpponentGrid::canShipBeAdded(BattleShip shipCopy) const
    {
        auto isValid = isShipInsideGrid(shipCopy);
        isValid = isValid && std::all_of(ships.begin(), ships.end(), [shipCopy, this](BattleShip ship)
                                         { return !ship.hasIntersectionCorner(shipCopy); });
        return isValid;
    }

    void OpponentGrid::onRender()
    {
        BattleGrid::onRender();
        std::for_each(ships.begin(), ships.end(), [this](BattleShip ship)
                      { drawShip(ship, getShipSnapped(ship)); });
        renderShots();
    }
}