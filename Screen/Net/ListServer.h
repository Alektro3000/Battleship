#include "MakeServer.h"
#include "../Base/Button.h"
#include "../Base/TextBox.h"

#ifndef ScreenListH
#define ScreenListH

class ListScreen : public Screen
{
private:
    ButtonScreen button;
    constexpr static RectF ButtonSelect = {{0.7,0.88},{0.98,0.95}};
public:
    ListScreen(): button(std::make_unique<TextBox>
        (L"Создать сервер",40,D2D1::ColorF(D2D1::ColorF::Black)),
        [this](auto val){changeScreenCallback(std::make_unique<ServerScreen>());})
    {

    }
    void onClick(Button click) override{button.tryClick(click); };
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        button.init(renderer);
    };
};
#endif