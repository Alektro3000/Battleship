
#include <utility>
#include <type_traits>
#include <concepts>
#include <algorithm>

#ifndef PointH
#define PointH

struct Rotation
{
    enum Value : char
    {
        Right = 0,
        Up = 1,
        Left = 2,
        Down = 3,
    };
  constexpr Rotation() = default;
  constexpr Rotation(char rot) : value((Value)rot) { }
  constexpr Rotation(Value rot) : value(rot) { }
  constexpr Rotation operator+(Rotation rot)  {return (value + rot.value) % 4;  }
  constexpr Rotation operator+(Value rot)  {return (value + rot) % 4;  }
  constexpr operator Value() const { return value; }
private:
  Value value;
};


template <std::copyable T>
struct Point
{
    T x;
    T y;

    constexpr Point()
        requires std::default_initializable<T>
    {};
    constexpr Point(T X, T Y) : x(std::move(X)), y(std::move(Y)) {};
    constexpr Point(T X) : x(std::move(X)), y(std::move(X)) {};

    template <typename K>
        requires std::convertible_to<K, T>
    constexpr Point(Point<K> other) : x(other.x), y(other.y){};
    constexpr auto operator<=>(const Point &left) const = default;

    template <typename K>
        requires requires(T a, K b) { a * b; }
    constexpr auto operator*(const K &right) const -> Point<std::decay_t<decltype(x * right)>>
    {
        return {x * right, y * right};
    };

    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const K &right) const -> Point<std::decay_t<decltype(x / right)>>
    {
        return {x / right, y / right};
    };

    template <typename K>
        requires requires(T a, K b) { a *b; }
    constexpr auto operator*(const Point<K> &right) const -> Point<std::decay_t<decltype(x * right.x)>>
    {
        return {x * right.x, y * right.y};
    };

    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const Point<K> &right) const -> Point<std::decay_t<decltype(x / right.x)>>
    {
        return {x / right.x, y / right.y};
    };

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const Point<K> &right) const -> Point<std::decay_t<decltype(x - right.x)>>
    {
        return {x - right.x, y - right.y};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const Point<K> &right) const -> Point<std::decay_t<decltype(x + right.x)>>
    {
        return {x + right.x, y + right.y};
    };

    constexpr Point operator-() const
        requires requires(T a) { -a; }
    {
        return {-x, -y};
    };
    constexpr Point rotated(Rotation rot) const
        requires requires(T a) { -a; }
    {
        switch(rot)
        {
            case Rotation::Right:
                return {x,y};
            case Rotation::Up:
                return {y,-x};
            case Rotation::Left:
                return {-x,-y};
            case Rotation::Down:
                return {-y,x};
        }
        return {y,x};
        
    };
};

using PointI = Point<int>;
using PointF = Point<float>;

template <std::copyable T>
struct Rect
{
    Point<T> low;
    Point<T> high;

    constexpr Rect()
        requires std::default_initializable<T>
    {};
    constexpr Rect(Point<T> newLow, Point<T> newHigh) : low(std::move(newLow)), high(std::move(newHigh)) {};
    constexpr Rect(const Rect& newf) : low(newf.low), high(newf.high) {};
    constexpr Point<T> size() const { return high - low; };

    template <typename K>
        requires std::convertible_to<K, T>
    constexpr Rect(Rect<K> other) : low(other.low), high(other.high){};

    constexpr bool isPointInside(const Point<T> &point) const
    {
        return low.x <= point.x && point.x <= high.x &&
               low.y <= point.y && point.y <= high.y;
    };
    constexpr Rect normalized()
    {
        using std::max;
        using std::min;
        return {{min(low.x,high.x),min(low.y,high.y)},
                {max(low.x,high.x),max(low.y,high.y)}};
    };

    constexpr auto operator<=>(const Rect &right) const = default;

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const Rect<K> &right) const -> Rect<std::decay_t<decltype(low.x - right.low.x)>>
    {
        return {low - right.low, high - right.high};
    };

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const K &right) const -> Rect<std::decay_t<decltype(low.x - right)>>
    {
        return {low - right, high - right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator-(const Point<K> &right) const -> Rect<std::decay_t<decltype(low.x - right.x)>>
    {
        return {low - right, high - right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    auto operator+(const Rect<K> &right) const -> Rect<std::decay_t<decltype(low.x + right.low.x)>>
    {
        return {low + right.low, high + right.high};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const K &right) const -> Rect<std::decay_t<decltype(low.x + right)>>
    {
        return {low + right, high + right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const Point<K> &right) const -> Rect<std::decay_t<decltype(low.x + right.x)>>
    {
        return {low + right, high + right};
    };
    
    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const Point<K> &right) const -> Rect<std::decay_t<decltype(low.x / right.x)>>
    {
        return {low / right, high / right};
    };
    
    template <typename K>
        requires requires(T a, K b) { a * b; }
    constexpr auto operator*(const Point<K> &right) const -> Rect<std::decay_t<decltype(low.x * right.x)>>
    {
        return {low * right, high * right};
    };

    constexpr Rect<T> operator&(const Rect<T> &right) const
    {
        using std::max;
        using std::min;
        return {{max(low.x, right.low.x), max(low.y, right.low.y)},
                {min(high.x, right.high.x), min(high.y, right.high.y)}};
    };
    constexpr Rect<T> operator|(const Rect<T> &right) const
    {
        using std::max;
        using std::min;
        return {{min(low.x, right.low.x), min(low.y, right.low.y)},
                {max(high.x, right.high.x), max(high.y, right.high.y)}};
    };
};
using RectI = Rect<int>;
using RectF = Rect<float>;
#endif