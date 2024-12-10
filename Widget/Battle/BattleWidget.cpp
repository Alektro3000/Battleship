#include "BattleWidget.h"
#include <thread>
#include <algorithm>
#include "../Base/Builder.h"
namespace widget
{
    BattleWidget::BattleWidget(GameRules nRules,
                               std::unique_ptr<Player> nOpponent,
                               VisualBattleGrid &&PlayerGrid,
                               VisualBattleGrid &&OpponentGrid,
                               bool isPlayerTurn) : Overlay(std::move(PlayerGrid), std::move(OpponentGrid)), isPlayerTurn(isPlayerTurn),
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

    void BattleWidget::onClick(MouseButton button)
    {
        if (isValid && isPlayerTurn && getOpponentGrid().getGridPos().isPointInsideExcl(Context::getInstance().getCursor()))
        {
            isPlayerTurn = false;
            auto p = getOpponentGrid().getPointCoords(Context::getInstance().getCursor());
            makingMove = std::jthread([this, p]()
                                      {
            try
            {
                makeMove(p);
            }
            catch(...)
            {
                isValid = false;
            } });
        }
    };
    void BattleWidget::makeMove(PointI p)
    {
        auto res = opponent->makeMove(p);
        opponentShots[p.x * rules.getSize().y + p.y] = res;
        if (res != Results::Miss)
        {
            totalHits++;
            if (res == Results::Destroy)
                getOpponentGrid().addShip(opponent->showDestroyedShip());
            if (totalHits == rules.getTotalHitAmount())
            {
                isWon = true;
            }
            else
                isPlayerTurn = true;
        }
        else
            getMove();
    }
    void BattleWidget::getMove()
    {
        PointI shot = opponent->getMove();
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
        opponent->returnResult(result);
        if (playerShots[shot.x * rules.getSize().y + shot.y] != Results::Miss)
        {
            totalOpHits++;
            if (totalOpHits == rules.getTotalHitAmount())
            {
                isLost = true;
                auto ships = opponent->showAllShips();
                std::for_each(ships.begin(), ships.end(),
                              [this](auto val)
                              { getOpponentGrid().addShip(val); });
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
        if (res == Results::Miss)
            Context::getInstance().getRenderTarget()->FillEllipse(ellipse, grayBrush);
        else if (res != Results::Clear)
        {
            if (res == Results::Destroy)
                redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
            Context::getInstance().getRenderTarget()->FillEllipse(ellipse, redBrush);

            redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        }
    }

    void BattleWidget::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

        for (int i = 0; i < position.size().x / gridSize.x; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(position.low.x + i * gridSize.x, position.low.y),
                                                               D2D1::Point2F(position.low.x + i * gridSize.x, position.high.y), grayBrush);

        for (int i = 0; i < position.size().y / gridSize.y; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(position.low.x, position.low.y + i * gridSize.y),
                                                               D2D1::Point2F(position.high.x, position.low.y + i * gridSize.y), grayBrush);

        Overlay::onRender();
        for (int i = 0; i < rules.getSize().x; i++)
            for (int j = 0; j < rules.getSize().y; j++)
            {
                RenderVal({i, j}, getPlayerGrid(), playerShots[i * rules.getSize().y + j]);
                RenderVal({i, j}, getOpponentGrid(), opponentShots[i * rules.getSize().y + j]);
            }
        Context::getInstance().getRenderTarget()->DrawRectangle(makeD2DRectF((isPlayerTurn ? getOpponentGrid() : getPlayerGrid()).getGridPos()), redBrush, 3);

        if (!textBox)
        {
            auto size = RectF{{0.3, 0.4}, {0.7, 0.6}}.scaled(position);
            if (isLost)
                textBox = Builder::makePopUpNotification(L"Вы проиграли", size);
            if (isWon)
                textBox = Builder::makePopUpNotification(L"Вы выиграли", size);
            if (!isValid)
            {
                opponent->onDetach();
                textBox = Builder::makePopUpNotification(L"Оппонент разорвал соединение", size);
            }
        }
        else
            textBox->onRender();
    }

    void BattleWidget::onResize(RectF newSize)
    {
        Widget::onResize(newSize);
        if (textBox)
            textBox->onResize(RectF{{0.3, 0.4}, {0.7, 0.6}}.scaled(newSize));

        gridSize = position.size().x / 30;
        auto grid1Offset = gridSize * PointI{2, 1};
        getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize() + 1)});

        auto grid2Offset = gridSize * PointI{2, 1};
        grid2Offset.x = position.size().x - grid2Offset.x - gridSize.x * (1 + rules.getSize().x);
        getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize() + 1)});
    }
}