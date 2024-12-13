#include "Grid.h"

#ifndef BattleGridH
#define BattleGridH

namespace widget
{
    struct BattleGrid : Grid
    {
        BattleGrid(PointI size) : Grid(size), shots(size.x * size.y) {};
        void onRender() override;
        std::atomic<Results>& getResult(PointI pos);
    protected:
        void renderShots();
    private:
        std::vector<std::atomic<Results> > shots;
        SolidBrush grayBrush{D2D1::ColorF(D2D1::ColorF::LightGray)};
        SolidBrush redBrush{D2D1::ColorF(D2D1::ColorF::Red)};
        void RenderVal(PointI pos, Results res);
    };
}
#endif