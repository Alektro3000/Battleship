#include <d2d1.h>
#include <d2d1helper.h>
#include <vector>
#include <set>
#include <algorithm>
#include <compare>
#include <stdexcept>
#include "../Visual/Base.h"


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
        return makePointF(p) * scaling;
    }
    PointF size;
    PointF scaling;
    ID2D1HwndRenderTarget *pRT;
public:
    virtual void onResize()  {
        size = makePointF(pRT->GetSize());
        auto b = makePointF(pRT->GetPixelSize());
        scaling = size / b;
        };
    virtual void onRender()  { };
    virtual void onClick(Button button) {};
    virtual void onClickUp(Button button) {};
    void Init(ID2D1HwndRenderTarget *npRT)
    {
        pRT = npRT;
    }
    virtual ~Screen(){};

};
#endif