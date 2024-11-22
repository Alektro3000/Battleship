#include "Point.h"
#include  <algorithm>

Rect Rect::operator- (Rect left) const
{
    return {low-left.low, high-left.high};
}

Rect Rect::operator& (Rect left) const
{
    return Rect({std::max(low.x,left.low.x),std::max(low.y,left.low.y)}, 
                {std::min(high.x,left.high.x),std::min(high.y,left.high.y)});
}