#include "Base.h"

D2D1_RECT_F makeD2DRectF(RectF rect)
{
    return D2D1::RectF(rect.low.x, rect.high.y, rect.high.x, rect.low.y);
}

PointF makePointF(POINT p)
{
    return PointF{static_cast<float>(p.x), static_cast<float>(p.y)};
}

PointF makePointF(D2D1_SIZE_U p)
{
    return PointF{static_cast<float>(p.width), static_cast<float>(p.height)};
}

PointF makePointF(D2D1_SIZE_F p)
{
    return {p.width, p.height};
}