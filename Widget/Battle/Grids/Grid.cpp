#include "Grid.h"
#include <algorithm>
#include <d2d1.h>

namespace widget
{

    void Grid::drawShip(BattleShip ship, RectF position) const
    {
        constexpr std::array ships{L"1.png", L"2.png", L"3.png", L"4.png"};
        if(ship.getLength() == 0)
            return;
        auto bitmap = Context::getInstance().getBitmapFromFile(ships[ship.getLength() - 1]);
        auto render = Context::getInstance().getRenderTarget();
        render->SetTransform(D2D1::Matrix3x2F::Rotation(-90 * ship.getRotation(),
                                                        makeD2DPointF((position.center()))));
        render->DrawBitmap(bitmap, makeD2DRectF(position.rotated(ship.getRotation())));
        render->SetTransform(D2D1::Matrix3x2F::Identity());
    }

    PointI Grid::getPointCoords(PointF point) const
    {
        return PointI((point - getGridPos().low) / getGridSize());
    };
    RectF Grid::getGridPos() const
    {
        return getPosition() + RectF{getGridSize(), {0, 0}};
    };
    PointF Grid::getCoordPosition(PointI point) const
    {
        return point * getGridSize() + getGridPos().low;
    };
    PointF Grid::getGridSize() const
    {
        return getPosition().size() / (size + 1);
    }
    PointI Grid::getSize() const
    {
        return size;
    }
    RectF Grid::getShipSnapped(BattleShip ship) const
    {
        return getShipRect(ship) + getGridPos().low;
    }
    RectF Grid::getShipRect(BattleShip ship) const
    {
        RectF ans = RectF(ship.getRect()) / size * getGridPos().size();
        ans.high = ans.high + getGridPos().size() / size;
        return ans;
    }
    bool Grid::isPointIntersect(PointF point, BattleShip ship) const
    {
        return getShipSnapped(ship).isPointInside(point);
    }
    bool Grid::isShipInsideGrid(BattleShip ship) const
    {
        RectI fSize = {0, {size - PointI{1, 1}}};
        return (fSize | ship.getRect()) == fSize;
    }

    void Grid::onResize(RectF newSize)
    {
        Widget::onResize(newSize);
        format = TextFormat(getGridSize().y * 0.8f);
    }
    std::optional<BattleShip> Grid::getIntersectionShip(PointF point) const
    {
        return getIntersectionShipCoord(getPointCoords(point));
    }
}