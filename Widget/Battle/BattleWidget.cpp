#include "BattleWidget.h"
#include <thread>
#include <algorithm>
#include "../Base/Builder.h"
namespace widget
{
    BattleWidget::BattleWidget(GameRules nRules,
                               std::unique_ptr<Player> nOpponent,
                               PlayerGrid &&PlayerGrid,
                               bool isPlayerTurn) : Overlay(std::move(PlayerGrid), OpponentGrid{nRules.getSize(),nRules.getTotalShipAmount()}), isPlayerTurn(isPlayerTurn),
                                                    opponent(std::move(nOpponent)), rules(std::move(nRules)),
                                                    shipHits(nRules.getTotalShipAmount())
    {
        auto hash = BattleShip::getHash(std::vector<BattleShip>(getPlayerGrid().begin(), getPlayerGrid().end()));
        if (!isPlayerTurn)
        {
            makingMove = std::jthread([this, hash]()
                                      { try {
                                    opponent->getHashGrid();
                                    opponent->returnHashGrid(hash);
                                    getMove();} catch(...) {isValid = false;} });
        }
        else
        {
            makingMove = std::jthread([this, hash]()
                                      { try {
                                    opponent->returnHashGrid(hash);
                                    opponent->getHashGrid();} catch(...) {isValid = false;} });
        }
    };

    void BattleWidget::onClickDown(MouseButton button)
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
    // Player Move
    void BattleWidget::makeMove(PointI p)
    {
        auto res = opponent->makeMove(p);
        getOpponentGrid().getResult(p) = res.val;
        if (res.val == Results::Miss)
        {
            getMove();
            return;
        }

        if (res.val == Results::Destroy)
            getOpponentGrid().addShip(res.destroyedShip);

        if (++totalHits != rules.getTotalHitAmount())
        {
            isPlayerTurn = true;
            return;
        }

        isWon = true;
        opponent->onEnd(std::vector<BattleShip>(getPlayerGrid().begin(), getPlayerGrid().end()));
    }
    // Opponent move
    void BattleWidget::getMove()
    {
        PointI shot = opponent->getMove();
        auto damagedShip = std::find_if(getPlayerGrid().begin(), getPlayerGrid().end(),
                                        [shot](BattleShip y)
                                        { return y.IntersectionPosition(shot) != -1; });

        AttResult result = Results::Miss;
        if (damagedShip != getPlayerGrid().end())
        {
            auto i = damagedShip - getPlayerGrid().begin();
            shipHits[i] |= (*damagedShip).getHitMask(shot);
            result = AttResult(*damagedShip, shipHits[i]);
        }

        getPlayerGrid().getResult(shot) = result.val;
        opponent->returnResult(result);
        if (result.val == Results::Miss)
        {
            isPlayerTurn = true;
            return;
        }

        if (++totalOpHits != rules.getTotalHitAmount())
        {
            getMove();
            return;
        }

        isLost = true;
        auto ships = opponent->showAllShips();
        std::for_each(ships.begin(), ships.end(),
                      [this](auto val)
                      { getOpponentGrid().addShip(val); });
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
        Context::getInstance().getRenderTarget()->DrawRectangle(
            makeD2DRectF((isPlayerTurn ? getOpponentGrid().getGridPos() : getPlayerGrid().getGridPos())), redBrush, 2);

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