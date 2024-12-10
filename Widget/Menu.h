#include "Base/Builder.h"

#ifndef MenuWidgetH
#define MenuWidgetH

namespace widget
{
    class Menu : public Overlay<Button<Padder<TextBox>>,
                                            Button<Padder<TextBox>>,
                                            Button<Padder<TextBox>>>
    {
    public:
        Menu();
        void onRender() override;
    };
};

#endif