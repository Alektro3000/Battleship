#include "Grids/PlayerGrid.h"
#include "Grids/OpponentGrid.h"
#include "../Base/TextBox.h"
#include "../Base/Overlay.h"
#include <thread>

#include "../Widget.h"

#ifndef BattleAppH
#define BattleAppH

namespace widget
{
    class BattleWidget final : public Overlay<PlayerGrid, OpponentGrid>
    {
        std::atomic<bool> isPlayerTurn;
        std::atomic<bool> isValid = true;
        bool isOpponentDetached = false;
        std::atomic<bool> isWon = false;
        std::atomic<bool> isLost = false;
        int totalHits = 0;
        int totalOpHits = 0;
        std::jthread makingMove;

        GameRules rules;
        std::unique_ptr<Player> opponent;

        std::unique_ptr<Widget> textBox = nullptr;

        PlayerGrid &getPlayerGrid()
        {
            return getWidget<0>();
        }
        OpponentGrid &getOpponentGrid()
        {
            return getWidget<1>();
        }
        std::vector<unsigned int> shipHits;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
        SolidBrush redBrush{D2D1::ColorF(D2D1::ColorF::Red)};

        PointF gridSize;
        void makeMove(PointI point);

    public:
        BattleWidget(GameRules nRules,
                     std::unique_ptr<Player> nOpponent,
                     PlayerGrid &&gridPlayer,
                     bool isPlayerTurn = true);

        void getMove();
        void onClickDown(MouseButton button) override;
        void onResize(RectF newSize) override;
        void onRender() override;
    };
}
#endif