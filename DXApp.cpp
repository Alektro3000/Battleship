
#include "DXApp.h"

ID2D1Factory *D2DFactory = nullptr;
ID2D1HwndRenderTarget *RenderTarget = nullptr;
IDWriteFactory *WriteFactory = nullptr;

ID2D1Factory *GetD2DFactory() { return D2DFactory; };
ID2D1HwndRenderTarget *GetRenderTarget() { return RenderTarget; };
IDWriteFactory *GetWriteFactory() { return WriteFactory; };

DXApp::DXApp(HWND hWnd) : hWnd(hWnd)
{
    {
        static_assert(sizeof(float) == sizeof(FLOAT));

        HRESULT hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &D2DFactory);

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(WriteFactory),
            reinterpret_cast<IUnknown **>(&WriteFactory));
        hr = CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory2),
                              reinterpret_cast<LPVOID *>(&WICFactory));

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
}
void DXApp::onWinResize(UINT32 width, UINT32 height)
{

    RenderTarget->Resize(D2D1::SizeU(width, height));
    size = makePointF(RenderTarget->GetSize());
    currentWidget->onResize({{0}, size});
};

void DXApp::renderFrame()
{
    RenderTarget->BeginDraw();

    currentWidget->onRender();
#ifdef LiveCheck
    SolidBrush brush{RenderTarget, D2D1::ColorF(rand(), 0.5)};
    RenderTarget->FillRectangle(makeD2DRectF({{0, 0}, {100, 100}}), brush);
#endif
    HRESULT hr = RenderTarget->EndDraw();
}

DXApp::~DXApp()
{
    // if(WICFactory) WICFactory->Release();
    if (WriteFactory)
        WriteFactory->Release();
    if (RenderTarget)
        RenderTarget->Release();
    if (D2DFactory)
        D2DFactory->Release();
}