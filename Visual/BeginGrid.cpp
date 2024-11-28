#include "BeginGrid.h"
#include <d2d1.h>

VisualBeginGrid::VisualBeginGrid(GameRules rules)
{
    int x = 0;
    int y = 0;
    for (int i = 1; i <= rules.getMaxShipLength(); i++)
    {
        BattleShip a = {PointI(x, y), i};
        x += i+1;
        if (x + i > rules.getSize().x)
        {
            y += 2;
            x = 0;
        }
        std::pair<BattleShip, int> Val{a, rules.getShipAmount(i - 1)};
        ships.emplace_back(Val);
    }
};

std::optional<BattleShip> VisualBeginGrid::getIntersectionShip(PointF point)
{
    auto ship = std::find_if(ships.begin(), ships.end(), [point, this](auto ship)
                             { return IsPointIntersect(point, ship.first) && ship.second > 0; });
    if (ship != ships.end())
        return (*ship).first;
    return {};
}
void VisualBeginGrid::removeShip(BattleShip ship)
{
    std::find_if(ships.begin(), ships.end(),  [ship](auto oneOf){
        return oneOf.first.getLength() == ship.getLength();})->second--;
}
void VisualBeginGrid::addShip(BattleShip ship)
{
    std::find_if(ships.begin(), ships.end(), [ship](auto oneOf){
        return oneOf.first.getLength() == ship.getLength();})->second++;
}
void VisualBeginGrid::draw(ID2D1HwndRenderTarget *drawTarget)
{
    ID2D1SolidColorBrush *pBlackBrush = nullptr;
    drawTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &pBlackBrush);
    drawTarget->DrawRectangle(
        makeD2DRectF(position),
        pBlackBrush, 1);
    pBlackBrush->Release();

    std::for_each(ships.begin(), ships.end(), [this, drawTarget](auto ship)
                  { auto a = GetShipSnapped(ship.first);
                    if(a.has_value() && ship.second > 0) DrawShip(ship.first, a.value(), drawTarget); });
}
bool VisualBeginGrid::canShipBeAdded(BattleShip shipCopy)
{
    return true;
}