#include "BattleGrid.h"
#include <d2d1.h>
#include <algorithm>

std::optional<BattleShip> VisualBattleGrid::getIntersectionShipCoord(PointI point) 
{
    auto ship = std::find_if(ships.begin(), ships.end(), [point, this](BattleShip ship)
                             { return ship.IntersectionPosition(point) != -1; });
    if (ship != ships.end())
        return *ship;
    return {};
}
void VisualBattleGrid::removeShip(BattleShip ship)
{
    ships.erase(std::find(ships.begin(), ships.end(), ship));
}
void VisualBattleGrid::addShip(BattleShip ship)
{
    ships.push_back(ship);
}
bool VisualBattleGrid::canShipBeAdded(BattleShip shipCopy)
{
    auto isValid = isShipInsideGrid(shipCopy);
    isValid = isValid && std::all_of(ships.begin(), ships.end(), [shipCopy, this](BattleShip ship)
                                     { return !ship.hasIntersectionCorner(shipCopy); });
    return isValid;
}
void VisualBattleGrid::onRender()
{
    render.RenderTarget->DrawRectangle(
                    makeD2DRectF(getGridPos()),
                    blackBrush,3);

    
    for(int i = 0; i<size.x;i++)
    {
        WCHAR letter(L'0'+i);
        auto offset = position.low + PointI{1+i,0}*getGridSize();
        render.RenderTarget->DrawText(
                &letter,
                1,
                format,
                makeD2DRectF({offset,offset + getGridSize()}),
                blackBrush
                );
    }
    for(int i = 0; i<size.y;i++)
    {
        WCHAR letter(L'А'+i);
        auto offset = position.low + PointI{0,1+i}*getGridSize();
        render.RenderTarget->DrawText(
                &letter,
                1,
                format,
                makeD2DRectF({offset,offset + getGridSize()}),
                blackBrush
                );
    }

    std::for_each(ships.begin(), ships.end(), [this](BattleShip ship)
                  { DrawShip(ship,  GetShipSnapped(ship)); });
}