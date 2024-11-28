#include <d2d1.h>
#include <d2d1helper.h>
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include "Screen/Screen.h"

#ifndef DXAppH
#define DXAppH

class DXApp
{
private:


public:
    std::unique_ptr<Screen> currentScreen;
    ID2D1Factory *pD2DFactory = nullptr;
    ID2D1HwndRenderTarget *pRT = nullptr;

    PointF size;

    DXApp(HWND hWnd)
    {
        static_assert(sizeof(float) == sizeof(FLOAT));

        HRESULT hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory);

        RECT rc;
        GetClientRect(hWnd, &rc);

        hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top)),
            &pRT);
    }
    void changeScreen(std::unique_ptr<Screen> newScreen)
    {
        currentScreen = std::move(newScreen);
        currentScreen->Init(pRT);
        currentScreen->onResize();
    }
    void onWinClick(POINT point, Button button)
    {
        currentScreen->onClick(button);
    }
    void onWinClickUp(Button button) {
        currentScreen->onClickUp(button);
    };
    void renderFrame()
    {
        currentScreen->onRender();
    }

    ~DXApp()
    {
        pRT->Release();
        

        pD2DFactory->Release();
    }
};
#endif