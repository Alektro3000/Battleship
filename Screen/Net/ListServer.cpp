#include "ListServer.h"

void ListScreen::onResize(RectF newSize)
{
    Screen::onResize(newSize);
    button.onResize(ButtonSelect.scaled(newSize));
}
void ListScreen::onRender()
{
    Screen::onRender();
    render.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    button.onRender();
}