#include "SelectScreen.h"
#include "BattleScreen.h"
#include <thread>

void SelectScreen::onClick(Button button)
{
    if (button == Button::right)
    {
        operation.OnRotate();
        return;
    }
    if (gridPlayer->getGridPos().isPointInside(getCursor()))
            operation.OnClickDown(getCursor(), *gridPlayer);
    else if (gridPlayerBegin->getGridPos().isPointInside(getCursor()))
    {
        if(operation.OnClickDown(getCursor(), *gridPlayerBegin))
            changeScreenCallback(std::make_unique<BattleScreen>(rules,
                 std::move(opponent),
                 std::move(gridPlayer),
                 std::move(gridOpponent),
                 isPlayerTurn));
        
    }
    return;
    
};

void SelectScreen::onClickUp(Button button)
{
    if (button != Button::left)
        return;
    if (gridPlayer->getGridPos().isPointInside(getCursor()))
        operation.OnClickUp(getCursor(), *gridPlayer);
    else if (gridPlayerBegin->getGridPos().isPointInside(getCursor()))
        operation.OnClickUp(getCursor(), *gridPlayerBegin);
};

void SelectScreen::RenderVal(PointI pos, VisualBattleGrid& grid, Results res)
{
    if(res != Results::Clear)
    {
        if(res == Results::Hit)
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        if(res == Results::Destroy)
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
        render.RenderTarget->FillEllipse(D2D1::Ellipse(makeD2DPointF(grid.getCoordPosition(pos) + grid.getGridSize()/2),10,10),grayBrush);
    
        grayBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
    }
}

void SelectScreen::onRender()
{
    render.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (int i = 0; i < position .size().x / gridSize.x; i++)
        render.RenderTarget->DrawLine(D2D1::Point2F(position.low.x + i * gridSize.x, position.low.y),
                      D2D1::Point2F(position.low.x + i * gridSize.x, position.high.y), grayBrush);

    for (int i = 0; i < position.size().y / gridSize.y; i++)
        render.RenderTarget->DrawLine(D2D1::Point2F(position.low.x, position.low.y + i * gridSize.y),
                      D2D1::Point2F(position.high.x, position.low.y + i * gridSize.y), grayBrush);

    gridPlayer->onRender();
    gridOpponent->onRender();
    
    gridPlayerBegin->onRender();

    {
        operation.OnUpdate(getCursor(), render);
    }
}

void SelectScreen::onResize(RectF newSize)
{

    Screen::onResize(newSize);

    grayBrush = SolidBrush(render.RenderTarget, D2D1::ColorF(D2D1::ColorF::LightGray));

    gridSize = position.size().x / 30;
    auto grid1Offset = gridSize * PointI{2,1};
    gridPlayer->onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize()+1)});

    auto grid2Offset = gridSize * PointI{2,1};
    grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1+rules.getSize().x);
    gridOpponent->onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize()+1)});

    auto gridBeginOffset = gridSize * Point{2, 12};
    gridPlayerBegin->onResize({gridBeginOffset, gridBeginOffset + gridSize * (gridPlayerBegin->getSize()+1)});
}

//Shows that player ended selecting ships positions
bool SelectScreen::DragAndDropShip::OnClickDown(PointF point, VisualGrid& grid)
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

void SelectScreen::DragAndDropShip::OnClickUp(PointF point, VisualGrid& grid)
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

void SelectScreen::DragAndDropShip::OnUpdate(PointF point, RenderThings drawTarget)
{
    if (!isDown)
        return;
    auto rect = grabbedShipGrid->GetShipRect(grabbedShip);
    rect = rect + grabbedShipGrid->getGridPos().low;
    grabbedShipGrid->DrawShip(grabbedShip, rect + point - beginPoint);
}

void SelectScreen::DragAndDropShip::OnRotate()
{
    if (!isDown)
        return;
    grabbedShip.setRotation(grabbedShip.getRotation() + Rotation::Up);
    auto rot = grabOffset.rotated(Rotation::Up);
    beginPoint = beginPoint + (grabOffset - rot) * grabbedShipGrid->getGridSize();
    grabOffset = rot;
}