#include "Base.h"
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include <functional>


#ifndef ScreenH
#define ScreenH
enum class Button 
{
    left,
    right,
};
class IScreen
{
public:
    virtual void onResize(RectF newSize)  {};
    virtual void onRender()  { };
    virtual void onClick(Button button) {};
    virtual void onClickUp(Button button) {};
    virtual void onChar(WCHAR letter) {};
    virtual ~IScreen(){};
};
void ChangeScreen(std::unique_ptr<IScreen> NewScreen, bool pushToStackPrev = true);

class Screen : public IScreen
{
protected:
    PointF getCursor() const
    {
        POINT p;
        if(!GetCursorPos(&p))
            throw std::runtime_error("error getting cursor");
        return makePointF(p) *  makePointF(GetRenderTarget()->GetSize())/ makePointF(GetRenderTarget()->GetPixelSize());
    }
    RectF position;
public:
    RectF getPosition()
    {
        return position;
    }
    virtual void onResize(RectF newSize) override {position = newSize; };
    bool tryClick(Button button)
    {
        if(position.isPointInsideExcl(getCursor()))
        {
            onClick(button);
            return true;
        }
        return false;
    }
    bool tryClickUp(Button button)
    {
        if(position.isPointInsideExcl(getCursor()))
        {
            onClickUp(button);
            return true;
        }
        return false;
    }

};
#endif