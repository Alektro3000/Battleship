#include "ServerConnections.h"
#include "../Base/Builder.h"
#include "../Base/WidgetPtr.h"
#include "../../Players/NetPlayer.h"
#include "../Battle/RulesSelect.h"
#ifndef WidgetSelectMakeServerH
#define WidgetSelectMakeServerH

namespace widget
{


  class MakeServer final: public Overlay<Button<TextBox>, RulesSelect , TextBox, Padder<TextBox> >
  {

    using SocketContext = std::pair<std::optional<tcp::socket>, std::unique_ptr<boost::asio::io_context> >; 
    GameRules rules;
    std::atomic<bool> isFutureReady = false;
    //Needed for faster shutting down
    std::atomic<boost::asio::io_context*> contextPointer = nullptr; 
    bool isServerEnabled = false;
    //Must be destroyed first 
    std::future<SocketContext > server;

  public:
    MakeServer();
    MakeServer(const MakeServer&) = delete;
    MakeServer& operator=(const MakeServer&) = delete;
    void onRender() override;
    void makeServer();
    void onChar(wchar_t key) override;
    void onClickUp(MouseButton mouse) override;
    void onClickDown(MouseButton mouse) override;
    ~MakeServer()
    {
      
      if(contextPointer)
        (contextPointer.load())->stop();
      contextPointer = nullptr;
        
    }
  };
}
#endif