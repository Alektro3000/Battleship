#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <dwrite.h>
#include <d2d1.h>
#include <utility>
#include <wincodec.h>
#include <comdef.h>
#include "../Players/Player.h"
#include "../Math/Rect.h"
#include <boost/unordered_map.hpp>

#ifndef BaseWidgetH
#define BaseWidgetH

PointF makePointF(D2D1_SIZE_F p);
PointF makePointF(D2D1_SIZE_U p);
PointF makePointF(POINT p);

D2D1_POINT_2F makeD2DPointF(PointF p);

D2D1_RECT_F makeD2DRectF(RectF rect);

bool operator==(D2D1::ColorF a, D2D1::ColorF b);

enum class MouseButton
{
    left,
    right,
};

struct Context
{
    ID2D1Factory *D2DFactory = nullptr;
    ID2D1HwndRenderTarget *RenderTarget = nullptr;
    IDWriteFactory *WriteFactory = nullptr;
    IWICImagingFactory *WICFactory = nullptr;
    HWND hWnd = nullptr;

    boost::unordered_map<std::wstring, ID2D1Bitmap *> loaded;
    ID2D1Bitmap *loadBitmapFromFile(const wchar_t *path);

    ID2D1HwndRenderTarget *getRenderTarget()
    {
        return RenderTarget;
    }
    IDWriteFactory *getWriteFactory()
    {
        return WriteFactory;
    }
    static Context &getInstance()
    {
        static Context instance;
        return instance;
    }
    PointF getCursor()
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hWnd, &p);
        return (makePointF(p))*makePointF(RenderTarget->GetSize()) / makePointF(RenderTarget->GetPixelSize());
    }
    ID2D1Bitmap *getBitmapFromFile(const wchar_t *path);
    ~Context()
    {
        clear();
    }
    void clear();
    void init(HWND hwnd);
};

struct SolidBrush
{
    ID2D1SolidColorBrush *brush;
    SolidBrush(D2D1::ColorF color = D2D1::ColorF(0));
    SolidBrush(const SolidBrush &) = delete;
    SolidBrush &operator=(const SolidBrush &) = delete;

    SolidBrush(SolidBrush &&other);
    SolidBrush &operator=(SolidBrush &&other)
    {
        std::swap(other.brush, brush);
        return *this;
    };
    operator ID2D1SolidColorBrush *() const
    {
        return brush;
    }

    ~SolidBrush();
};

struct TextFormat
{
    IDWriteTextFormat *textFormat;
    static const WCHAR fontName[8];
    TextFormat(float size)
    {
        auto res = Context::getInstance().getWriteFactory()->CreateTextFormat(
            fontName,
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            size,
            L"", // locale
            &textFormat);
        if (!res)
        {
            textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
    }
    DWRITE_TEXT_METRICS getTextMetrix(const std::wstring &string, PointF size)
    {
        IDWriteTextLayout *layout;
        auto hr = Context::getInstance().getWriteFactory()->CreateTextLayout(string.c_str(), string.size(), textFormat, size.x, size.y, &layout);
        DWRITE_TEXT_METRICS lam;
        if(!hr)
        {
            layout->GetMetrics(&lam);
            layout->Release();
        }
        else
            throw std::exception("Failed to get text metrix");
        return lam;
    }
    TextFormat(const TextFormat &) = delete;
    TextFormat &operator=(const TextFormat &) = delete;

    TextFormat(TextFormat &&other)
    {
        textFormat = other.textFormat;
        other.textFormat = nullptr;
    };
    TextFormat &operator=(TextFormat &&other)
    {
        std::swap(other.textFormat, textFormat);
        return *this;
    };
    operator IDWriteTextFormat *()
    {
        return textFormat;
    }

    ~TextFormat()
    {
        if (textFormat)
            textFormat->Release();
    }
};

#endif