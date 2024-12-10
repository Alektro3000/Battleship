#include "Overlay.h"

#ifndef ButtonH
#define ButtonH

namespace widget
{
    template <TWidget Child>
    class Button final : public Stack<Child>
    {
        std::function<void(MouseButton)> callback;

    public:
        Button(Child &&child, auto &&callback) : 
            Stack<Child>(std::move(child)), callback(std::move(callback)) {};
        Button(Button &&other) = default;
        Button &operator=(Button &&other) = default;
        void onClick(MouseButton button) override
        {
            callback(button);
        }
        Child& getChild()
        {
            return std::get<0>(this->_widgets);
        }
    };
}
#endif