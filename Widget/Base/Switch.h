#include "Stack.h"

#ifndef SwitchWidgetH
#define SwitchWidgetH

namespace widget {

    //widget that add possibility to turn off rendering of widget
    template<TWidget Child>
    class Switch final : public Stack<Child>
    {
        bool enabled = true;
    public:
        Switch(Child&& child, bool enabled = true):
            Stack<Child>(std::move(child)), enabled(enabled) {};
        Switch(Switch&& other) = default;
        Switch& operator =(Switch&& other) = default;
        void onRender() override
        {   
            if(enabled)
                Stack<Child>::onRender();
        }
        void setEnabled(bool enable)
        {
            enabled = enable;
        }
    };
}
#endif