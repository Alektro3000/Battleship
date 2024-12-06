#include "Grid.h"
#include <algorithm>
#include <d2d1.h>

void VisualGrid::DrawShip(BattleShip ship, RectF position) const
{
        GetRenderTarget()->FillRectangle(
            makeD2DRectF(position),
            blackBrush);
}

PointI VisualGrid::getPointCoords(PointF point) const
{
        return PointI((point - getGridPos().low) / getGridSize());
};
RectF VisualGrid::getGridPos() const{
        return position + RectF{getGridSize(),{0,0}};
};
PointF VisualGrid::getCoordPosition(PointI point) const
{
        return point * getGridSize() + getGridPos().low;
};
PointF VisualGrid::getGridSize() const
{
        return position.size() / (size + 1);
}
PointI VisualGrid::getSize() const
{
        return size;
}
RectF VisualGrid::GetShipSnapped(BattleShip ship) const
{
        return GetShipRect(ship) + getGridPos().low;
}
RectF VisualGrid::GetShipRect(BattleShip ship) const
{
        RectF ans = RectF(ship.getRect()) / size * getGridPos().size();
        ans.high = ans.high + getGridPos().size() / size;
        return ans;
}
bool VisualGrid::IsPointIntersect(PointF point, BattleShip ship) const
{
        return GetShipSnapped(ship).isPointInside(point);
}
bool VisualGrid::isShipInsideGrid(BattleShip ship) const
{
        RectI fSize = {0, {size - PointI{1, 1}}};
        return (fSize | ship.getRect()) == fSize;
}

void VisualGrid::onResize(RectF newSize)
{
        Widget::onResize(newSize);
        format = TextFormat(getGridSize().y * 0.8f);
}