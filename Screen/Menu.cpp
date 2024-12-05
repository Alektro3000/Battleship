#include "Menu.h"
#include "Battle/SelectScreen.h"
#include "Net/ListServer.h"
#include "../Players/PcPlayer.h"

MenuScreen::MenuScreen() : ScreenOverlay(
    {RectF{{0.05,0.4},{0.4,0.48}},ButtonScreen(TextBox(L"Начать игру с ПК", 40),
                     [](auto _){ ChangeScreen(std::make_unique<SelectScreen>(GameRules(),std::make_unique<PCPlayer>(GameRules()))); })},
    {RectF{{0.05,0.5},{0.4,0.58}},ButtonScreen(TextBox(L"Начать игру по сети", 40), 
                     [](auto _){ ChangeScreen(std::make_unique<ListScreen>()); })})
{

}

void MenuScreen::onRender()
{
    GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
    ScreenOverlay::onRender();
}