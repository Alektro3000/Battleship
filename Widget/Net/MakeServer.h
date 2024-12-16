#include "ServerConnections.h"
#include "../Base/Builder.h"
#include "../../Players/NetPlayer.h"
#ifndef WidgetSelectMakeServerH
#define WidgetSelectMakeServerH

namespace widget
{

  inline auto editableText()
  {
    return Builder::makeText(L"1").setEditableText().addButton([](auto _) {}).build();
  }

  class MakeServer final: public Overlay<Button<TextBox>>
  {
    decltype(editableText()) text = editableText();
    constexpr static RectF TextBegin = {{0.05, 0.4}, {0.4, 0.48}};
    constexpr static RectF ButtonPos = {{0.7, 0.88}, {0.98, 0.99}};
    using SocketContext = std::pair<std::optional<tcp::socket>, std::unique_ptr<boost::asio::io_context> >; 
    GameRules rules;
    std::atomic<bool> isFutureReady = false;
    //Needed for faster shutting down
    std::atomic<boost::asio::io_context*> contextPointer = nullptr; 
    bool isServerEnabled = false;
    //Must be destroyed first 
    std::future<SocketContext > server;

  public:
    MakeServer() : Overlay({ButtonPos, Builder::makeText(L"Создать сервер").addButton([this](auto _)
                                                 { makeServer(); }).build()}) {};
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
      contextPointer = nullptr;
        
    }
  };
}
#endif