#include "ServerConnections.h"
#include "../Base/Builder.h"
#include <future>
#include <atomic>

#ifndef WidgetServerNodeListH
#define WidgetServerNodeListH

namespace widget
{
    class ServerNode : public Stack<Padder<Overlay<Padder<TextBox>, Padder<TextBox> > > >
    {
    private:
        ResponseFull info;

    public:
        ServerNode(ResponseFull info);
        void onErrorConnection();
        void onClick(MouseButton button) override;
    };
}
#endif