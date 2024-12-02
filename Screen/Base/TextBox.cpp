#include "TextBox.h"

void TextBox::onResize(RectF newSize)
{
    Screen::onResize(newSize);
}
void TextBox::onRender()
{
    Screen::onRender();
    render.RenderTarget->DrawRectangle(makeD2DRectF(position),brush);
    render.RenderTarget->DrawText(string.c_str(), string.size(), format, makeD2DRectF(position), brush);
}