#include "Menu.h"
#include "Battle/SelectWidget.h"
#include "Battle/RulesForPc.h"
#include "Net/ServerList.h"
#include "../Players/PcPlayer.h"
#include "Base/Builder.h"

namespace widget
{
    Menu::Menu() : Overlay(
                       {RectF{{0.05, 0.4}, {0.4, 0.48}},
                        Builder::makeText(L"Начать игру с ПК")
                            .addPadding(RectF{{20, 3}})
                            .setBorder()
                            .addButton([](auto _)
                                       { pushWidget(std::make_unique<PcRules>()); })
                            .build()},
                       {RectF{{0.05, 0.5}, {0.4, 0.58}},
                        Builder::makeText(L"Начать игру по сети")
                            .addPadding(RectF{{20, 3}})
                            .setBorder()
                            .addButton([](auto _)
                                       { pushWidget(std::make_unique<ServerList>()); })
                            .build()},
                       {RectF{{0.05, 0.6}, {0.4, 0.68}},
                        Builder::makeText(L"Создать сервер")
                            .addPadding(RectF{{20, 3}})
                            .setBorder()
                            .addButton([](auto _)
                                       { pushWidget(std::make_unique<MakeServer>()); })
                            .build()})
    {
    }

}