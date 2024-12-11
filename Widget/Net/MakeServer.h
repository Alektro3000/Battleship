#include "ServerConnections.h"
#include "../Widget.h"
#include "../Base/Builder.h"
#include "../../Players/NetPlayer.h"
#ifndef WidgetSelectMakeServerH
#define WidgetSelectMakeServerH

namespace widget
{

  inline auto editableText()
  {
    return Builder::makeText(L"1").addButton([](auto _) {}).build();
  }

  class MakeServer : public Overlay<Button<TextBox>>
  {
    decltype(editableText()) text = editableText();
    constexpr static RectF TextBegin = {{0.05, 0.4}, {0.4, 0.48}};
    constexpr static RectF ButtonPos = {{0.8, 0.88}, {0.98, 0.99}};
    std::future<boost::asio::ip::address_v4> server;
    GameRules rules;
    std::atomic<bool> isFutureReady = false;
    std::atomic<boost::asio::io_context*> contextPointer = nullptr; //Needed for faster shutting down
    bool isServerEnabled = false;

  public:
    MakeServer() : Overlay({ButtonPos,
                                    Button{TextBox(L"Создать сервер", 40), [this](auto _)
                                                 { makeServer(); }}}) {};
    MakeServer(const MakeServer&) = delete;
    MakeServer& operator=(const MakeServer&) = delete;
    void onChar(WCHAR letter) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void makeServer();
    ~MakeServer()
    {
      if(contextPointer)
        (contextPointer.load())->stop();
    }
  };
}
#endif