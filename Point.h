
#include <compare>
#ifndef PointH
#define PointH
struct Point 
{
    int x;
    int y;
    Point(int X, int Y): x(X), y(Y){};
};

Point operator/ (Point left, int right);
Point operator- (Point left, Point right);
Point operator- (Point left);
std::strong_ordering operator<=> (Point left, Point right);
#endif