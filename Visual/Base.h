#include <d2d1.h>
#include "../Players/Player.h"
#include "../Point.h"

PointF makePointF(D2D1_SIZE_F p);
PointF makePointF(D2D1_SIZE_U p);

PointF makePointF(POINT p);
D2D1_RECT_F makeD2DRectF(RectF rect);