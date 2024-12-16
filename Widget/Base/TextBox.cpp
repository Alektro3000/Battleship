#include "TextBox.h"
#include <algorithm>

namespace widget
{

    void TextBox::onResize(RectF newSize)
    {
        Widget::onResize(newSize);
        if (size == -1 && newSize.size().sgn() == PointI{1})
        {
            format = TextFormat(0.125f);
            auto metric = format.getTextMetrix(string, newSize.size());
            // Limit from width
            auto maxWidth = format.textFormat->GetFontSize() *
                            (newSize.size().x * 0.99f) / metric.width;
            // Limit from height
            auto maxHeight = format.textFormat->GetFontSize() *
                             (newSize.size().y * 0.99f) / metric.height;
            format = TextFormat(std::min(maxWidth, maxHeight));
        }
    }
    void TextBox::onRender()
    {
        Widget::onRender();
        Context::getInstance().getRenderTarget()->DrawText(string.c_str(),
                                                           string.size(), format, makeD2DRectF(getPosition()), brush);
    }

    void TextBox::onChar(WCHAR letter)
    {
        if (!isEditable)
            return;

        if (letter == '\b')
        {
            if (string.size())
                string.pop_back();
        }
        else
        {
            string.push_back(letter);
            DWRITE_TEXT_METRICS lam = format.getTextMetrix(string, getPosition().size());
            if (lam.height > getPosition().size().y)
                string.pop_back();
        }
    }
    void TextBox::updateTextColor(D2D1::ColorF newColor)
    {
        brush = SolidBrush(newColor);
    }
    D2D1::ColorF TextBox::getTextColor()
    {
        auto val = brush.brush->GetColor();
        return D2D1::ColorF(val.r, val.g, val.b, val.a);
    }
    std::wstring TextBox::getText()
    {
        return string;
    }
    void TextBox::updateText(std::wstring newText)
    {
        string = newText;
    }
}