#include "../ScreenOverlay.h"

#ifndef ButtonScreenH
#define ButtonScreenH

/*

class ButtonScreen final : public Screen 
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
};
*/


template<typename T>
class ButtonScreen final : public ScreenOverlay<T> 
{
    std::function<void(Button)> callback;
public:
    ButtonScreen(T&& child, auto&& callback):
        ScreenOverlay<T>({RectF{{0},{1}},std::move(child)}), callback(std::move(callback)) {};
    void onClick(Button button) override
    {
        callback(button);
    }
};
#endif