#include "Grids/BeginGrid.h"
#include "Grids/BattleGrid.h"
#include "../Base/Overlay.h"

#include "../Widget.h"

#ifndef SelectWidgetH
#define SelectWidgetH

namespace widget
{
    class SelectWidget final : public Overlay<VisualBattleGrid,
                                                    VisualBeginGrid,
                                                    VisualBattleGrid>
    {
        bool isPlayerTurn;
        GameRules rules;
        std::unique_ptr<Player> opponent;
        VisualBattleGrid &getPlayerGrid()
        {
            return getWidget<0>();
        }
        VisualBeginGrid &getPlayerStartGrid()
        {
            return getWidget<1>();
        }
        VisualBattleGrid &getOpponentGrid()
        {
            return getWidget<2>();
        }

        struct DragAndDropShip
        {
            bool isDown = false;
            bool isMovedAway = false;
            BattleShip grabbedShip;
            BattleShip beginShip;
            VisualGrid *grabbedShipGrid;
            PointF beginPoint;
            PointF grabOffset;
            bool OnClickDown(PointF point, VisualGrid &grid);
            void OnClickUp(PointF point, VisualGrid &grid);
            void OnClickUp(PointF point);
            void OnUpdate(PointF point);
            void OnRotate(PointF point);
        } operation;
        PointF gridSize;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};

    public:
        SelectWidget(GameRules nRules,
                     std::unique_ptr<Player> nOpponent,
                     bool isPlayerTurn = true) : Overlay((rules.getSize()),
                                                               (rules), (rules.getSize())),
                                                 isPlayerTurn(isPlayerTurn),
                                                 opponent(std::move(nOpponent)), rules(nRules) {
                                                 };
        void onClick(MouseButton button) override;
        void onClickUp(MouseButton button) override;
        void onResize(RectF newSize) override;
        void onRender() override;
        void RenderVal(PointI pos, VisualBattleGrid &grid, Results res);
    };
}
#endif