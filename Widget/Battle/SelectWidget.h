#include "Grids/BeginGrid.h"
#include "Grids/PlayerGrid.h"
#include "Grids/OpponentGrid.h"
#include "../Base/Builder.h"

#include "../Widget.h"

#ifndef SelectWidgetH
#define SelectWidgetH

namespace widget
{
    class SelectWidget final : public Overlay<PlayerGrid,
                                                    BeginGrid,
                                                    OpponentGrid,
                                                    Button<Padder<TextBox>>>
    {
        bool isPlayerTurn;
        GameRules rules;
        std::unique_ptr<Player> opponent;
        PlayerGrid &getPlayerGrid()
        {
            return getWidget<0>();
        }
        BeginGrid &getPlayerStartGrid()
        {
            return getWidget<1>();
        }
        OpponentGrid &getOpponentGrid()
        {
            return getWidget<2>();
        }
        auto& getAutoButton()
        {
            return getWidget<3>();
        }

        struct DragAndDropShip
        {
            bool isDown = false;
            bool isMovedAway = false;
            BattleShip grabbedShip;
            BattleShip beginShip;
            Grid *grabbedShipGrid;
            PointF beginPoint;
            PointF grabOffset;
            bool onClickDownDown(PointF point, Grid &grid);
            void onClickUp(PointF point, Grid &grid);
            void onClickUp(PointF point);
            void OnUpdate(PointF point);
            void OnRotate(PointF point);
        } operation;
        PointF gridSize;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};

    public:
        SelectWidget(GameRules nRules,
                     std::unique_ptr<Player> nOpponent,
                     bool isPlayerTurn = true);
        void onClickDown(MouseButton button) override;
        void onClickUp(MouseButton button) override;
        void onResize(RectF newSize) override;
        void onRender() override;
        void autoSelect();
    };
}
#endif