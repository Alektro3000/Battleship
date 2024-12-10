#include "ListServer.h"

namespace widget
{
    ServerList::ServerList() : Overlay(
                                   {RectF{{0.7, 0.92}, {0.99, 0.98}},
                                    Builder::makeText(L"Создать сервер")
                                        .addPadding(RectF{{10, 3}})
                                        .setBorder()
                                        .addButton(
                                            [this](auto _)
                                            { ChangeWidget(std::make_unique<MakeServer>()); })
                                        .build()},
                                   {RectF{{0.5, 0.92}, {0.68, 0.98}},
                                    Builder::makeText(L"Обновить")
                                        .addPadding(RectF{{10, 3}})
                                        .setBorder()
                                        .addButton(
                                            [this](auto _)
                                            { updateServers(); })
                                        .build()},
                                   {RectF{{0.1}, {0.9}},
                                    Builder::makeList<ServerNode>(10, 0, 0.1)
                                        .addPadding(RectF{{10, 10}})
                                        .setBorder()
                                        .build()})
    {
        updateServers();
    }
    void ServerList::updateServers()
    {
        if (!quering.valid())
        {
            isUpdating = true;
            isFutureReady = false;
            quering = std::async(std::launch::async, [this]()
                                 {auto a = queryServers(context); isFutureReady = true; return a; });
        }
    }
    void ServerList::onResize(RectF newSize)
    {
        Overlay::onResize(newSize);
        NoServerBox.onResize(RectF{{0.1, 0.1}, {0.9, 0.9}}.scaled(newSize));
    }
    void ServerList::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
        Overlay::onRender();

        if (isUpdating && isFutureReady)
        {
            std::vector<ResponseFull> servers = quering.get();
            auto &serverNodes = getWidget<2>().getChild().childs;
            serverNodes.clear();
            std::transform(servers.begin(), servers.end(), std::back_insert_iterator(serverNodes), [](auto val)
                           { return ServerNode(val); });
            getWidget<2>().getChild().update();
            isUpdating = false;
        }

        if (getWidget<2>().getChild().childs.size() == 0)
            NoServerBox.onRender();

        if (isUpdating)
        {
            Context::getInstance().getRenderTarget()->FillRectangle(makeD2DRectF(RectF{{0.1, 0.1}, {0.9, 0.9}}.scaled(position)), halfOpacity);
        }
    }
}