#include "MakeServer.h"
#include "ServerNode.h"
#include <future>
#include <atomic>

#ifndef WidgetListH
#define WidgetListH

namespace widget
{
    class ServerList final: public Overlay<Button<Padder<TextBox>>,
                                      Button<Padder<TextBox>>,
                                      Padder<ListFinal<ServerNode>>,
                                      Switch<TextBox>,
                                      Switch<Padder<Empty>>,
                                      WidgetPtr>
    {
    private:
        std::future<std::vector<ResponseFull>> quering;
        bool isUpdating = false;
        std::atomic<bool> isFutureReady = false;
        auto& getList(){return getWidget<2>();}
        auto& getNoServerText(){return getWidget<3>();}
        auto& getTint(){return getWidget<4>();}
        auto& getPopup(){return getWidget<5>();}
    public:
        ServerList();
        ServerList(const ServerList&) = delete;
        ServerList& operator=(const ServerList&) = delete;
        void onRender() override;
        void updateServers();
        void onErrorConnection();
    };
}
#endif