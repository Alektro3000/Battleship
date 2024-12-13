#include "Base.h"
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
    public:
        virtual void onResize(RectF newSize) {};
        virtual void onRender() {};
        virtual void onClickDown(MouseButton button) {};
        virtual void onClickUp(MouseButton button) {};
        virtual void onChar(WCHAR letter) {};
        virtual RectF getPosition() const = 0;
        virtual ~IWidget() {};
        bool tryClickDown(MouseButton button)
        {
            if (getPosition().isPointInsideExcl(Context::getInstance().getCursor()))
            {
                onClickDown(button);
                return true;
            }
            return false;
        }
        bool tryClickUp(MouseButton button)
        {
            if (getPosition().isPointInsideExcl(Context::getInstance().getCursor()))
            {
                onClickUp(button);
                return true;
            }
            return false;
        }
    };

    void pushWidget(std::unique_ptr<IWidget> NewWidget, bool pushToStackPrev = true);

    template <typename T>
    concept TWidget = std::derived_from<T, IWidget>;

    class Widget : public IWidget
    {
    private:
        RectF position;

    public:
        RectF getPosition() const override final 
        {
            return position;
        }
        virtual void onResize(RectF newSize) override { position = newSize; };
    };
}
#endif