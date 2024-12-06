#include "Widget.h"
#include <tuple>
#include <utility> 

#ifndef WidgetOverlayH
#define WidgetOverlayH
template<TWidget ...Args>
class WidgetOverlay : public Widget
{
protected:
    std::tuple<Args...> _widgets;
    std::array<RectF, sizeof...(Args)> _pos;
public:
    WidgetOverlay(std::pair<RectF,Args&&>... args) : _widgets(std::move(args.second)...), _pos((args.first)...) {

    }
    WidgetOverlay(Args&&... args) : _widgets(std::move(args)...), _pos(RectF{{0},{1}}){

    }
    void onResize(RectF newSize) override {
        Widget::onResize(newSize);
        helper([newSize](RectF rect, auto& scr) 
        {scr.onResize(rect.scaled(newSize));});
    };
    void onRender() override {
        Widget::onRender();
        helper([](RectF rect, auto& scr) 
        {scr.onRender();});
    };
    void onClick(Button button) override {
        Widget::onClick(button);
        helper([button](RectF rect, auto& scr) 
        {scr.tryClick(button);});
    };
    void onClickUp(Button button) override {
        Widget::onClickUp(button);
        helper([button](RectF rect, auto& scr) 
        {
            scr.tryClickUp(button);});
        };
    void onChar(WCHAR letter) override {
        Widget::onChar(letter);
        helper([letter](RectF rect, auto& scr) 
        { scr.onChar(letter);});
    }
    private:
    
    template <std::size_t I = 0, typename F >
    void helper(const F& function) {
        if constexpr (I < sizeof...(Args)) {
            function((_pos)[I], std::get<I>(_widgets));
            helper<I + 1>(function);
        }
    }
};
#endif