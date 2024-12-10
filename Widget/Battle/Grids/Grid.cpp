#include "Grid.h"
#include <algorithm>
#include <d2d1.h>

namespace widget
{

        void VisualGrid::DrawShip(BattleShip ship, RectF position) const
        {
                constexpr std::array ships{L"1.png", L"2.png", L"3.png", L"4.png"};
                auto bitmap = Context::getInstance().getBitmapFromFile(ships[ship.getLength() - 1]);
                auto render = Context::getInstance().getRenderTarget();
                render->SetTransform(D2D1::Matrix3x2F::Rotation(-90 * ship.getRotation(),
                                                                makeD2DPointF((position.center()))));
                render->DrawBitmap(bitmap, makeD2DRectF(position.rotated(ship.getRotation())));
                render->SetTransform(D2D1::Matrix3x2F::Identity());
        }

        PointI VisualGrid::getPointCoords(PointF point) const
        {
                return PointI((point - getGridPos().low) / getGridSize());
        };
        RectF VisualGrid::getGridPos() const
        {
                return position + RectF{getGridSize(), {0, 0}};
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
}