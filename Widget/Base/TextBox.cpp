#include "TextBox.h"

void TextBox::onResize(RectF newSize)
{
    Widget::onResize(newSize);
}
void TextBox::onRender()
{
    Widget::onRender();
    
    GetRenderTarget()->FillRoundedRectangle(D2D1::RoundedRect(makeD2DRectF(position),10,10),backBrush);
    GetRenderTarget()->DrawRoundedRectangle(D2D1::RoundedRect(makeD2DRectF(position),10,10),brush);
    GetRenderTarget()->DrawText(string.c_str(), string.size(), format, makeD2DRectF(position), brush);
}