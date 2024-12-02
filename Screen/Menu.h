#include "Base/Button.h"
#include "Base/TextBox.h"

#ifndef MenuScreenH
#define MenuScreenH

class MenuScreen : public Screen
{
    std::unique_ptr<ButtonScreen> PcButton;
    std::unique_ptr<ButtonScreen> NetButton;
    constexpr static RectF PcBegin = {{0.05,0.4},{0.4,0.48}};
    constexpr static RectF NetBegin = {{0.05,0.5},{0.4,0.58}};
public:
    MenuScreen();
    void onClick(Button button) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        PcButton->init(renderer);
        NetButton->init(renderer);
    };
    
};
#endif