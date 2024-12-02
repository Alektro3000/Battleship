#include "Battle/SelectScreen.h"

#ifndef MenuScreenH
#define MenuScreenH

class MenuScreen : public Screen
{
    TextFormat format;
    SolidBrush brush;
    constexpr static RectF PcBegin = {{0.05,0.4},{0.4,0.48}};
    constexpr static RectF NetBegin = {{0.05,0.5},{0.4,0.58}};
public:
    void onClick(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        format = TextFormat(render.WriteFactory,40);
        brush = SolidBrush(render.RenderTarget, D2D1::ColorF(D2D1::ColorF::Black));
    };
    
};
#endif