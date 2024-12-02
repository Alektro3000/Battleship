#include "../Screen.h"
#include "../Base/TextEditable.h"

#ifndef ScreenSelectMakeServerH
#define ScreenSelectMakeServerH

class ServerScreen : public Screen
{
    TextEditable text;
    constexpr static RectF TextBegin = {{0.05,0.4},{0.4,0.48}};
public:
    ServerScreen() : text(L"",40,D2D1::ColorF(D2D1::ColorF::Black)){};
    void onChar(WCHAR letter) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        text.init(renderer);
    };
    
};
#endif