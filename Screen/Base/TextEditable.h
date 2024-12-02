#include "TextBox.h"

#ifndef TextEditableH
#define TextEditableH
class TextEditable : public TextBox
{
public:
    template<typename ...Args>
    TextEditable( Args&&... arg) : TextBox(std::forward<Args>(arg)...) {};
    void onChar(WCHAR letter) override;
};
#endif