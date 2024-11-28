#include "../Players/Player.h"
#include "Base.h"
#include <optional>

#ifndef VisualGridH
#define VisualGridH

struct VisualGrid : std::enable_shared_from_this<VisualGrid>
{
    RectF position;
    PointI size;
    PointI getPointCoords(PointF point) const
    {
        return PointI((point - position.low) / getGridSize());
    };
    PointF getGridSize() const
    {
        return position.size()/size;
    }
    std::optional<RectF> GetShipSnapped(BattleShip ship)
    {
        return GetShipRect(ship)+position.low;
    }
    RectF GetShipRect(BattleShip ship)
    {
        RectF ans = RectF(ship.getRect()) / size * position.size();
        ans.high = ans.high + position.size() / size ;
        return ans;
    }
    bool IsPointIntersect(PointF point, BattleShip ship)
    {
        auto a = GetShipSnapped(ship);
        if(a)
            return a->isPointInside(point);
        return false;
    }
    bool isShipInsideGrid(BattleShip ship) const
    {
        RectI fSize = {0,{size-PointI{1,1}}};
        return (fSize | ship.getRect()) == fSize;
    }

    virtual std::optional<class BattleShip> getIntersectionShip(PointF point) = 0;
    virtual void draw(class ID2D1HwndRenderTarget* drawTarget) = 0;
    virtual void removeShip(BattleShip ship) = 0;
    virtual void addShip(BattleShip ship) = 0;
    virtual bool canShipBeAdded(BattleShip ship) = 0;
};

void DrawShip(BattleShip ship, RectF position, ID2D1HwndRenderTarget* drawTarget);

using sVisualGrid = std::shared_ptr<VisualGrid>;

#endif