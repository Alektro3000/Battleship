#include "Widget.h"

#ifndef WidgetPtrH
#define WidgetPtrH

namespace widget
{
    class WidgetPtr final : public IWidget
    {
    public:
        std::unique_ptr<IWidget> widget = nullptr; 
        WidgetPtr(){};
        WidgetPtr(std::unique_ptr<IWidget> widget) : widget(std::move(widget)){};
        WidgetPtr(WidgetPtr&& other) = default;
        WidgetPtr& operator =(WidgetPtr&& other) = default;
        RectF getPosition() const override {
            if(!widget)
                return {0,0};
            return widget->getPosition();
        }
        void onResize(RectF newSize) override { 
            if(widget)
                widget->onResize(newSize);
        }
        void onRender() override { 
            if(widget)
            widget->onRender();
        }
        void onClickDown(MouseButton button) override { 
            if(widget)
            widget->onClickDown(button);
        }
        void onClickUp(MouseButton button) override { 
            if(widget)
                widget->onClickUp(button);
        }
        void onChar(WCHAR button) override { 
            if(widget)
                widget->onChar(button);
        }
    };
}
#endif