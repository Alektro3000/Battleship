#include "Screen/Screen.h"
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
    std::unique_ptr<IScreen> currentScreen;
    IWICImagingFactory* WICFactory = nullptr;
    HWND hWnd;

    PointF size;

    DXApp(HWND hWnd);
    void changeScreen(std::unique_ptr<IScreen> newScreen, bool pushToStackPrev = true)
    {
        currentScreen = std::move(newScreen);
        //currentScreen->changeScreenSetup([this](auto screen){ changeScreen(std::move(screen)); });
        currentScreen->onResize({{0,0},size});
    }
    void onWinClick(POINT point, Button button)
    {
        currentScreen->onClick(button);
    }
    void onWinClickUp(Button button) {
        currentScreen->onClickUp(button);
    };
    void onWinChar(WCHAR param) {
        currentScreen->onChar(param);
    };
    void onWinResize(UINT32 width, UINT32 height);
    void renderFrame();

    ~DXApp();
};
#endif