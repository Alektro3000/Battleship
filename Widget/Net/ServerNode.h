#include "ServerConnections.h"
#include "../Base/Builder.h"
#include <future>
#include <atomic>

#ifndef WidgetServerNodeListH
#define WidgetServerNodeListH

namespace widget
{
    class ServerNode final: public Stack<Padder<OverlayFinal<Padder<TextBox>, Padder<TextBox> > > >
    {
    private:
        ResponseFull info;
        class ServerList* parent;
    public:
        ServerNode(class ServerList* parent, ResponseFull info);
        void onErrorConnection();
        void onClickDown(MouseButton button) override;
    };
}
#endif