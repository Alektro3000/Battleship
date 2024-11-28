#include "BattleGrid.h"
#include <d2d1.h>

std::optional<BattleShip> VisualBattleGrid::getIntersectionShip(PointF point) 
{
    auto ship = std::find_if(ships.begin(), ships.end(), [point, this](BattleShip ship)
                             { return IsPointIntersect(point, ship); });
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
void VisualBattleGrid::draw(ID2D1HwndRenderTarget* drawTarget)
{
    ID2D1SolidColorBrush *pBlackBrush = nullptr;
    drawTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush);
    drawTarget->DrawRectangle(
                    makeD2DRectF(position),
                    pBlackBrush,3);
    pBlackBrush->Release();

    std::for_each(ships.begin(), ships.end(), [this, drawTarget](BattleShip ship)
                  { auto a = GetShipSnapped(ship);
                    if(a.has_value()) DrawShip(ship, a.value(), drawTarget); });
}
bool VisualBattleGrid::canShipBeAdded(BattleShip shipCopy)
{
    auto isValid = isShipInsideGrid(shipCopy);
    isValid = isValid && std::all_of(ships.begin(), ships.end(), [shipCopy, this](BattleShip ship)
                                     { return !ship.hasIntersectionCorner(shipCopy); });
    return isValid;
}