#include "../Widget.h"

#ifndef TextBoxH
#define TextBoxH

namespace widget
{

    class TextBox : public Widget
    {
    protected:
        TextFormat format;
        SolidBrush brush;
        int size;
        std::wstring string;
        bool isEditable = false;

    public:
        TextBox(std::wstring str, int size = -1, D2D1::ColorF color = D2D1::ColorF(0, 1)) : string(std::move(str)), size(size), brush(color), format(size) {};
        void onResize(RectF newSize) override;
        void onRender() override;
        void UpdateTextColor(D2D1::ColorF newColor = D2D1::ColorF(0, 1))
        {
            brush = SolidBrush(newColor);
        }
        void UpdateText(std::wstring newText)
        {
            string = newText;
        }
        std::wstring getText()
        {
            return string;
        }
        D2D1::ColorF GetTextColor()
        {
            auto val = brush.brush->GetColor();
            return D2D1::ColorF(val.r, val.g, val.b, val.a);
        }
        void onChar(WCHAR letter) override;
    };
}
#endif