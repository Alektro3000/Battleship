#include "Widget.h"

#ifndef EmptyWidgetH
#define EmptyWidgetH

namespace widget
{
    class Empty final : public Widget
    {
        public:
        Empty(){};
        Empty(Empty&& other) = default;
        Empty& operator =(Empty&& other) = default;
    };
}
#endif