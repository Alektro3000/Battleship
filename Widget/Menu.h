#include "Base/Button.h"
#include "Base/TextBox.h"
#include "WidgetOverlay.h"

#ifndef MenuWidgetH
#define MenuWidgetH

class MenuWidget : public WidgetOverlay<ButtonWidget<TextBox>, ButtonWidget<TextBox> >
{
public:
    MenuWidget();
    void onRender() override;
};
#endif