#include "Widget/Widget.h"
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include <stack>

#ifndef DXAppH
#define DXAppH

class DXApp
{

public:
    std::stack<std::unique_ptr<widget::IWidget> > Stack;
    std::unique_ptr<widget::IWidget> currentWidget;

    PointF size;

    DXApp(HWND hWnd);
    void changeWidget(std::unique_ptr<widget::IWidget> newWidget, bool pushToStackPrev = true)
    {
        if(pushToStackPrev)
            Stack.push(std::move(currentWidget));
        currentWidget = std::move(newWidget);
        //currentWidget->changeWidgetSetup([this](auto widget){ changeWidget(std::move(widget)); });
        currentWidget->onResize({{0,0},size});
    }
    void popWidget();
    void onWinClick(POINT point, MouseButton button)
    {
        if(!Stack.empty() && RectF{{0},{60}}.isPointInside(Context::getInstance().getCursor()))
            popWidget();
        else
            currentWidget->onClick(button);
    }
    void onWinClickUp(MouseButton button) {
        currentWidget->onClickUp(button);
    };
    void onWinChar(WCHAR param) {
        currentWidget->onChar(param);
    };
    void onWinResize(UINT32 width, UINT32 height);
    void renderFrame();
};
#endif