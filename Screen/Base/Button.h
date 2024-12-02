#include "../Screen.h"

#ifndef ButtonScreenH
#define ButtonScreenH


class ButtonScreen : public Screen
{
    std::unique_ptr<Screen> child;
    std::function<void(Button)> callback;
public:
    ButtonScreen(std::unique_ptr<Screen> child, std::function<void(Button)> callback):
        child(std::move(child)), callback(std::move(callback)) {};
    void onClick(Button button) override
    {
        callback(button);
    }
    void onResize(RectF newSize) override;
    void onRender() override;
    void init(RenderThings renderer) override;
};
#endif