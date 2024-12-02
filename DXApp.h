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
    std::unique_ptr<Screen> currentScreen;
    ID2D1Factory *D2DFactory = nullptr;
    IWICImagingFactory* WICFactory = nullptr;
    ID2D1HwndRenderTarget *RenderTarget = nullptr;
    IDWriteFactory *WriteFactory;

    PointF size;

    DXApp(HWND hWnd)
    {
        static_assert(sizeof(float) == sizeof(FLOAT));

        HRESULT hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &D2DFactory);

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(WriteFactory),
            reinterpret_cast<IUnknown **>(&WriteFactory)
            );
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&WICFactory));


        RECT rc;
        GetClientRect(hWnd, &rc);

        hr = D2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top)),
            &RenderTarget);
        size = makePointF(RenderTarget->GetSize());
    }
    void changeScreen(std::unique_ptr<Screen> newScreen)
    {
        currentScreen = std::move(newScreen);
        currentScreen->init({D2DFactory, RenderTarget, WriteFactory});
        currentScreen->changeScreenSetup([this](auto screen){ changeScreen(std::move(screen)); });
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
    void renderFrame()
    {
        RenderTarget->BeginDraw();
        
            
        currentScreen->onRender();
        HRESULT hr = RenderTarget->EndDraw();
    }

    ~DXApp()
    {
        if(WriteFactory) WriteFactory->Release();
        if(RenderTarget) RenderTarget->Release();
        if(D2DFactory) D2DFactory->Release();
        if(WICFactory) WICFactory->Release();
    }
};
#endif