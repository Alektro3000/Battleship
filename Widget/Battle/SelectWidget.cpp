#include "SelectWidget.h"
#include "BattleWidget.h"
#include "../Base/Builder.h"
#include <thread>

namespace widget
{
    SelectWidget::SelectWidget(GameRules nRules,
                               std::unique_ptr<Player> nOpponent,
                               bool isPlayerTurn) : Overlay<PlayerGrid,
                                                    BeginGrid,
                                                    OpponentGrid,
                                                    Button<Padder<TextBox>>>(PlayerGrid{nRules.getSize()},
                                                            BeginGrid{nRules}, 
                                                            OpponentGrid{nRules.getSize(), nRules.getTotalShipAmount()},
                                                            Builder::makeText(L"Авто")
                                                                .addPadding()
                                                                .setBorder()
                                                                .addButton([this](auto _)
                                                                           { autoSelect(); })
                                                                .build()),
                                                    isPlayerTurn(isPlayerTurn),
                                                    opponent(std::move(nOpponent)), rules(nRules) {
                                                    };
    void SelectWidget::autoSelect()
    {
        for (auto &q : getPlayerStartGrid())
        {
            for (int ship = 0; ship < q.second; ship++)
            {
                bool fl = true;
                for (int i = 0; i < rules.getSize().x && fl; i++)
                    for (int j = 0; j <= rules.getSize().y - q.first.getLength() && fl; j++)
                    {
                        auto locationShip = BattleShip(PointI(i, j), q.first.getLength());
                        if (getPlayerGrid().canShipBeAdded(locationShip))
                        {
                            getPlayerGrid().addShip(locationShip);
                            q.second--;
                            if (q.second <= 0)
                                fl = false;
                        }
                    }
            }
        }
    }
    void SelectWidget::onClickDown(MouseButton button)
    {
        auto cursor = Context::getInstance().getCursor();
        getAutoButton().tryClickDown(button);
        if (button == MouseButton::right)
        {
            operation.OnRotate(cursor);
            return;
        }
        if (getPlayerGrid().getGridPos().isPointInside(cursor))
            operation.onClickDownDown(cursor, getPlayerGrid());
        else if (getPlayerStartGrid().getGridPos().isPointInside(cursor))
        {
            if (operation.onClickDownDown(cursor, getPlayerStartGrid()))
                pushWidget(std::make_unique<BattleWidget>(rules,
                                                          std::move(opponent),
                                                          std::move(getPlayerGrid()),
                                                          isPlayerTurn),
                           false);
        }
        return;
    };

    void SelectWidget::onClickUp(MouseButton button)
    {
        if (button != MouseButton::left)
            return;
        auto cursor = Context::getInstance().getCursor();
        if (getPlayerGrid().getGridPos().isPointInside(cursor))
            operation.onClickUp(cursor, getPlayerGrid());
        else if (getPlayerStartGrid().getGridPos().isPointInside(cursor))
            operation.onClickUp(cursor, getPlayerStartGrid());
        else
            operation.onClickUp(cursor);
    };

    void SelectWidget::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

        for (int i = 0; i < getPosition().size().x / gridSize.x; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(getPosition().low.x + i * gridSize.x, getPosition().low.y),
                                                               D2D1::Point2F(getPosition().low.x + i * gridSize.x, getPosition().high.y), grayBrush);

        for (int i = 0; i < getPosition().size().y / gridSize.y; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(getPosition().low.x, getPosition().low.y + i * gridSize.y),
                                                               D2D1::Point2F(getPosition().high.x, getPosition().low.y + i * gridSize.y), grayBrush);

        Overlay::onRender();

        // Context::getInstance().getRenderTarget()->DrawRectangle(makeD2DRectF(getPlayerGrid().getGridPos()),grayBrush,12);

        {
            operation.OnUpdate(Context::getInstance().getCursor());
        }
    }

    void SelectWidget::onResize(RectF newSize)
    {
        Widget::onResize(newSize);

        gridSize = getPosition().size().x / 30;
        auto grid1Offset = gridSize * PointI{2, 1};
        getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize() + 1)});

        auto grid2Offset = gridSize * PointI{2, 1};
        grid2Offset.x = getPosition().size().x - grid2Offset.x - gridSize.x * (1 + rules.getSize().x);
        getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize() + 1)});

        auto gridBeginOffset = gridSize * Point{2, 12};
        getPlayerStartGrid().onResize({gridBeginOffset, gridBeginOffset + gridSize * (getPlayerStartGrid().getSize() + 1)});

        auto autoBeginOffset = gridSize * Point{2, 10};
        getAutoButton().onResize({gridBeginOffset, gridBeginOffset + gridSize * PointI{3, 1}});
    }

    // Shows that player ended selecting ships positions
    bool SelectWidget::DragAndDropShip::onClickDownDown(PointF point, Grid &grid)
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
        auto val = dynamic_cast<BeginGrid *>(&grid);
        if (val)
            return val->isButtonBegin(point);

        return false;
    }

    void SelectWidget::DragAndDropShip::onClickUp(PointF point, Grid &grid)
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

    void SelectWidget::DragAndDropShip::onClickUp(PointF point)
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

        auto rect = grabbedShipGrid->getShipRect(grabbedShip) + grabbedShipGrid->getGridPos().low;

#if 0
        Context::getInstance().getRenderTarget()->DrawEllipse(
           D2D1::Ellipse(makeD2DPointF(beginPoint),10,10), SolidBrush(D2D1::ColorF(0,1)) 
        );
        Context::getInstance().getRenderTarget()->DrawEllipse(
           D2D1::Ellipse(makeD2DPointF(point+grabOffset),10,10), SolidBrush(D2D1::ColorF(0,1)) 
        );
#endif
        grabbedShipGrid->drawShip(grabbedShip, rect + point - beginPoint);
    }

    void SelectWidget::DragAndDropShip::OnRotate(PointF point)
    {
        if (!isDown)
            return;
        isMovedAway = true;
        grabbedShip.setRotation(grabbedShip.getRotation() + Rotation::Down);

        auto rot = grabOffset.rotated(Rotation::Up, -grabbedShipGrid->getGridSize() * 0.5f);
        beginPoint = beginPoint - (rot - grabOffset);
        grabOffset = rot;
    }
}