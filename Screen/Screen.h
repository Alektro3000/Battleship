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

class Screen
{
protected:
    PointF getCursor() const
    {
        POINT p;
        if(!GetCursorPos(&p))
            throw std::runtime_error("error getting cursor");
        return makePointF(p) *  makePointF(render.RenderTarget->GetSize())/ makePointF(render.RenderTarget->GetPixelSize());
    }
    RectF position;
    RenderThings render;
    using callback = std::function<void(std::unique_ptr<Screen>)>;
    callback changeScreenCallback;
public:
    RectF getPosition()
    {
        return position;
    }

    void changeScreenSetup(callback func)
    {
        changeScreenCallback = std::move(func);
    }
    virtual void onResize(RectF newSize)  {position = newSize; };
    virtual void onRender()  { };
    virtual void onClick(Button button) {};
    bool tryClick(Button button)
    {
        if(position.isPointInsideExcl(getCursor()))
        {
            onClick(button);
            return true;
        }
        return false;
    }
    virtual void onClickUp(Button button) {};
    virtual void onChar(WCHAR letter) {};
    bool tryClickUp(Button button)
    {
        if(position.isPointInsideExcl(getCursor()))
        {
            onClickUp(button);
            return true;
        }
        return false;
    }
    virtual void init(RenderThings npRT) {render = npRT;};
    
    virtual ~Screen(){};

};
#endif