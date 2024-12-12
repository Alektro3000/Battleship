#include "../Widget.h"
#include <tuple>
#include <utility>

#ifndef StackWidgetH
#define StackWidgetH

namespace widget
{
    
    //widget that contains several widgets fullsized, and will forward all events to childs
    template <TWidget... Args>
    class Stack: public Widget
    {
    public:
        std::tuple<Args...> widgets;

        Stack(Stack &&other) = default;
        Stack &operator=(Stack &&other) = default;
        Stack(Args &&...args) : widgets(std::move(args)...)
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
        auto& getChild() requires (sizeof...(Args) == 1)
        {
            return std::get<0>(widgets);
        }
    private:
        template <std::size_t I = 0, typename F>
        void helper(const F &function)
        {
            if constexpr (I < sizeof...(Args))
            {
                function(std::get<I>(widgets));
                helper<I + 1>(function);
            }
        };
    };

}
#endif