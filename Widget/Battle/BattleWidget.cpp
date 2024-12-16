#include "BattleWidget.h"
#include <thread>
#include <algorithm>
#include "../Base/Builder.h"
namespace widget
{
    BattleWidget::BattleWidget(GameRules nRules,
                               std::unique_ptr<Player> nOpponent,
                               PlayerGrid &&PlayerGrid,
                               bool isPlayerTurn) : Overlay(std::move(PlayerGrid), OpponentGrid{nRules.getSize(), nRules.getTotalShipAmount()}, {}),
                                                    isPlayerTurn(isPlayerTurn),
                                                    opponent(std::move(nOpponent)), rules(std::move(nRules)),
                                                    shipHits(nRules.getTotalShipAmount())
    {
        auto hash = BattleShip::getHash(std::vector<BattleShip>(getPlayerGrid().begin(), getPlayerGrid().end()));
        if (!isPlayerTurn)
        {
            makingMove = std::jthread([this, hash](std::stop_token token)
                                      { try {
                                    if(!token.stop_requested())
                                        opponent->getHashGrid();
                                    if(!token.stop_requested())
                                        opponent->returnHashGrid(hash);
                                    getMove(token);} catch(...) {isValid = false;} });
        }
        else
        {
            makingMove = std::jthread([this, hash](std::stop_token token)
                                      { try {
                                    if(!token.stop_requested())
                                        opponent->returnHashGrid(hash);
                                    if(!token.stop_requested())
                                        opponent->getHashGrid();} catch(...) {isValid = false;} });
        }
    };

    void BattleWidget::onClickDown(MouseButton button)
    {
        if (isValid && isPlayerTurn && getOpponentGrid().getGridPos().isPointInsideExcl(Context::getInstance().getCursor()))
        {
            isPlayerTurn = false;
            auto p = getOpponentGrid().getPointCoords(Context::getInstance().getCursor());
            makingMove = std::jthread([this, p](std::stop_token token)
                                      {
            try
            {
                makeMove(p, token);
            }
            catch(...)
            {
                isValid = false;
            } });
        }
    };
    // Player Move
    void BattleWidget::makeMove(PointI p, std::stop_token token)
    {
        auto res = opponent->makeMove(p);
        if (token.stop_requested())
            return;
        getOpponentGrid().getResult(p) = res.val;
        if (res.val == Results::Miss)
        {
            getMove(token);
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
    void BattleWidget::getMove(std::stop_token token)
    {
        PointI shot = opponent->getMove();
        if (token.stop_requested())
            return;

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
        if (token.stop_requested())
            return;

        if (result.val == Results::Miss)
        {
            isPlayerTurn = true;
            return;
        }

        if (++totalOpHits != rules.getTotalHitAmount())
        {
            getMove(token);
            return;
        }

        isLost = true;
        auto ships = opponent->showAllShips();
        if (token.stop_requested())
            return;

        std::for_each(ships.begin(), ships.end(),
                      [this](auto val)
                      { getOpponentGrid().addShip(val); });
    }

    void BattleWidget::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));

        for (int i = 0; i < getPosition().size().x / gridSize.x; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(getPosition().low.x + i * gridSize.x, getPosition().low.y),
                                                               D2D1::Point2F(getPosition().low.x + i * gridSize.x, getPosition().high.y), grayBrush);

        for (int i = 0; i < getPosition().size().y / gridSize.y; i++)
            Context::getInstance().getRenderTarget()->DrawLine(D2D1::Point2F(getPosition().low.x, getPosition().low.y + i * gridSize.y),
                                                               D2D1::Point2F(getPosition().high.x, getPosition().low.y + i * gridSize.y), grayBrush);

        Overlay::onRender();
        if (getWidget<2>().widget)
            return;

        auto size = getPosition();
        if (isLost)
            getWidget<2>().widget = Builder::makePopUpNotification(L"Вы проиграли", size);
        if (isWon)
            getWidget<2>().widget = Builder::makePopUpNotification(L"Вы выиграли", size);
        if (!isValid)
        {
            opponent->onDetach();
            getWidget<2>().widget = Builder::makePopUpNotification(L"Оппонент разорвал соединение", size);
        }

        Context::getInstance().getRenderTarget()->DrawRectangle(
            makeD2DRectF((isPlayerTurn ? getOpponentGrid().getGridPos() : getPlayerGrid().getGridPos())), redBrush, 2);
    }

    void BattleWidget::onResize(RectF newSize)
    {
        Widget::onResize(newSize);

        gridSize = getPosition().size().x / 30;
        auto grid1Offset = gridSize * PointI{2, 1};
        getPlayerGrid().onResize({grid1Offset, grid1Offset + gridSize * (rules.getSize() + 1)});

        auto grid2Offset = gridSize * PointI{2, 1};
        grid2Offset.x = getPosition().size().x - grid2Offset.x - gridSize.x * (1 + rules.getSize().x);
        getOpponentGrid().onResize({grid2Offset, grid2Offset + gridSize * (rules.getSize() + 1)});

        getWidget<2>().onResize(newSize);
    }
}