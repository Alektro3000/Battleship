#include "../../../Players/Player.h"
#include "../../Widget.h"
#include "../../Base.h"
#include <optional>

#ifndef GridH
#define GridH

namespace widget
{
    class Grid : public Widget
    {
    protected:
        PointI size;
        SolidBrush blackBrush{D2D1::ColorF(0,1)};
        TextFormat format = {40};

    public:
        Grid(PointI size) : size(size) {};
        //Convert grid coordinates to global
        PointI getPointCoords(PointF point) const;
        //Convert global coordinates to grid
        PointF getCoordPosition(PointI point) const;
        //Returns size of one cell
        PointF getGridSize() const;
        //Returns position of Grid
        RectF getGridPos() const;
        //Returns size in local coordinates of grid
        PointI getSize() const;

        //Snap ship to grid and return it's position in global coordinates
        RectF getShipSnapped(BattleShip ship) const;
        //Converts ship's position to global coordinates
        RectF getShipRect(BattleShip ship) const;
        //Check if ship contains in local coordinates contain point in Global coordinates
        bool isPointIntersect(PointF point, BattleShip ship) const;
        //Check if ship can be placed in grid
        bool isShipInsideGrid(BattleShip ship) const;

        //Return ship if there is any under selected point in gloval coords
        std::optional<BattleShip> getIntersectionShip(PointF point) const;

        //Return ship if there is any under selected point in local coords
        virtual std::optional<BattleShip> getIntersectionShipCoord(PointI point) const = 0;
        
        virtual void removeShip(BattleShip ship) = 0;
        virtual void addShip(BattleShip ship) = 0;
        virtual bool canShipBeAdded(BattleShip ship) const = 0;

        void onResize(RectF newSize) override;

        //Draws ship in selected global rect
        void drawShip(BattleShip ship, RectF position) const;
    };
}

#endif