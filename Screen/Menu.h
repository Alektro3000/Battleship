#include "Base/Button.h"
#include "Base/TextBox.h"
#include "ScreenOverlay.h"

#ifndef MenuScreenH
#define MenuScreenH

class MenuScreen : public ScreenOverlay<ButtonScreen<TextBox>, ButtonScreen<TextBox> >
{
public:
    MenuScreen();
    void onRender() override;
};
#endif