#include <d2d1.h>
#include <d2d1helper.h>
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include "Players/Player.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

inline  PointF MakePointF(D2D1_SIZE_F p)
{
    return {p.width, p.height};
}
inline PointF MakePointF(D2D1_SIZE_U p)
{
    return PointF{static_cast<float>(p.width), static_cast<float>(p.height)};
}


class DXApp
{

public:
    ID2D1Factory *pD2DFactory = nullptr;
    ID2D1HwndRenderTarget *pRT = nullptr;
    ID2D1SolidColorBrush *pBlackBrush = nullptr;
    ID2D1SolidColorBrush *pRedBrush = nullptr;
    RECT rc;
    int size = 30;
    PointF Scaling;

    DXApp() {};
    // Constructor
    DXApp(HWND hWnd)
    {
        static_assert(sizeof(float) == sizeof(FLOAT));

        HRESULT hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory);

        GetClientRect(hWnd, &rc);

        hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top)),
            &pRT);
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush);
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Red),
            &pRedBrush);
        auto a = MakePointF(pRT->GetSize());
        auto b = MakePointF(pRT->GetPixelSize());
        Scaling =  a / b;
    }

    ~DXApp()
    {
        pRT->Release();
        pBlackBrush->Release();
        pRedBrush->Release();

        pD2DFactory->Release();
    }
    void InitBackBuffer();
    void DrawField(D2D_SIZE_U offset, D2D_SIZE_U size)
    {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                pRT->DrawRectangle(
                    D2D1::RectF(
                        offset.height + i * size.height,
                        offset.width + j * size.width,
                        offset.height + (i + 1) * size.height,
                        offset.width + (j + 1) * size.width),
                    pBlackBrush);
    }
    std::set<Point> points;
    void OnClick(Point Point)
    {
        if(!points.contains(Point*Scaling /30))
            points.insert(Point*Scaling/30);
        else
            points.erase(Point*Scaling/30);
    };
    void OnClickUp() {
    };
    // this is the function used to render a single frame
    void RenderFrame()
    {
        pRT->BeginDraw();

        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        auto size = pRT->GetPixelSize();

        DrawField(D2D1::SizeU(210, 300), D2D1::SizeU(30, 30));
        DrawField(D2D1::SizeU(210, 900), D2D1::SizeU(30, 30));

        {
            POINT p;
            GetCursorPos(&p);
            int i = p.x * Scaling.x / 30;
            int j = p.y * Scaling.y / 30;
            pRT->FillRectangle(
                D2D1::RectF(
                    i * 30,
                    j * 30,
                    (i + 1) * 30,
                    (j + 1) * 30),
                pBlackBrush);
            for (auto q : points)
            {
                pRT->FillRectangle(
                    D2D1::RectF(
                        q.x * 30,
                        q.y * 30,
                        (q.x + 1) * 30,
                        (q.y + 1) * 30),
                    pRedBrush);
            }
        }
        HRESULT hr = pRT->EndDraw();
    }
};
