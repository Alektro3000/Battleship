#include "BattleScreen.h"
#include <thread>

void BattleScreen::onClick(Button button)
{
    if (isPlayerTurn && gridOpponent->getGridPos().isPointInsideExcl(getCursor()))
    {
        isPlayerTurn = false;
        auto p = gridOpponent->getPointCoords(getCursor());
        makingMove = std::jthread([this,p](){
            opponentShots[p.x*rules.getSize().y+p.y] = opponent->MakeMove(p);
            PointI shot = opponent->GetMove();
            bool hadVal = gridPlayer->getIntersectionShipCoord(shot).has_value();
            auto result = hadVal ? Results::Hit : Results::Miss;
            playerShots[shot.x*rules.getSize().y+shot.y] = result;
            opponent->ReturnResult(result);
            isPlayerTurn = true;
        });
    }
};

void BattleScreen::onClickUp(Button button)
{

};

void BattleScreen::RenderVal(PointI pos, VisualBattleGrid& grid, Results res)
{
    if(res != Results::Clear)
    {
        if(res == Results::Hit)
        grayBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        if(res == Results::Destroy)
        grayBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
        render.RenderTarget->FillEllipse(D2D1::Ellipse(makeD2DPointF(grid.getCoordPosition(pos) + grid.getGridSize()/2),10,10),grayBrush);
    
        grayBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
    }
}

void BattleScreen::onRender()
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
    for(int i = 0; i < rules.getSize().x; i++)
        for(int j = 0; j < rules.getSize().y; j++)
            {
                RenderVal({i,j},*gridPlayer,playerShots[i*rules.getSize().y+j]);
                RenderVal({i,j},*gridOpponent,opponentShots[i*rules.getSize().y+j]);
            }
}

void BattleScreen::onResize(RectF newSize)
{
    if (grayBrush != nullptr)
        grayBrush->Release();

    Screen::onResize(newSize);

    render.RenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::LightGray),
        &grayBrush);

    gridSize = position.size().x / 30;
    auto grid1Offset = gridSize * PointI{2,1};
    gridPlayer->onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize()+1)});

    auto grid2Offset = gridSize * PointI{2,1};
    grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1+rules.getSize().x);
    gridOpponent->onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize()+1)});
}
