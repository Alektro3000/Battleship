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
        virtual void onClick(MouseButton button) {};
        virtual void onClickUp(MouseButton button) {};
        virtual void onChar(WCHAR letter) {};
        virtual ~IWidget() {};
    };

    void pushWidget(std::unique_ptr<IWidget> NewWidget, bool pushToStackPrev = true);

    template <typename T>
    concept TWidget = std::derived_from<T, IWidget>;

    class Widget : public IWidget
    {
    protected:
        RectF position;

    public:
        RectF getPosition()
        {
            return position;
        }
        virtual void onResize(RectF newSize) override { position = newSize; };
        bool tryClick(MouseButton button)
        {
            if (position.isPointInsideExcl(Context::getInstance().getCursor()))
            {
                onClick(button);
                return true;
            }
            return false;
        }
        bool tryClickUp(MouseButton button)
        {
            if (position.isPointInsideExcl(Context::getInstance().getCursor()))
            {
                onClickUp(button);
                return true;
            }
            return false;
        }
    };
}
#endif