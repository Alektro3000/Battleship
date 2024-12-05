#include "TextEditable.h"
void TextEditable::onChar(WCHAR letter) 
{
        if(letter == '\b')
        {
            if(string.size())
                string.pop_back();
        }
        else 
        {
            string.push_back(letter);
            auto pos = position.size();
            IDWriteTextLayout* layout;
            GetWriteFactory()->CreateTextLayout(string.c_str(),string.size(),format,pos.x,pos.y, &layout);
            DWRITE_TEXT_METRICS lam;
            layout->GetMetrics(&lam);
            layout->Release();
            if(lam.height > pos.y)
                string.pop_back();
        }
    }