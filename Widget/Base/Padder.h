#include "Overlay.h"

#ifndef PadderWidgetH
#define PadderWidgetH

namespace widget {

    template<TWidget Child>
    class Padder final : public Stack<Child>
    {
        RectF padding;
        SolidBrush borderColor;
        SolidBrush backColor;
        int borderWidth;
        int borderRadius;
    public:
        Padder(Child&& child, RectF padding, D2D1::ColorF backColor = D2D1::ColorF(0,0)):
            Stack<Child>(std::move(child)), padding(std::move(padding)), backColor(backColor) {};
        Padder(Padder&& other) = default;
        Padder& operator =(Padder&& other) = default;
        void onResize(RectF newSize) override
        {
            Stack<Child>::onResize(newSize+RectF{padding.low,-padding.high});
            Stack<Child>::position = newSize;
        }
        void setBorder(D2D1::ColorF color, int width = 1, int radius = 10)
        {
            borderColor = SolidBrush(color);
            borderWidth = width;
            borderRadius = radius;
        }
        void onRender() override
        {   
            if(backColor)
                Context::getInstance().getRenderTarget()->FillRoundedRectangle(
                    D2D1::RoundedRect(makeD2DRectF(Widget::position),borderRadius,borderRadius),backColor);

            Stack<Child>::onRender();

            auto opacity = borderColor.brush->GetOpacity();
            if(borderColor && opacity > 0.f)
                Context::getInstance().getRenderTarget()->DrawRoundedRectangle(
                    D2D1::RoundedRect(makeD2DRectF(Widget::position),borderRadius,borderRadius),borderColor);
        }
        Child& getChild()
        {
            return std::get<0>(this->_widgets);
        }
    };
}
#endif