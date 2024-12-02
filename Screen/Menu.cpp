#include "Menu.h"
#include "../Players/PcPlayer.h"

void MenuScreen::onClick(Button button)
{
    if(PcBegin.scaled(position).isPointInsideExcl(getCursor()))
    {
        changeScreenCallback(std::make_unique<SelectScreen>(GameRules(),
                std::make_unique<PCPlayer>(GameRules())));
        return;
    }
    if(NetBegin.scaled(position).isPointInsideExcl(getCursor()))
    {
        
        return;
    }
}
void MenuScreen::onResize(RectF newSize)
{
    Screen::onResize(newSize);
}
void MenuScreen::onRender()
{
    Screen::onRender();
    render.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    RectF pos = PcBegin.scaled(position);
    render.RenderTarget->DrawRectangle(makeD2DRectF(pos),brush);
    render.RenderTarget->DrawText(L"Начать игру с ПК", 17, format, makeD2DRectF(pos), brush);

    
    pos = NetBegin.scaled(position);
    render.RenderTarget->DrawRectangle(makeD2DRectF(pos),brush);
    render.RenderTarget->DrawText(L"Начать игру по сети", 20, format, makeD2DRectF(pos), brush);

}