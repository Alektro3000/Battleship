#include "../WidgetOverlay.h"

#ifndef ButtonWidgetH
#define ButtonWidgetH

/*

class ButtonWidget final : public Widget 
{
    std::unique_ptr<Widget> child;
    std::function<void(Button)> callback;
public:
    ButtonWidget(std::unique_ptr<Widget> child, std::function<void(Button)> callback):
        child(std::move(child)), callback(std::move(callback)) {};
    void onClick(Button button) override
    {
        callback(button);
    }
    void onResize(RectF newSize) override;
    void onRender() override;
};
*/


template<TWidget Child>
class ButtonWidget final : public WidgetOverlay<Child> 
{
    std::function<void(Button)> callback;
public:
    ButtonWidget(Child&& child, auto&& callback):
        WidgetOverlay<Child>({RectF{{0},{1}},std::move(child)}), callback(std::move(callback)) {};
    void onClick(Button button) override
    {
        callback(button);
    }
};
#endif