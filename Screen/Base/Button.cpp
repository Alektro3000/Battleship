#include "Button.h"

void ButtonScreen::onResize(RectF newSize)
{
    Screen::onResize(newSize);
    child->onResize(newSize);
}
void ButtonScreen::onRender()
{
    Screen::onRender();
    child->onRender();
}

void ButtonScreen::init(RenderThings renderer) 
{
    Screen::init(renderer);
    child->init(renderer);
}