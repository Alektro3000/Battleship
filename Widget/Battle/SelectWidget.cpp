#include "SelectWidget.h"
#include "BattleWidget.h"
#include <thread>

namespace widget
{

    void SelectWidget::onClick(MouseButton button)
    {
        auto cursor = Context::getInstance().getCursor();
        if (button == MouseButton::right)
        {
            operation.OnRotate(cursor);
            return;
        }
        if (getPlayerGrid().getGridPos().isPointInside(cursor))
            operation.OnClickDown(cursor, getPlayerGrid());
        else if (getPlayerStartGrid().getGridPos().isPointInside(cursor))
        {
            if (operation.OnClickDown(cursor, getPlayerStartGrid()))
                ChangeWidget(std::make_unique<BattleWidget>(rules,
                                                            std::move(opponent),
                                                            std::move(getPlayerGrid()),
                                                            std::move(getOpponentGrid()),
                                                            isPlayerTurn),
                             false);
        }
        return;
    };

    void SelectWidget::onClickUp(MouseButton button)
    {
        if (button != MouseButton::left)
            return;
        if (getPlayerGrid().getGridPos().isPointInside(Context::getInstance().getCursor()))
            operation.OnClickUp(Context::getInstance().getCursor(), getPlayerGrid());
        else if (getPlayerStartGrid().getGridPos().isPointInside(Context::getInstance().getCursor()))
            operation.OnClickUp(Context::getInstance().getCursor(), getPlayerStartGrid());
        else
            operation.OnClickUp(Context::getInstance().getCursor());
    };

    void SelectWidget::RenderVal(PointI pos, VisualBattleGrid &grid, Results res)
    {
        if (res != Results::Clear)
        {
            if (res == Results::Hit)
                grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            if (res == Results::Destroy)
                grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
            Context::getInstance().getRenderTarget()->FillEllipse(D2D1::Ellipse(makeD2DPointF(grid.getCoordPosition(pos) + grid.getGridSize() / 2), 10, 10), grayBrush);

            grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
        }
    }

    void SelectWidget::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

        for (int i = 0; i < position.size().x / gridSize.x; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(position.low.x + i * gridSize.x, position.low.y),
                                                               D2D1::Point2F(position.low.x + i * gridSize.x, position.high.y), grayBrush);

        for (int i = 0; i < position.size().y / gridSize.y; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(position.low.x, position.low.y + i * gridSize.y),
                                                               D2D1::Point2F(position.high.x, position.low.y + i * gridSize.y), grayBrush);

        Overlay::onRender();

        // Context::getInstance().getRenderTarget()->DrawRectangle(makeD2DRectF(getPlayerGrid().getGridPos()),grayBrush,12);

        {
            operation.OnUpdate(Context::getInstance().getCursor());
        }
    }

    void SelectWidget::onResize(RectF newSize)
    {
        Widget::onResize(newSize);

        gridSize = position.size().x / 30;
        auto grid1Offset = gridSize * PointI{2, 1};
        getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize() + 1)});

        auto grid2Offset = gridSize * PointI{2, 1};
        grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1 + rules.getSize().x);
        getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize() + 1)});

        auto gridBeginOffset = gridSize * Point{2, 12};
        getPlayerStartGrid().onResize({gridBeginOffset, gridBeginOffset + gridSize * (getPlayerStartGrid().getSize() + 1)});
    }

    // Shows that player ended selecting ships positions
    bool SelectWidget::DragAndDropShip::OnClickDown(PointF point, VisualGrid &grid)
    {
        auto ship = grid.getIntersectionShip(point);
        if (ship.has_value())
        {
            grabbedShip = *ship;
            grid.removeShip(grabbedShip);
            beginPoint = point;
            grabOffset = grid.getCoordPosition(grabbedShip.getPoint()) - point;
            beginShip = grabbedShip;
            grabbedShipGrid = &grid;
            isDown = true;
            isMovedAway = false;
            return false;
        }
        auto val = dynamic_cast<VisualBeginGrid *>(&grid);
        if (val)
            return val->isButtonBegin(point);

        return false;
    }

    void SelectWidget::DragAndDropShip::OnClickUp(PointF point, VisualGrid &grid)
    {
        if (!isDown)
            return;

        if (!isMovedAway)
            OnRotate(point);

        auto shipCopy = grabbedShip;
        shipCopy.setPoint(grid.getPointCoords(grabOffset + point + grid.getGridSize() * 0.5f));

        if (grid.canShipBeAdded(shipCopy))
        {
            grabbedShip = shipCopy;
            grid.addShip(grabbedShip);
        }
        else
            grabbedShipGrid->addShip(beginShip);

        isDown = false;
    }

    void SelectWidget::DragAndDropShip::OnClickUp(PointF point)
    {
        if (!isDown)
            return;
        grabbedShipGrid->addShip(beginShip);

        isDown = false;
    }

    void SelectWidget::DragAndDropShip::OnUpdate(PointF point)
    {
        if (!isDown)
            return;
        if (!RectF{point - 2, point + 2}.isPointInside(beginPoint))
            isMovedAway = true;

        auto rect = grabbedShipGrid->GetShipRect(grabbedShip) + grabbedShipGrid->getGridPos().low;

        grabbedShipGrid->DrawShip(grabbedShip, rect + point - beginPoint);
    }

    void SelectWidget::DragAndDropShip::OnRotate(PointF point)
    {
        if (!isDown)
            return;
        isMovedAway = true;
        grabbedShip.setRotation(grabbedShip.getRotation() + Rotation::Up);

        auto rot = grabOffset.rotated(Rotation::Up, -grabbedShipGrid->getGridSize() * 0.5f);
        beginPoint = beginPoint - (rot - grabOffset);
        grabOffset = rot;

        // beginPoint = (beginPoint - grabOffset).rotated(Rotation::Up) + grabOffset;
    }
}