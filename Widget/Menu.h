#include "Base/Builder.h"

#ifndef MenuWidgetH
#define MenuWidgetH

namespace widget
{
    class Menu final: public Overlay<Button<Padder<TextBox>>,
                                            Button<Padder<TextBox>>,
                                            Button<Padder<TextBox>>>
    {
    public:
        Menu();
    };
};

#endif