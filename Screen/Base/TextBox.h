#include "../Screen.h"

#ifndef TextBoxH
#define TextBoxH

class TextBox : public Screen
{
protected:
    TextFormat format;
    SolidBrush brush;
    int size;
    D2D1::ColorF color;
    std::wstring string;
public:
    TextBox(std::wstring str, int size, D2D1::ColorF color):
        string(std::move(str)), size(size), color(color) {};
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override{
        Screen::init(renderer);
        format = TextFormat(render.WriteFactory, size);
        brush = SolidBrush(render.RenderTarget, color);
    };
    
};
#endif