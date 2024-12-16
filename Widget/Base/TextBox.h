#include "Widget.h"

#ifndef TextBoxH
#define TextBoxH

namespace widget
{

    class TextBox final: public Widget
    {
        TextFormat format;
        SolidBrush brush;
        float size;
        std::wstring string;

    public:
        TextBox(TextBox&& other) = default;
        TextBox& operator =(TextBox&& other) = default;
        
        bool isEditable = false;
        TextBox(std::wstring str, float size = -1, D2D1::ColorF color = D2D1::ColorF(0, 1)) : string(std::move(str)), size(size), brush(color), format(size) {};
        void onResize(RectF newSize) override;
        void onRender() override;
        void updateTextColor(D2D1::ColorF newColor = D2D1::ColorF(0, 1));
        void updateText(std::wstring newText);
        std::wstring getText();
        D2D1::ColorF getTextColor();
        void onChar(WCHAR letter) override;
    };
}
#endif