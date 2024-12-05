#include "Grids/BattleGrid.h"
#include "../Base/TextBox.h"
#include "../ScreenOverlay.h"
#include <thread>

#include "../Screen.h"

#ifndef BattleAppH
#define BattleAppH

class BattleScreen final : public ScreenOverlay<VisualBattleGrid, VisualBattleGrid>
{
    // Could be changed from another thread
    std::atomic<bool> isPlayerTurn;
    // Could be changed from another thread
    std::vector<std::atomic<Results>> playerShots;
    // Could be changed from another thread
    std::vector<std::atomic<Results>> opponentShots;
    // Could be changed from another thread
    std::atomic<bool> isValid = true;
    bool isOpponentDetached = false;
    std::atomic<bool> isWon = false;
    std::atomic<bool> isLost = false;
    int totalHits = 0;
    int totalOpHits = 0;
    std::jthread makingMove;

    GameRules rules;
    std::unique_ptr<Player> opponent;

    TextBox textBox = {L"", 40, D2D1::ColorF(0,0)};

    VisualBattleGrid& getPlayerGrid()
    {
        return std::get<0>(_screens);
    }
    VisualBattleGrid& getOpponentGrid()
    {
        return std::get<1>(_screens);
    }
    std::vector<int> shipHits;

    PointF gridSize;
    SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
    SolidBrush redBrush{D2D1::ColorF(D2D1::ColorF::Red)};

public:
    BattleScreen(GameRules nRules,
                 std::unique_ptr<Player> nOpponent,
                 VisualBattleGrid&& gridPlayer,
                 VisualBattleGrid&& gridOpponent,
                 bool isPlayerTurn = true);

    void getMove();
    void onClick(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void RenderVal(PointI pos, VisualBattleGrid& grid, Results res);
};
#endif