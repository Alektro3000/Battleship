#include "SelectWidget.h"
#include "BattleWidget.h"
#include <thread>

void SelectWidget::onClick(Button button)
{
    if (button == Button::right)
    {
        operation.OnRotate();
        return;
    }
    if (getPlayerGrid().getGridPos().isPointInside(getCursor()))
            operation.OnClickDown(getCursor(), getPlayerGrid());
    else if (getPlayerStartGrid().getGridPos().isPointInside(getCursor()))
    {
        if(operation.OnClickDown(getCursor(), getPlayerStartGrid()))
            ChangeWidget(std::make_unique<BattleWidget>(rules,
                 std::move(opponent),
                 std::move(getPlayerGrid()),
                 std::move(getOpponentGrid()),
                 isPlayerTurn));
        
    }
    return;
    
};

void SelectWidget::onClickUp(Button button)
{
    if (button != Button::left)
        return;
    if (getPlayerGrid().getGridPos().isPointInside(getCursor()))
        operation.OnClickUp(getCursor(), getPlayerGrid());
    else if (getPlayerStartGrid().getGridPos().isPointInside(getCursor()))
        operation.OnClickUp(getCursor(), getPlayerStartGrid());
};

void SelectWidget::RenderVal(PointI pos, VisualBattleGrid& grid, Results res)
{
    if(res != Results::Clear)
    {
        if(res == Results::Hit)
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        if(res == Results::Destroy)
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
        GetRenderTarget()->FillEllipse(D2D1::Ellipse(makeD2DPointF(grid.getCoordPosition(pos) + grid.getGridSize()/2),10,10),grayBrush);
    
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
    }
}

void SelectWidget::onRender()
{
    GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (int i = 0; i < position .size().x / gridSize.x; i++)
        GetRenderTarget()->DrawLine(D2D1::Point2F(position.low.x + i * gridSize.x, position.low.y),
                      D2D1::Point2F(position.low.x + i * gridSize.x, position.high.y), grayBrush);

    for (int i = 0; i < position.size().y / gridSize.y; i++)
        GetRenderTarget()->DrawLine(D2D1::Point2F(position.low.x, position.low.y + i * gridSize.y),
                      D2D1::Point2F(position.high.x, position.low.y + i * gridSize.y), grayBrush);

    WidgetOverlay::onRender();

    //GetRenderTarget()->DrawRectangle(makeD2DRectF(getPlayerGrid().getGridPos()),grayBrush,12);
    
    

    {
        operation.OnUpdate(getCursor());
    }
}

void SelectWidget::onResize(RectF newSize)
{
    Widget::onResize(newSize);


    gridSize = position.size().x / 30;
    auto grid1Offset = gridSize * PointI{2,1};
    getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize()+1)});

    auto grid2Offset = gridSize * PointI{2,1};
    grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1+rules.getSize().x);
    getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize()+1)});

    auto gridBeginOffset = gridSize * Point{2, 12};
    getPlayerStartGrid().onResize({gridBeginOffset, gridBeginOffset + gridSize * (getPlayerStartGrid().getSize()+1)});
}

//Shows that player ended selecting ships positions
bool SelectWidget::DragAndDropShip::OnClickDown(PointF point, VisualGrid& grid)
{
    auto ship = grid.getIntersectionShip(point);
    if (ship.has_value())
    {
        grabbedShip = *ship;
        grid.removeShip(grabbedShip);
        grabOffset = grabbedShip.getPoint() - grid.getPointCoords(point);
        beginShip = grabbedShip;
        grabbedShipGrid = &grid;
        isDown = true;
        beginPoint = point;
        return false;
    }
    auto val = dynamic_cast<VisualBeginGrid*>(&grid);
    if(val)
    {
        return val->isButtonBegin(point);
    }
    return false;
}

void SelectWidget::DragAndDropShip::OnClickUp(PointF point, VisualGrid& grid)
{
    if (!isDown)
        return;

    PointI offset = grid.getPointCoords(point);
    PointI shipPoint = offset;
    auto shipCopy = grabbedShip;
    shipCopy.setPoint(shipPoint + grabOffset);

    if (grid.canShipBeAdded(shipCopy))
    {
        grabbedShip = shipCopy;
        grid.addShip(grabbedShip);
    }
    else
        grabbedShipGrid->addShip(beginShip);

    isDown = false;
}

void SelectWidget::DragAndDropShip::OnUpdate(PointF point)
{
    if (!isDown)
        return;
    auto rect = grabbedShipGrid->GetShipRect(grabbedShip);
    rect = rect + grabbedShipGrid->getGridPos().low;
    grabbedShipGrid->DrawShip(grabbedShip, rect + point - beginPoint);
}

void SelectWidget::DragAndDropShip::OnRotate()
{
    if (!isDown)
        return;
    grabbedShip.setRotation(grabbedShip.getRotation() + Rotation::Up);
    auto rot = grabOffset.rotated(Rotation::Up);
    beginPoint = beginPoint + (grabOffset - rot) * grabbedShipGrid->getGridSize();
    grabOffset = rot;
}