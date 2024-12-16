#include "Stack.h"

#ifndef ButtonH
#define ButtonH

namespace widget
{
    template <TWidget Child>
    class Button final : public Stack<Child>
    {
        std::function<void(MouseButton)> callbackDown;
        std::function<void(MouseButton)> callbackUp;

    public:
        
        template<std::invocable<MouseButton> FDown, std::invocable<MouseButton> FUp = std::function<void(MouseButton)> >
        Button(Child &&child, FDown &&callbackDown, FUp &&callbackUp) : 
            Stack<Child>(std::move(child)), 
            callbackDown(std::forward<FDown>(callbackDown)), 
            callbackUp(std::forward<FUp>(callbackUp)) {};
        
        Button(Button &&other) = default;
        Button &operator=(Button &&other) = default;

        void onClickDown(MouseButton button) override
        {
            if(callbackDown)
            callbackDown(button);
        }
        void onClickUp(MouseButton button) override
        {
            if(callbackUp)
                callbackUp(button);
        }
    };
}
#endif