#include "Widget/Widget.h"
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>

#ifndef DXAppH
#define DXAppH

class DXApp
{

public:
    std::unique_ptr<IWidget> currentWidget;
    IWICImagingFactory* WICFactory = nullptr;
    HWND hWnd;

    PointF size;

    DXApp(HWND hWnd);
    void changeWidget(std::unique_ptr<IWidget> newWidget, bool pushToStackPrev = true)
    {
        currentWidget = std::move(newWidget);
        //currentWidget->changeWidgetSetup([this](auto widget){ changeWidget(std::move(widget)); });
        currentWidget->onResize({{0,0},size});
    }
    void onWinClick(POINT point, Button button)
    {
        currentWidget->onClick(button);
    }
    void onWinClickUp(Button button) {
        currentWidget->onClickUp(button);
    };
    void onWinChar(WCHAR param) {
        currentWidget->onChar(param);
    };
    void onWinResize(UINT32 width, UINT32 height);
    void renderFrame();

    ~DXApp();
};
#endif