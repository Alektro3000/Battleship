#include "Grids/BattleGrid.h"
#include "../Base/TextBox.h"
#include "../Base/Overlay.h"
#include <thread>

#include "../Widget.h"

#ifndef BattleAppH
#define BattleAppH

namespace widget
{
    class BattleWidget final : public Overlay<VisualBattleGrid, VisualBattleGrid>
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

        std::unique_ptr<Widget> textBox = nullptr;

        VisualBattleGrid &getPlayerGrid()
        {
            return getWidget<0>();
        }
        VisualBattleGrid &getOpponentGrid()
        {
            return getWidget<1>();
        }
        std::vector<int> shipHits;

        PointF gridSize;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
        SolidBrush redBrush{D2D1::ColorF(D2D1::ColorF::Red)};

        void makeMove(PointI point);

    public:
        BattleWidget(GameRules nRules,
                     std::unique_ptr<Player> nOpponent,
                     VisualBattleGrid &&gridPlayer,
                     VisualBattleGrid &&gridOpponent,
                     bool isPlayerTurn = true);

        void getMove();
        void onClick(MouseButton button) override;
        void onResize(RectF newSize) override;
        void onRender() override;
        void RenderVal(PointI pos, VisualBattleGrid &grid, Results res);
    };
}
#endif