#include "Point.h"
Point operator/ (Point left, int right)
{
    return Point(left.x/right,left.y/right);
}

std::strong_ordering operator<=> (Point left, Point right)
{
    if(left.x == right.x)
        return left.y <=> right.y;
    else
        return left.x <=> right.x;
}

Point operator- (Point left, Point right)
{
    return Point(left.x-right.x, left.y-right.y);
}
Point operator- (Point left)
{
    return Point(-left.x, -left.y);
}