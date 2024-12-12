#include "../Widget.h"
#include <tuple>
#include <utility>

#ifndef OverlayWidgetH
#define OverlayWidgetH
namespace widget
{
    
    template <TWidget... Args>
    class Overlay : public Widget
    {
        std::tuple<Args...> widgets;
        std::array<RectF, sizeof...(Args)> positions;
    public:
        Overlay(Overlay &&other) = default;
        Overlay &operator=(Overlay &&other) = default;
        Overlay(std::pair<RectF, Args &&>... args) : widgets(std::move(args.second)...), positions((args.first)...)
        {
        }
        Overlay(Args &&...args) : widgets(std::move(args)...), positions(RectF{{0}, {1}})
        {
        }
        void onResize(RectF newSize) override
        {
            Widget::onResize(newSize);
            helper([newSize](RectF rect, auto &scr)
                   { scr.onResize(rect.scaled(newSize)); });
        };
        void onRender() override
        {
            Widget::onRender();
            helper([](RectF rect, auto &scr)
                   { scr.onRender(); });
        };
        void onClick(MouseButton button) override
        {
            Widget::onClick(button);
            helper([button](RectF rect, auto &scr)
                   { scr.tryClick(button); });
        };
        void onClickUp(MouseButton button) override
        {
            Widget::onClickUp(button);
            helper([button](RectF rect, auto &scr)
                   { scr.tryClickUp(button); });
        };
        void onChar(WCHAR letter) override
        {
            Widget::onChar(letter);
            helper([letter](RectF rect, auto &scr)
                   { scr.onChar(letter); });
        }

        template <std::size_t I = 0>
        auto& getWidget()
        {
            return std::get<I>(widgets);
        }
    private:
        template <std::size_t I = 0, typename F>
        void helper(const F &function)
        {
            if constexpr (I < sizeof...(Args))
            {
                function((positions)[I], std::get<I>(widgets));
                helper<I + 1>(function);
            }
        }
    };
}
#endif