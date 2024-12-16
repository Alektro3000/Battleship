#include "../Base.h"
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include <functional>

#ifndef WidgetH
#define WidgetH

namespace widget
{
    class IWidget
    {
        friend class Builder;
    protected:
        IWidget(IWidget&& other) = default;
        IWidget& operator =(IWidget&& other) = default;
    public:
        virtual void onResize(RectF newSize) {};
        virtual void onRender() {};
        virtual void onClickDown(MouseButton button) {};
        virtual void onClickUp(MouseButton button) {};
        virtual void onChar(wchar_t letter) {}; 
        virtual RectF getPosition() const {return {0.f,0.f};};
        virtual ~IWidget() {};
        
        IWidget(){};
        IWidget(const IWidget&) = delete;
        IWidget& operator=(const IWidget&) = delete;

        bool tryClickDown(MouseButton button)
        {
            if (!getPosition().isPointInsideExcl(Context::getInstance().getCursor()))
                return false;

            onClickDown(button);
            return true;
        }
        bool tryClickUp(MouseButton button)
        {
            if (!getPosition().isPointInsideExcl(Context::getInstance().getCursor()))
                return false;

            onClickUp(button);
            return true;
        }
    };

    void pushWidget(std::unique_ptr<IWidget> NewWidget, bool pushToStackPrev = true);

    template <typename T>
    concept TWidget = std::derived_from<T, IWidget> && std::is_final_v<T>;

    class Widget : public IWidget
    {
    protected:
        Widget(){};
        Widget(Widget&& other) = default;
        Widget& operator =(Widget&& other) = default;
    
    private:
        RectF position = {0,0};

    public:
        RectF getPosition() const override final
        {
            return position;
        }
        virtual void onResize(RectF newSize) override { position = newSize; };
    };
}
#endif