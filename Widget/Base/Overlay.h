#include "../Widget.h"
#include <tuple>
#include <utility>

#ifndef OverlayH
#define OverlayH
namespace widget
{

    template <TWidget... Args>
    class Stack: public Widget
    {
    public:
        std::tuple<Args...> _widgets;

        Stack(Stack &&other) = default;
        Stack &operator=(Stack &&other) = default;
        Stack(Args &&...args) : _widgets(std::move(args)...)
        {
        }
        void onResize(RectF newSize) override
        {
            Widget::onResize(newSize);
            helper([newSize](auto &scr)
                   { scr.onResize(newSize); });
        };
        void onRender() override
        {
            Widget::onRender();
            helper([](auto &scr)
                   { scr.onRender(); });
        };
        void onClick(MouseButton button) override
        {
            Widget::onClick(button);
            helper([button](auto &scr)
                   { scr.onClick(button); });
        };
        void onClickUp(MouseButton button) override
        {
            Widget::onClickUp(button);
            helper([button](auto &scr)
                   { scr.onClickUp(button); });
        };
        void onChar(WCHAR letter) override
        {
            Widget::onChar(letter);
            helper([letter](auto &scr)
                   { scr.onChar(letter); });
        };

    private:
        template <std::size_t I = 0, typename F>
        void helper(const F &function)
        {
            if constexpr (I < sizeof...(Args))
            {
                function(std::get<I>(_widgets));
                helper<I + 1>(function);
            }
        };
    };

    template <TWidget... Args>
    class Overlay : public Widget
    {
        std::tuple<Args...> _widgets;
    public:
        std::array<RectF, sizeof...(Args)> _pos;
        Overlay(Overlay &&other) = default;
        Overlay &operator=(Overlay &&other) = default;
        Overlay(std::pair<RectF, Args &&>... args) : _widgets(std::move(args.second)...), _pos((args.first)...)
        {
        }
        Overlay(Args &&...args) : _widgets(std::move(args)...), _pos(RectF{{0}, {1}})
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
            return std::get<I>(_widgets);
        }
    private:
        template <std::size_t I = 0, typename F>
        void helper(const F &function)
        {
            if constexpr (I < sizeof...(Args))
            {
                function((_pos)[I], std::get<I>(_widgets));
                helper<I + 1>(function);
            }
        }
    };
}
#endif