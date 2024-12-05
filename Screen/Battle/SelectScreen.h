#include "Grids/BeginGrid.h"
#include "Grids/BattleGrid.h"
#include "../ScreenOverlay.h"

#include "../Screen.h"

#ifndef SelectScreenH
#define SelectScreenH

class SelectScreen final : public ScreenOverlay<VisualBattleGrid, VisualBeginGrid, VisualBattleGrid>
{
    bool isPlayerTurn;
    GameRules rules;
    std::unique_ptr<Player> opponent;
    VisualBattleGrid& getPlayerGrid()
    {
        return std::get<0>(_screens);
    }
    VisualBeginGrid& getPlayerStartGrid()
    {
        return std::get<1>(_screens);
    }
    VisualBattleGrid& getOpponentGrid()
    {
        return std::get<2>(_screens);
    }
    
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
        void OnUpdate(PointF point);
        void OnRotate();
    } operation;
    PointF gridSize;
    SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
public:
    SelectScreen(GameRules nRules,
                 std::unique_ptr<Player> nOpponent,
                 bool isPlayerTurn = true) : ScreenOverlay((rules.getSize()),
                                             (rules),(rules.getSize())),
                                             isPlayerTurn(isPlayerTurn),
                                             opponent(std::move(nOpponent)), rules(nRules) {
                                             };
    void onClick(Button button) override;
    void onClickUp(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void RenderVal(PointI pos, VisualBattleGrid& grid, Results res);
};

#endif