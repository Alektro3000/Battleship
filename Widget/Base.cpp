#include "Base.h"

ID2D1Bitmap *Context::getBitmapFromFile(const wchar_t *uri)
{
    if (loaded.contains(uri))
        return loaded.at(uri);
    auto bitmap = loadBitmapFromFile(uri);
    loaded.try_emplace(uri, bitmap);
    return bitmap;
}

ID2D1Bitmap *Context::loadBitmapFromFile(const wchar_t *uri)
{
    IWICBitmapDecoder *pDecoder = nullptr;
    IWICBitmapFrameDecode *pSource = nullptr;
    IWICFormatConverter *Converter = nullptr;
    IWICStream *pStream = nullptr;

    HRESULT hr = WICFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (SUCCEEDED(hr))
        hr = pDecoder->GetFrame(0, &pSource);

    if (SUCCEEDED(hr))
        hr = WICFactory->CreateFormatConverter(&Converter);

    if (SUCCEEDED(hr))
        hr = Converter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);

    ID2D1Bitmap *bitmap = nullptr;
    if (SUCCEEDED(hr))
        hr = RenderTarget->CreateBitmapFromWicBitmap(Converter, nullptr, &bitmap);

    // Release resources
    if (pDecoder)
        pDecoder->Release();
    if (pSource)
        pSource->Release();
    if (pStream)
        pStream->Release();
    if (Converter)
        Converter->Release();

    return bitmap;
}

void Context::init(HWND hwnd)
{
    hWnd = hwnd;
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
}
void Context::clear()
{

    for (auto q : loaded)
        q.second->Release();
    loaded.clear();

    if (WICFactory)
    {
        WICFactory->Release();
        WICFactory = nullptr;
    }
    if (WriteFactory)
    {
        WriteFactory->Release();
        WriteFactory = nullptr;
    }
    if (RenderTarget)
    {
        RenderTarget->Release();
        RenderTarget = nullptr;
    }
    if (D2DFactory)
    {
        D2DFactory->Release();
        D2DFactory = nullptr;
    }
}

D2D1_RECT_F makeD2DRectF(RectF rect)
{
    return D2D1::RectF(rect.low.x, rect.high.y, rect.high.x, rect.low.y);
}

D2D1_POINT_2F makeD2DPointF(PointF p)
{
    return {p.x, p.y};
}
PointF makePointF(POINT p)
{
    return PointF{static_cast<float>(p.x), static_cast<float>(p.y)};
}

PointF makePointF(D2D1_SIZE_U p)
{
    return PointF{static_cast<float>(p.width), static_cast<float>(p.height)};
}

PointF makePointF(D2D1_SIZE_F p)
{
    return {p.width, p.height};
}
const WCHAR TextFormat::fontName[8] = L"Verdana";

bool operator==(D2D1::ColorF a, D2D1::ColorF b)
{
    return a.a == b.a && a.r == b.r && a.g == b.g && a.b == b.b;
}

SolidBrush::SolidBrush(D2D1::ColorF color)
{
    Context::getInstance().getRenderTarget()->CreateSolidColorBrush(
        color,
        &brush);
}
SolidBrush::SolidBrush(SolidBrush &&other)
{
    brush = other.brush;
    other.brush = nullptr;
};

SolidBrush::~SolidBrush()
{
    if (brush)
        brush->Release();
}
