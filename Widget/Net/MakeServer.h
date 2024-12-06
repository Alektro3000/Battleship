#include "../Widget.h"
#include "../Base/TextEditable.h"
#include "../Base/Button.h"
#include "../WidgetOverlay.h"

#ifndef WidgetSelectMakeServerH
#define WidgetSelectMakeServerH

struct Response
{
  std::array<wchar_t, 32> name;
  int len;
};


class ServerWidget : public WidgetOverlay<ButtonWidget<TextBox> >
{
    TextEditable text;
    constexpr static RectF TextBegin = {{0.05,0.4},{0.4,0.48}};
    constexpr static RectF ButtonPos = {{0.8,0.88},{0.98,0.99}};
public:
    ServerWidget() : WidgetOverlay({ButtonPos,
      ButtonWidget{TextBox(L"Создать сервер",40),[this](auto _){makeServer();}}}), 
        text(L"",40,D2D1::ColorF(D2D1::ColorF::Black)){};
    void onChar(WCHAR letter) override;
    void onResize(RectF newSize) override;
    void onRender() override;
    void makeServer();
    
};
#endif