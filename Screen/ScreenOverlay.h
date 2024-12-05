#include "Screen.h"
#include <tuple>
#include <utility> 

#ifndef ScreenOverlayH
#define ScreenOverlayH
template<typename ...Args>
class ScreenOverlay : public Screen
{
protected:
    std::tuple<Args...> _screens;
    std::array<RectF, sizeof...(Args)> _pos;
public:
    ScreenOverlay(std::pair<RectF,Args&&>... args) : _screens(std::move(args.second)...), _pos((args.first)...) {

    }
    ScreenOverlay(Args&&... args) : _screens(std::move(args)...), _pos(RectF{{0},{1}}){

    }
    void onResize(RectF newSize) override {
        Screen::onResize(newSize);
        helper([newSize](RectF rect, auto& scr) 
        {scr.onResize(rect.scaled(newSize));});
    };
    void onRender() override {
        Screen::onRender();
        helper([](RectF rect, auto& scr) 
        {scr.onRender();});
    };
    void onClick(Button button) override {
        Screen::onClick(button);
        helper([button](RectF rect, auto& scr) 
        {scr.tryClick(button);});
    };
    void onClickUp(Button button) override {
        Screen::onClickUp(button);
        helper([button](RectF rect, auto& scr) 
        {
            scr.tryClickUp(button);});
        };
    void onChar(WCHAR letter) override {
        Screen::onChar(letter);
        helper([letter](RectF rect, auto& scr) 
        { scr.onChar(letter);});
    }
    private:
    
    template <std::size_t I = 0, typename F >
    void helper(const F& function) {
        if constexpr (I < sizeof...(Args)) {
            function((_pos)[I], std::get<I>(_screens));
            helper<I + 1>(function);
        }
    }
};
#endif