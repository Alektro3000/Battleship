#include "MakeServer.h"

void ServerScreen::onResize(RectF newSize)
{
    Screen::onResize(newSize);
    text.onResize(TextBegin.scaled(newSize));
}
void ServerScreen::onChar(WCHAR letter)
{
    text.onChar(letter);
}

void ServerScreen::onRender()
{
    Screen::onRender();
    render.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    text.onRender();
}