#include "MakeServer.h"
#include "ServerNode.h"
#include "../Base/List.h"
#include <future>
#include <atomic>

#ifndef WidgetListH
#define WidgetListH

namespace widget
{
    class ServerList final: public Overlay<Button<Padder<TextBox>>,
                                      Button<Padder<TextBox>>,
                                      Padder<List<ServerNode>>>
    {
    private:
        std::future<std::vector<ResponseFull>> quering;
        bool isUpdating = false;
        std::atomic<bool> isFutureReady = false;
        SolidBrush brush{D2D1::ColorF(0)};
        SolidBrush halfOpacity{D2D1::ColorF(D2D1::ColorF::LightGray, 0.5f)};
        TextBox NoServerBox = {L"Нет серверов"};

    public:
        ServerList();
        ServerList(const ServerList&) = delete;
        ServerList& operator=(const ServerList&) = delete;
        void onResize(RectF newSize) override;
        void onRender() override;
        void updateServers();
    };
}
#endif