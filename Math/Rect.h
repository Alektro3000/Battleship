#include "Point.h"

#ifndef RectH
#define RectH

template <std::copyable T>
struct Rect
{
    Point<T> low;
    Point<T> high;

    constexpr Rect() noexcept
        requires std::default_initializable<T>
    {};
    constexpr explicit Rect(T val) noexcept: low(Point<T>(val)), high(std::move(Point<T>(val))) {};
    constexpr explicit Rect(Point<T> val) noexcept: low(val), high(std::move(val)) {};
    
    constexpr Rect(Point<T> newLow, Point<T> newHigh) noexcept: low(std::move(newLow)), high(std::move(newHigh)) {};
    constexpr Rect(const Rect& newf) noexcept: low(newf.low), high(newf.high) {};
    constexpr Point<T> size() const noexcept { return high - low; };

    template <typename K>
        requires std::convertible_to<K, T>
    constexpr Rect(Rect<K> other) noexcept: low(other.low), high(other.high){};

    constexpr bool isPointInside(const Point<T> &point) const 
    {
        return low.x <= point.x && point.x <= high.x &&
               low.y <= point.y && point.y <= high.y;
    };
    constexpr bool isPointInsideExcl(const Point<T> &point) const 
    {
        return low.x <= point.x && point.x < high.x &&
               low.y <= point.y && point.y < high.y;
    };
    constexpr Rect normalized() const noexcept
    {
        using std::max;
        using std::min;
        return {{min(low.x,high.x),min(low.y,high.y)},
                {max(low.x,high.x),max(low.y,high.y)}};
    };

    constexpr auto operator<=>(const Rect &right) const noexcept = default;

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const Rect<K> &right) const noexcept(noexcept(low - right.low))
    -> Rect<std::decay_t<decltype(low.x - right.low.x)>>
    {
        return {low - right.low, high - right.high};
    };

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const K &right) const noexcept(noexcept(low - right))
    -> Rect<std::decay_t<decltype(low.x - right)>>
    {
        return {low - right, high - right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator-(const Point<K> &right) const noexcept(noexcept(low - right))
    -> Rect<std::decay_t<decltype(low.x - right.x)>>
    {
        return {low - right, high - right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    auto operator+(const Rect<K> &right) const noexcept(noexcept(low + right.low))
    -> Rect<std::decay_t<decltype(low.x + right.low.x)>>
    {
        return {low + right.low, high + right.high};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const K &right) const noexcept(noexcept(low + right))
    -> Rect<std::decay_t<decltype(low.x + right)>>
    {
        return {low + right, high + right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const Point<K> &right) const noexcept(noexcept(low + right))
    -> Rect<std::decay_t<decltype(low.x + right.x)>>
    {
        return {low + right, high + right};
    };
    
    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const Point<K> &right) const noexcept(noexcept(low / right))
    -> Rect<std::decay_t<decltype(low.x / right.x)>>
    {
        return {low / right, high / right};
    };
    
    template <typename K>
        requires requires(T a, K b) { a * b; }
    constexpr auto operator*(const Point<K> &right) const noexcept(noexcept(low*right))
     -> Rect<std::decay_t<decltype(low.x * right.x)>>
    {
        return {low * right, high * right};
    };

    constexpr Rect<T> operator&(const Rect<T> &right) const noexcept
    {
        using std::max;
        using std::min;
        return {{max(low.x, right.low.x), max(low.y, right.low.y)},
                {min(high.x, right.high.x), min(high.y, right.high.y)}};
    };
    constexpr Rect<T> operator|(const Rect<T> &right) const noexcept
    {
        using std::max;
        using std::min;
        return {{min(low.x, right.low.x), min(low.y, right.low.y)},
                {max(high.x, right.high.x), max(high.y, right.high.y)}};
    };
    constexpr Rect<T> scaled(const Rect<T> &right) const noexcept
    {
        return (*this) * right.size() + right.low;
    };
    constexpr Point<T> center() const noexcept
    {
        return (low+high)/2;
    };
    constexpr Rect<T> rotated(Rotation rotate) const noexcept
    {
        return {(low-center()).rotated(rotate) + center(),
                (high-center()).rotated(rotate) + center()};
    };
};
using RectI = Rect<int>;
using RectF = Rect<float>;
#endif