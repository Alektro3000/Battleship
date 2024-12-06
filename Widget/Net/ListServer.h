#define UNICODE
#include <boost/asio/ip/address_v4.hpp>
#include "MakeServer.h"
#include "../Base/Button.h"
#include "../Base/TextBox.h"
#include <future>
#include <atomic>

#ifndef WidgetListH
#define WidgetListH

struct ResponseFull
{
  Response response;
  boost::asio::ip::address_v4 ipv4;
};

class ListWidget : public WidgetOverlay<ButtonWidget<TextBox>,ButtonWidget<TextBox> >
{
private:
    std::future<std::vector<ResponseFull> > quering;
    std::vector<ResponseFull> servers;
    bool isUpdating = false;
    std::atomic<bool> isFutureReady = false;
    SolidBrush brush{D2D1::ColorF(0)};
    SolidBrush halfOpacity{D2D1::ColorF(D2D1::ColorF::LightGray,0.5f)};
    TextBox NoServerBox={L"Нет серверов",40};
    TextFormat format = {40};
public:
    ListWidget(): WidgetOverlay({RectF{{0.7,0.92},{0.99,0.98}},
        ButtonWidget(TextBox(L"Создать сервер",40,D2D1::ColorF(D2D1::ColorF::Black)),
        [this](auto _){ChangeWidget(std::make_unique<ServerWidget>());})},
        {RectF{{0.5,0.92},{0.68,0.98}},
        ButtonWidget(TextBox(L"Обновить",40,D2D1::ColorF(D2D1::ColorF::Black)),
        [this](auto _){ updateServers();})})
    {
        updateServers();
    }
    void onResize(RectF newSize) override;
    void onRender() override;
    void updateServers();
};
#endif