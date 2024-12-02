#include "Grids/BeginGrid.h"
#include "Grids/BattleGrid.h"

#include "../Screen.h"

#ifndef SelectScreenH
#define SelectScreenH

class SelectScreen : public Screen
{
    bool isPlayerTurn;
    GameRules rules;
    std::unique_ptr<Player> opponent;

    std::unique_ptr<VisualBattleGrid> gridPlayer;
    std::unique_ptr<VisualBeginGrid> gridPlayerBegin;
    std::unique_ptr<VisualBattleGrid> gridOpponent;
    
    struct DragAndDropShip
    {
        bool isDown = false;
        BattleShip grabbedShip;
        BattleShip beginShip;
        VisualGrid* grabbedShipGrid;
        PointF beginPoint;
        PointI grabOffset;
        bool OnClickDown(PointF point, VisualGrid& grid);
        void OnClickUp(PointF point, VisualGrid& grid);
        void OnUpdate(PointF point, RenderThings drawTarget);
        void OnRotate();
    } operation;
    PointF gridSize;
    SolidBrush grayBrush;
public:
    SelectScreen(GameRules nRules,
                 std::unique_ptr<Player> nOpponent,
                 bool isPlayerTurn = true) : isPlayerTurn(isPlayerTurn),
                                             opponent(std::move(nOpponent)), rules(nRules),
                                             gridPlayer(std::make_unique<VisualBattleGrid>(rules.getSize())),
                                             gridPlayerBegin(std::make_unique<VisualBeginGrid>(rules)),
                                             gridOpponent(std::make_unique<VisualBattleGrid>(rules.getSize())) {
                                                 
                                             };
    void onClick(Button button) override;
    void onClickUp(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void RenderVal(PointI pos, VisualBattleGrid& grid, Results res);
    void init(RenderThings npRT) override { 
        Screen::init(npRT);
        gridPlayer->init(npRT);
        gridPlayerBegin->init(npRT);
        gridOpponent->init(npRT);
    }
};

#endif