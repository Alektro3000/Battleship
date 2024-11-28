#include "../Visual/BeginGrid.h"
#include "../Visual/BattleGrid.h"
#include "Screen.h"

#ifndef BattleAppH
#define BattleAppH

struct DragAndDrop  
{
    bool isDown = false;
    BattleShip grabbedShip;
    BattleShip beginShip;
    sVisualGrid grabbedShipGrid;
    PointF beginPoint;
    PointI grabOffset;
    void OnClickDown(PointF point, sVisualGrid grid)
    {
        auto ship = grid->getIntersectionShip(point);
        if(ship.has_value())
        {
            grabbedShip = *ship;
            grid->removeShip(grabbedShip);
            grabOffset = grabbedShip.getPoint() - grid->getPointCoords(point) ;
            beginShip = grabbedShip;
            grabbedShipGrid = grid;
            isDown = true;
            beginPoint = point;
        }
    }
    void OnClickUp(PointF point, sVisualGrid grid)
    {
        if(!isDown)
            return;

        PointI offset = grid->getPointCoords(point) ;
        PointI shipPoint =  offset;
        auto shipCopy = grabbedShip;
        shipCopy.setPoint(shipPoint+grabOffset);
        
        if(grid->canShipBeAdded(shipCopy))
        {
            grabbedShip = shipCopy;
            grid->addShip(grabbedShip);
        }
        else
            grabbedShipGrid->addShip(beginShip);
        
        isDown = false;

    }
    void OnUpdate(PointF point, ID2D1HwndRenderTarget* drawTarget)
    {
        if(!isDown)
            return;
        auto rect = grabbedShipGrid->GetShipRect(grabbedShip);
        rect = rect + grabbedShipGrid->position.low;
        DrawShip(grabbedShip,rect + point - beginPoint,drawTarget);
    }
    void OnRotate()
    {
        if(!isDown)
            return;
        grabbedShip.setRotation(grabbedShip.getRotation() + Rotation::Up);
        auto rot = grabOffset.rotated(Rotation::Up);
        beginPoint = beginPoint + (grabOffset - rot) * grabbedShipGrid->getGridSize();
        grabOffset = rot;
    }
};

class BattleApp : public Screen
{
    std::shared_ptr<VisualBattleGrid> gridPlayer;
    std::shared_ptr<VisualBeginGrid> gridPlayerBegin;
    std::shared_ptr<VisualBattleGrid> gridOpponent;
    GameRules rules;
    std::unique_ptr<Player> opponent;

    DragAndDrop operation;
    PointF gridSize;
    ID2D1SolidColorBrush* grayBrush = nullptr;
public:
    BattleApp(GameRules nRules, std::unique_ptr<Player> nOpponent) : opponent(std::move(nOpponent)), rules(nRules)
    {
        gridPlayer = std::make_shared<VisualBattleGrid>();
        gridPlayerBegin = std::make_shared<VisualBeginGrid>(rules);
        gridOpponent = std::make_shared<VisualBattleGrid>();
    };
    
    void onClick(Button button) override {
        if(button == Button::right)
        {
            operation.OnRotate();
            return;
        }
        if(gridPlayer->position.isPointInside(getCursor()))
            operation.OnClickDown(getCursor(), gridPlayer);
        else
            operation.OnClickDown(getCursor(), gridPlayerBegin);
    };
    void onClickUp(Button button) override
    {
        if(button != Button::left)
            return;
        if(gridPlayer->position.isPointInside(getCursor()))
            operation.OnClickUp(getCursor(),gridPlayer);
        else
            operation.OnClickUp(getCursor(),gridPlayerBegin);

    };
    void onResize()  override
    {
        
        if(grayBrush != nullptr)
            grayBrush->Release();

        Screen::onResize();

        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightGray),
            &grayBrush);


        gridSize = size.x / 30;
        auto grid1Offset = gridSize * 2;
        gridPlayer->size = rules.getSize();
        gridPlayer->position = {grid1Offset, grid1Offset + gridSize*gridPlayer->size};

        auto grid2Offset = gridSize * 2;
        grid2Offset.x = size.x - grid2Offset.x - gridSize.x * rules.getSize().x;
        gridOpponent->size = rules.getSize();
        gridOpponent->position = {grid2Offset, grid2Offset + gridSize*gridPlayer->size};

        gridPlayerBegin->size = {10,3};
        auto gridBeginOffset = gridSize * Point{2,13};
        gridPlayerBegin->position = {gridBeginOffset, gridBeginOffset + gridSize*gridPlayerBegin->size};

    }
    // this is the function used to render a single frame
    void onRender() override
    {
        pRT->BeginDraw();

        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

        for(int i = 0; i < size.x/gridSize.x; i++)
            pRT->DrawLine(D2D1::Point2F(i*gridSize.x,0),D2D1::Point2F(i*gridSize.x,size.y),grayBrush);
            
        for(int i = 0; i < size.y/gridSize.y; i++)
            pRT->DrawLine(D2D1::Point2F(0,i*gridSize.y),D2D1::Point2F(size.x,i*gridSize.y),grayBrush);

        gridPlayer->draw(pRT);
        gridOpponent->draw(pRT);

        gridPlayerBegin->draw(pRT);

        {
            operation.OnUpdate(getCursor(),pRT);
        }
        HRESULT hr = pRT->EndDraw();
    }
    ~BattleApp() override {
        if(grayBrush != nullptr)
            grayBrush->Release();
    };
};
#endif