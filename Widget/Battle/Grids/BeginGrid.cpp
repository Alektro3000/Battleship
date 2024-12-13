#include "BeginGrid.h"
#include <d2d1.h>
#include <algorithm>
#include <numeric>

namespace widget
{

    BeginGrid::BeginGrid(GameRules rules) : Grid({10, 3})
    {
        int x = 0;
        int y = 0;
        for (int i = 1; i <= rules.getMaxShipLength(); i++)
        {
            BattleShip a = {PointI(x, y), i};
            x += i + 1;
            if (x + i > rules.getSize().x)
            {
                y += 2;
                x = 0;
            }
            std::pair<BattleShip, int> Val{a, rules.getShipAmount(i)};
            ships.emplace_back(Val);
        }
    };

    std::optional<BattleShip> BeginGrid::getIntersectionShipCoord(PointI point) const
    {
        auto ship = std::find_if(ships.begin(), ships.end(), [point, this](auto ship)
                                 { return (ship.first.IntersectionPosition(point) != -1) && ship.second > 0; });
        if (ship != ships.end())
            return (*ship).first;
        return {};
    }
    void BeginGrid::removeShip(BattleShip ship)
    {
        std::find_if(ships.begin(), ships.end(), [ship](auto oneOf)
                     { return oneOf.first.getLength() == ship.getLength(); })
            ->second--;
    }
    void BeginGrid::addShip(BattleShip ship)
    {
        std::find_if(ships.begin(), ships.end(), [ship](auto oneOf)
                     { return oneOf.first.getLength() == ship.getLength(); })
            ->second++;
    }
    void BeginGrid::onRender()
    {
        Context::getInstance().getRenderTarget()->DrawRectangle(
            makeD2DRectF(getGridPos()),
            blackBrush, 1);

        int draw = 0;

        std::for_each(ships.begin(), ships.end(), [this, &draw](auto ship)
                      { auto a = getShipSnapped(ship.first);
                    if(ship.second > 0) {drawShip(ship.first, a); draw++;} });
        if (draw == 0)
        {
            WCHAR let[]{L"Закончить выбор?"};
            Context::getInstance().getRenderTarget()->DrawText(
                let,
                sizeof(let) / sizeof(WCHAR),
                format,
                makeD2DRectF(getGridPos()),
                blackBrush);
        }
    }
    bool BeginGrid::canShipBeAdded(BattleShip shipCopy) const
    {
        return true;
    }

    bool BeginGrid::isButtonBegin(PointF coords)
    {
        auto amount = std::accumulate(ships.begin(), ships.end(), 0, [](auto prev, auto cur)
                                      { return prev + cur.second; });

        return amount == 0;
    }
}