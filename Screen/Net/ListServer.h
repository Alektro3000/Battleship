#define UNICODE
#include <boost/asio/ip/address_v4.hpp>
#include "MakeServer.h"
#include "../Base/Button.h"
#include "../Base/TextBox.h"
#include <future>
#include <atomic>

#ifndef ScreenListH
#define ScreenListH

struct ResponseFull
{
  Response response;
  boost::asio::ip::address_v4 ipv4;
};

class ListScreen : public ScreenOverlay<ButtonScreen<TextBox>,ButtonScreen<TextBox> >
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
    ListScreen(): ScreenOverlay({RectF{{0.7,0.92},{0.99,0.98}},
        ButtonScreen(TextBox(L"Создать сервер",40,D2D1::ColorF(D2D1::ColorF::Black)),
        [this](auto _){ChangeScreen(std::make_unique<ServerScreen>());})},
        {RectF{{0.5,0.92},{0.68,0.98}},
        ButtonScreen(TextBox(L"Обновить",40,D2D1::ColorF(D2D1::ColorF::Black)),
        [this](auto _){ updateServers();})})
    {
        updateServers();
    }
    void onResize(RectF newSize) override;
    void onRender() override;
    void updateServers();
};
#endif