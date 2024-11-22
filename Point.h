
#include <compare>
#include <utility>
#include <type_traits>



#ifndef PointH
#define PointH
template<typename T>
struct BasePoint
{
    T x;
    T y;
    BasePoint(){};
    BasePoint(T X, T Y): x(X), y(Y){};
    template<typename K>
    BasePoint(BasePoint<K> other): x(other.x), y(other.y){};
    auto operator<=> (const BasePoint& left) const = default;
    
    template<typename K>
    auto operator* (K right) const -> BasePoint<std::decay_t<decltype(x * right)> >
    {return {x*right,y*right};};

    template<typename K>
    auto operator/ (K right) const -> BasePoint<std::decay_t<decltype(x / right)> >
    {return {x/right,y/right};};
    
    template<typename K>
    auto operator* (BasePoint<K> right) const -> BasePoint<std::decay_t<decltype(x * right.x)> >
    {return {x*right.x,y*right.y};};

    template<typename K>
    auto operator/ (BasePoint<K> right) const -> BasePoint<std::decay_t<decltype(x / right.x)> >
    {return {x/right.x,y/right.y};};

    template<typename K>
    auto operator- (BasePoint<K> right) const -> BasePoint<std::decay_t<decltype(x - right.x)> >
    {return {x-right.x,y-right.y};};

    template<typename K>
    auto operator+ (BasePoint<K> right) const -> BasePoint<std::decay_t<decltype(x + right.x)> >
    {return {x+right.x,y+right.y};};

    BasePoint operator- () const {return BasePoint(-x,-y);};
};

using Point = BasePoint<int>; 
using PointF = BasePoint<float>;

struct Rect 
{
    Point low;
    Point high;
    Rect(Point newLow, Point newHigh): low(newLow), high(newHigh){};
    std::weak_ordering operator<=> (const Rect& right) const = default;
    Rect operator- (Rect left) const;
    Rect operator& (Rect left) const;
};
#endif