#include "Base.h"

HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, const wchar_t *uri, ID2D1Bitmap* *ppBitmap)
{
    IWICBitmapDecoder *pDecoder = nullptr;
    IWICBitmapFrameDecode *pSource = nullptr;
    IWICStream *pStream = nullptr;
    IWICFormatConverter *pConverter = nullptr;
    IWICBitmapScaler *pScaler = nullptr;

    HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (SUCCEEDED(hr))
        hr = pDecoder->GetFrame(0, &pSource);
    
    if (SUCCEEDED(hr))
        hr = pIWICFactory->CreateFormatConverter(&pConverter);

    if (SUCCEEDED(hr))
        hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
    
    if (SUCCEEDED(hr))
        hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);
    

    // Release resources
    if (pDecoder)
        pDecoder->Release();
    if (pSource)
        pSource->Release();
    if (pStream)
        pStream->Release();
    if (pConverter)
        pConverter->Release();
    if (pScaler)
        pScaler->Release();

    return hr;
}

D2D1_RECT_F makeD2DRectF(RectF rect)
{
    return D2D1::RectF(rect.low.x, rect.high.y, rect.high.x, rect.low.y);
}

D2D1_POINT_2F makeD2DPointF(PointF p)
{
    return {p.x,p.y};
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