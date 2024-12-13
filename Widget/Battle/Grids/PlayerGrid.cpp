#include "PlayerGrid.h"
namespace widget
{

    std::optional<BattleShip> PlayerGrid::getIntersectionShipCoord(PointI point) const
    {
        auto ship = std::find_if(ships.begin(), ships.end(), [point, this](BattleShip ship)
                                 { return ship.IntersectionPosition(point) != -1; });
        if (ship != ships.end())
            return *ship;
        return {};
    }
    void PlayerGrid::removeShip(BattleShip ship)
    {
        ships.erase(std::find(ships.begin(), ships.end(), ship));
    }
    void PlayerGrid::addShip(BattleShip ship)
    {
        ships.push_back(ship);
    }
    bool PlayerGrid::canShipBeAdded(BattleShip shipCopy) const
    {
        auto isValid = isShipInsideGrid(shipCopy);
        isValid = isValid && std::all_of(ships.begin(), ships.end(), [shipCopy, this](BattleShip ship)
                                         { return !ship.hasIntersectionCorner(shipCopy); });
        return isValid;
    }

    void PlayerGrid::onRender()
    {
        BattleGrid::onRender();
        std::for_each(ships.begin(), ships.end(), [this](BattleShip ship)
                      { drawShip(ship, getShipSnapped(ship)); });
        renderShots();
    }
}