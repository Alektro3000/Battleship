#include "Menu.h"
#include "Battle/SelectWidget.h"
#include "Net/ListServer.h"
#include "../Players/PcPlayer.h"

MenuWidget::MenuWidget() : WidgetOverlay(
    {RectF{{0.05,0.4},{0.4,0.48}},ButtonWidget(TextBox(L"Начать игру с ПК", 40),
                     [](auto _){ ChangeWidget(std::make_unique<SelectWidget>(GameRules(),std::make_unique<PCPlayer>(GameRules()))); })},
    {RectF{{0.05,0.5},{0.4,0.58}},ButtonWidget(TextBox(L"Начать игру по сети", 40), 
                     [](auto _){ ChangeWidget(std::make_unique<ListWidget>()); })})
{

}

void MenuWidget::onRender()
{
    GetRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
    WidgetOverlay::onRender();
}