#include "../../../Players/Player.h"
#include "../../Screen.h"
#include "../../Base.h"
#include <optional>

#ifndef VisualGridH
#define VisualGridH

class VisualGrid : public Screen
{
protected:
    PointI size;
    SolidBrush blackBrush;
    TextFormat format; 
public:
    VisualGrid(PointI size) : size(size) {};
    PointI getPointCoords(PointF point) const;
    PointF getCoordPosition(PointI point) const;
    PointF getGridSize() const;
    RectF getGridPos() const;
    PointI getSize() const;

    RectF GetShipSnapped(BattleShip ship) const;
    RectF GetShipRect(BattleShip ship) const;
    bool IsPointIntersect(PointF point, BattleShip ship) const;
    bool isShipInsideGrid(BattleShip ship) const;

    virtual std::optional<BattleShip> getIntersectionShipCoord(PointI point) const = 0;    
    std::optional<BattleShip> getIntersectionShip(PointF point) const
    {
        return getIntersectionShipCoord(getPointCoords(point));
    }
    virtual void removeShip(BattleShip ship) = 0;
    virtual void addShip(BattleShip ship) = 0;
    virtual bool canShipBeAdded(BattleShip ship) const = 0;
    
    void onResize(RectF newSize) override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        blackBrush = SolidBrush(render.RenderTarget,D2D1::ColorF(D2D1::ColorF::Black));
    };
    
    void DrawShip(BattleShip ship, RectF position) const;
};


#endif