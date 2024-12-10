#include "TextBox.h"
#include <algorithm>

namespace widget
{

    void TextBox::onResize(RectF newSize)
    {
        Widget::onResize(newSize);
        if (size == -1)
        {
            format = TextFormat(0.125f);
            auto metric = format.getTextMetrix(string, newSize.size());
            //Limit from width
            auto maxWidth = format.textFormat->GetFontSize() *
                        (newSize.size().x * 0.99f) / metric.width / static_cast<float>(metric.lineCount);
            //Limit from height
            auto maxHeight = format.textFormat->GetFontSize() *
                        (newSize.size().y * 0.99f) / metric.height / static_cast<float>(metric.lineCount);
            format = TextFormat(min(maxWidth,maxHeight));
        }
    }
    void TextBox::onRender()
    {
        Widget::onRender();
        Context::getInstance().getRenderTarget()->DrawText(string.c_str(),
                                                           string.size(), format, makeD2DRectF(position), brush);
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
            auto pos = position.size();
            IDWriteTextLayout *layout;
            Context::getInstance().getWriteFactory()->CreateTextLayout(string.c_str(), string.size(), format, pos.x, pos.y, &layout);
            DWRITE_TEXT_METRICS lam;
            layout->GetMetrics(&lam);
            layout->Release();
            if (lam.height > pos.y)
                string.pop_back();
        }
    }
}