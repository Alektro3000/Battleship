
#include "ServerConnections.h"
#include "MakeServer.h"
#include <future>

namespace widget
{
    void MakeServer::makeServer()
    {
        if (isServerEnabled)
            return;
        isServerEnabled = true;
        auto str = text.getChild().getText();
        server = std::jthread([str, this](std::stop_token token)
                              {
        auto holder = ServerHolder(context, token, str);
        context.run(); });
    }
    void MakeServer::onResize(RectF newSize)
    {
        Overlay::onResize(newSize);
        if (!isServerEnabled)
            text.onResize(TextBegin.scaled(newSize));
    }
    void MakeServer::onChar(WCHAR letter)
    {
        if (!isServerEnabled)
            text.onChar(letter);
    }

    void MakeServer::onRender()
    {
        Context::getInstance().getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
        Overlay::onRender();
        if (!isServerEnabled)
            text.onRender();
    }
}