#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <dwrite.h>
#include <d2d1.h>
#include <utility>
#include <wincodec.h> 
#include <comdef.h>
#include "../Players/Player.h"
#include "../Math/Rect.h"

#ifndef BaseScreenH
#define BaseScreenH
HRESULT LoadBitmapFromFile(ID2D1RenderTarget*, IWICImagingFactory*, const wchar_t*, ID2D1Bitmap**);


PointF makePointF(D2D1_SIZE_F p);
PointF makePointF(D2D1_SIZE_U p);
PointF makePointF(POINT p);


D2D1_POINT_2F makeD2DPointF(PointF p);

D2D1_RECT_F makeD2DRectF(RectF rect);

bool operator==(D2D1::ColorF a, D2D1::ColorF b);


ID2D1Factory* GetD2DFactory();
ID2D1HwndRenderTarget* GetRenderTarget();
IDWriteFactory* GetWriteFactory();

struct SolidBrush
{
    ID2D1SolidColorBrush* brush;
    SolidBrush(D2D1::ColorF color = D2D1::ColorF(0)) {
        GetRenderTarget()->CreateSolidColorBrush(
            color,
            &brush);
    }
    SolidBrush(const SolidBrush&) = delete;
    SolidBrush& operator=(const SolidBrush&) = delete;
    
    SolidBrush(SolidBrush&& other) {
        brush = other.brush;
        other.brush = nullptr;
    };
    SolidBrush& operator=(SolidBrush&& other) {
        std::swap(other.brush,brush);
        return *this;
    };
    operator ID2D1SolidColorBrush*() const
    {
        return brush;
    }

    ~SolidBrush()
    {
        if(brush)
            brush->Release();
    }
};
struct TextFormat
{
    IDWriteTextFormat *textFormat;
    static const WCHAR fontName[8];
    TextFormat(int size) {
        GetWriteFactory()->CreateTextFormat(
            fontName,
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            size,
            L"", //locale
            &textFormat
            );
        textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
    TextFormat(const TextFormat&) = delete;
    TextFormat& operator=(const TextFormat&) = delete;
    
    TextFormat(TextFormat&& other) {
        textFormat = other.textFormat;
        other.textFormat = nullptr;
    };
    TextFormat& operator=(TextFormat&& other) {
        std::swap(other.textFormat,textFormat);
        return *this;
    };
    operator IDWriteTextFormat*()
    {
        return textFormat;
    }

    ~TextFormat()
    {
        if(textFormat)
            textFormat->Release();
    }
};
#endif