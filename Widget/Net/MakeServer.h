#include "../Widget.h"
#include "../Base/Builder.h"
#include <boost/asio/io_context.hpp>

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
    std::jthread server;
    class boost::asio::io_context context;
    bool isServerEnabled = false;

  public:
    MakeServer() : Overlay({ButtonPos,
                                    Button{TextBox(L"Создать сервер", 40), [this](auto _)
                                                 { makeServer(); }}}) {};
    void onChar(WCHAR letter) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void makeServer();
  };
}
#endif