#include "Grids/PlayerGrid.h"
#include "Grids/OpponentGrid.h"
#include "../Base/TextBox.h"
#include "../Base/Overlay.h"
#include "../Base/WidgetPtr.h"
#include <thread>

#ifndef BattleAppH
#define BattleAppH

namespace widget
{
    class BattleWidget final : public Overlay<PlayerGrid, OpponentGrid, WidgetPtr>
    {
        std::atomic<bool> isPlayerTurn;
        std::atomic<bool> isValid = true;
        bool isOpponentDetached = false;
        std::atomic<bool> isWon = false;
        std::atomic<bool> isLost = false;
        int totalHits = 0;
        int totalOpHits = 0;

        GameRules rules;

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
        void makeMove(PointI point, std::stop_token token);

        std::unique_ptr<Player> opponent;
        std::jthread makingMove;

    public:
        BattleWidget(GameRules nRules,
                     std::unique_ptr<Player> nOpponent,
                     PlayerGrid &&gridPlayer,
                     bool isPlayerTurn = true);

        void getMove(std::stop_token token);
        void onClickDown(MouseButton button) override;
        void onResize(RectF newSize) override;
        void onRender() override;
        ~BattleWidget()
        {
            opponent->onDetach();
        }
    };
}
#endif