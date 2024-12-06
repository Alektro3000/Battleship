#include "Base.h"
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include <functional>

#ifndef WidgetH
#define WidgetH
enum class Button
{
    left,
    right,
};
class IWidget
{
public:
    virtual void onResize(RectF newSize) {};
    virtual void onRender() {};
    virtual void onClick(Button button) {};
    virtual void onClickUp(Button button) {};
    virtual void onChar(WCHAR letter) {};
    virtual ~IWidget() {};
};

void ChangeWidget(std::unique_ptr<IWidget> NewWidget, bool pushToStackPrev = true);

PointF getCursor();

template<typename T>
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
    bool tryClick(Button button)
    {
        if (position.isPointInsideExcl(getCursor()))
        {
            onClick(button);
            return true;
        }
        return false;
    }
    bool tryClickUp(Button button)
    {
        if (position.isPointInsideExcl(getCursor()))
        {
            onClickUp(button);
            return true;
        }
        return false;
    }
};
#endif