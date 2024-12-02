#include "Menu.h"
#include "Battle/SelectScreen.h"
#include "Net/ListServer.h"
#include "../Players/PcPlayer.h"

MenuScreen::MenuScreen()
{
    PcButton = std::make_unique<ButtonScreen>(
        std::make_unique<TextBox>(L"Начать игру с ПК", 40, D2D1::ColorF(D2D1::ColorF::Black)), [this](auto _)
                                  { changeScreenCallback(std::make_unique<SelectScreen>(GameRules(),
                                                                                        std::make_unique<PCPlayer>(GameRules()))); });

    NetButton = std::make_unique<ButtonScreen>(
        std::make_unique<TextBox>(L"Начать игру по сети", 40, D2D1::ColorF(D2D1::ColorF::Black)), [this](auto _)
                                  { changeScreenCallback(std::make_unique<ListScreen>()); });
}

void MenuScreen::onClick(Button button)
{
    PcButton->tryClick(button) || NetButton->tryClick(button);
}
void MenuScreen::onResize(RectF newSize)
{
    Screen::onResize(newSize);
    PcButton->onResize(PcBegin.scaled(newSize));
    NetButton->onResize(NetBegin.scaled(newSize));
}
void MenuScreen::onRender()
{
    Screen::onRender();
    render.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    PcButton->onRender();
    NetButton->onRender();
}