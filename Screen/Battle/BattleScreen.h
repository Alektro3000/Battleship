#include "Grids/BattleGrid.h"
#include <thread>

#include "../Screen.h"

#ifndef BattleAppH
#define BattleAppH

class BattleScreen : public Screen
{
    // Could be changed from another thread
    std::atomic<bool> isPlayerTurn;
    // Could be changed from another thread
    std::vector<std::atomic<Results>> playerShots;
    // Could be changed from another thread
    std::vector<std::atomic<Results>> opponentShots;
    std::jthread makingMove;

    GameRules rules;
    std::unique_ptr<Player> opponent;

    std::unique_ptr<VisualBattleGrid> gridPlayer;
    std::unique_ptr<VisualBattleGrid> gridOpponent;

    std::vector<int> shipHits;

    PointF gridSize;
    ID2D1SolidColorBrush *grayBrush = nullptr;

public:
    BattleScreen(GameRules nRules,
                 std::unique_ptr<Player> nOpponent,
                 std::unique_ptr<VisualBattleGrid> gridPlayer,
                 std::unique_ptr<VisualBattleGrid> gridOpponent,
                 bool isPlayerTurn = true) : isPlayerTurn(isPlayerTurn),
                                             opponent(std::move(nOpponent)), rules(std::move(nRules)),
                                             gridPlayer(std::move(gridPlayer)),
                                             gridOpponent(std::move(gridOpponent)),
                                             opponentShots(nRules.getSize().x * nRules.getSize().y),
                                             playerShots(nRules.getSize().x * nRules.getSize().y),
                                             shipHits(nRules.getTotalShipAmount()) {
                                                 if(!isPlayerTurn)
                                                 {
                                                    makingMove = std::jthread([this](){
                                                        getMove();
                                                    });
                                                 }
                                             };

    void getMove();
    void onClick(Button button) override;
    void onClickUp(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void RenderVal(PointI pos, VisualBattleGrid& grid, Results res);
    void init(RenderThings npRT) override { 
        Screen::init(npRT);
        gridPlayer->init(npRT);
        gridOpponent->init(npRT);
    }
    // this is the function used to render a single frame
    ~BattleScreen() override
    {
        if (grayBrush != nullptr)
            grayBrush->Release();
    };
};
#endif