#include "Widget.h"
#include <tuple>
#include <utility>

#ifndef OverlayWidgetH
#define OverlayWidgetH
namespace widget
{
    
    template <TWidget... Args>
    class Overlay : public Widget
    {
    protected:
        std::tuple<Args...> widgets;
        std::array<RectF, sizeof...(Args)> positions;
        template <std::size_t I = 0>
        auto& getWidget(){
            return std::get<I>(widgets);
        }
        Overlay(Overlay&& other) = default;
        Overlay& operator =(Overlay&& other) = default;
    public:

        Overlay(std::pair<RectF, Args &&>... args) : widgets(std::move(args.second)...), positions((args.first)...)
        {
        }
        Overlay(Args &&...args) : widgets(std::move(args)...), positions(RectF{{0}, {1}})
        {
        }
        void onResize(RectF newSize) override
        {
            Widget::onResize(newSize);
            forEach([newSize](RectF rect, auto &scr)
                   { scr.onResize(rect.scaled(newSize)); });
        };
        void onRender() override
        {
            Widget::onRender();
            forEach([](RectF rect, auto &scr)
                   { scr.onRender(); });
        };
        void onClickDown(MouseButton button) override
        {
            Widget::onClickDown(button);
            forEach([button](RectF rect, auto &scr)
                   { scr.tryClickDown(button); });
        };
        void onClickUp(MouseButton button) override
        {
            Widget::onClickUp(button);
            forEach([button](RectF rect, auto &scr)
                   { scr.tryClickUp(button); });
        };
        void onChar(WCHAR letter) override
        {
            Widget::onChar(letter);
            forEach([letter](RectF rect, auto &scr)
                   { scr.onChar(letter); });
        }

    private:
        template <std::size_t I = 0, typename F>
        void forEach(const F &function)
        {
            if constexpr (I < sizeof...(Args))
            {
                function((positions)[I], std::get<I>(widgets));
                forEach<I + 1>(function);
            }
        }
    };
    template <TWidget... Args>
    class OverlayFinal final : public Overlay<Args...>
    {
    public:
        using Overlay<Args...>::Overlay;
        using Overlay<Args...>::operator=;
    };
}
#endif