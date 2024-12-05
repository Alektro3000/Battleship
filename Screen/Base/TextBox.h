#include "../Screen.h"

#ifndef TextBoxH
#define TextBoxH

class TextBox : public Screen
{
protected:
    TextFormat format;
    SolidBrush brush;
    SolidBrush backBrush;
    int size;
    std::wstring string;
public:
    TextBox(std::wstring str, int size, D2D1::ColorF color = D2D1::ColorF(0,1), D2D1::ColorF backColor = D2D1::ColorF(0,0)):
        string(std::move(str)), size(size), brush(color), backBrush(backColor),format(40) {};
    void onResize(RectF newSize) override;
    void onRender() override;
    void UpdateTextColor(D2D1::ColorF newColor = D2D1::ColorF(0,1)){
        brush = SolidBrush(newColor);
    }
    void UpdateBackgroundColor(D2D1::ColorF newColor = D2D1::ColorF(0,1)){
        backBrush = SolidBrush(newColor);
    }
    void UpdateText(std::wstring newText){
        string = newText;   
    }
    D2D1::ColorF GetTextColor(){
        auto val = brush.brush->GetColor();
        return D2D1::ColorF(val.r,val.g,val.b,val.a);
    }
    D2D1::ColorF GetBackgroundColor(){
        auto val = backBrush.brush->GetColor();
        return D2D1::ColorF(val.r,val.g,val.b,val.a);
    }
};
#endif