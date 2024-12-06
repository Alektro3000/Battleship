#include "BattleWidget.h"
#include <thread>
#include <algorithm>
BattleWidget::BattleWidget(GameRules nRules,
                           std::unique_ptr<Player> nOpponent,
                            VisualBattleGrid&& PlayerGrid,
                            VisualBattleGrid&& OpponentGrid,
                           bool isPlayerTurn) : WidgetOverlay(std::move(PlayerGrid),std::move(OpponentGrid)), isPlayerTurn(isPlayerTurn),
                                                       opponent(std::move(nOpponent)), rules(std::move(nRules)),
                                                       opponentShots(nRules.getSize().x * nRules.getSize().y),
                                                       playerShots(nRules.getSize().x * nRules.getSize().y),
                                                       shipHits(nRules.getTotalShipAmount())
{
    if (!isPlayerTurn)
    {
        makingMove = std::jthread([this]()
                                  { try {getMove();} catch(...) {isValid = false;} });
    }
};

void BattleWidget::onClick(Button button)
{
    if (isValid && isPlayerTurn && getOpponentGrid().getGridPos().isPointInsideExcl(getCursor()))
    {
        isPlayerTurn = false;
        auto p = getOpponentGrid().getPointCoords(getCursor());
        makingMove = std::jthread([this, p]()
                                  {
            try
            {
                opponentShots[p.x*rules.getSize().y+p.y] = opponent->MakeMove(p);
                if(opponentShots[p.x*rules.getSize().y+p.y] != Results::Miss)
                {
                    totalHits++;
                    if(totalHits == rules.getTotalHitAmount())
                    {         
                        isWon = true;
                    }
                    else
                        isPlayerTurn = true;
                }
                else
                    getMove(); 
            }
            catch(...)
            {
                isValid = false;
            } });
    }
};
void BattleWidget::getMove()
{
    PointI shot = opponent->GetMove();
    auto damagedShip = std::find_if(getPlayerGrid().begin(), getPlayerGrid().end(),
                                    [shot](BattleShip y)
                                    { return y.IntersectionPosition(shot) != -1; });

    auto result = Results::Miss;
    if (damagedShip != getPlayerGrid().end())
    {
        auto i = damagedShip - getPlayerGrid().begin();
        shipHits[i] |= damagedShip->getHitMask(shot);
        result = damagedShip->isDestroyedRes(shipHits[i]);
    }

    playerShots[shot.x * rules.getSize().y + shot.y] = result;
    opponent->ReturnResult(result);
    if (playerShots[shot.x * rules.getSize().y + shot.y] != Results::Miss)
    {
        totalOpHits++;
        if (totalOpHits == rules.getTotalHitAmount())
        {
            isLost = true;
        }
        else
            getMove();
    }
    else
        isPlayerTurn = true;
}

void BattleWidget::RenderVal(PointI pos, VisualBattleGrid &grid, Results res)
{
    auto ellipse = D2D1::Ellipse(makeD2DPointF(grid.getCoordPosition(pos) + grid.getGridSize() / 2), 10, 10);
    if(res  == Results::Miss)
        GetRenderTarget()->FillEllipse(ellipse,grayBrush);
    else if (res != Results::Clear)
    {
        if (res == Results::Destroy)
            redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
        GetRenderTarget()->FillEllipse(ellipse, redBrush);

        redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    }
}

void BattleWidget::onRender()
{
    GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

    for (int i = 0; i < position.size().x / gridSize.x; i++)
        GetRenderTarget()->DrawLine(D2D1::Point2F(position.low.x + i * gridSize.x, position.low.y),
                                      D2D1::Point2F(position.low.x + i * gridSize.x, position.high.y), grayBrush);

    for (int i = 0; i < position.size().y / gridSize.y; i++)
        GetRenderTarget()->DrawLine(D2D1::Point2F(position.low.x, position.low.y + i * gridSize.y),
                                      D2D1::Point2F(position.high.x, position.low.y + i * gridSize.y), grayBrush);

    WidgetOverlay::onRender();
    for (int i = 0; i < rules.getSize().x; i++)
        for (int j = 0; j < rules.getSize().y; j++)
        {
            RenderVal({i, j}, getPlayerGrid(), playerShots[i * rules.getSize().y + j]);
            RenderVal({i, j}, getOpponentGrid(), opponentShots[i * rules.getSize().y + j]);
        }
    GetRenderTarget()->DrawRectangle(makeD2DRectF((isPlayerTurn?getOpponentGrid():getPlayerGrid()).getGridPos()),redBrush,3);
    
    textBox.onRender();
    if (textBox.GetBackgroundColor() == D2D1::ColorF(0, 0))
    {
        if (isLost)
        {
            textBox.UpdateText(L"Вы проиграли");
            textBox.UpdateTextColor();
            textBox.UpdateBackgroundColor(D2D1::ColorF(D2D1::ColorF::LightSlateGray));
        }
        if (isWon)
        {
            textBox.UpdateText(L"Вы выиграли");
            textBox.UpdateTextColor();
            textBox.UpdateBackgroundColor(D2D1::ColorF(D2D1::ColorF::LightSlateGray));
        }
        if (!isValid)
        {
            opponent->onDetach();
            textBox.UpdateText(L"Противник разорвал соединение");
            textBox.UpdateTextColor();
            textBox.UpdateBackgroundColor(D2D1::ColorF(D2D1::ColorF::LightSlateGray));
        }
    }
}

void BattleWidget::onResize(RectF newSize)
{
    Widget::onResize(newSize);
    textBox.onResize(RectF{{0.3, 0.4}, {0.7, 0.6}}.scaled(newSize));

    gridSize = position.size().x / 30;
    auto grid1Offset = gridSize * PointI{2, 1};
    getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize() + 1)});

    auto grid2Offset = gridSize * PointI{2, 1};
    grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1 + rules.getSize().x);
    getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize() + 1)});
}
