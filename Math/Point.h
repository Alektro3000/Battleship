#include <utility>
#include <type_traits>
#include <concepts>

#ifndef PointH
#define PointH

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

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
  constexpr Rotation operator-()  {return (4-value ) % 4;  }
  constexpr operator Value() const { return value; }
private:
  Value value;
};


//Template class for Point/Vector2D supports base element-by-element operations and rotating via Rotation
template <std::copyable T>
struct Point
{
    T x;
    T y;

    constexpr Point() noexcept
        requires std::default_initializable<T>
    {};
    constexpr Point(T X, T Y) noexcept: x((X)), y((Y)) {};
    constexpr Point(T X) noexcept: x(X), y(X) {};

    template <typename K>
        requires std::convertible_to<K, T>
    constexpr Point(Point<K> other) noexcept: x(other.x), y(other.y){};
    constexpr auto operator<=>(const Point &left) const noexcept = default;

    template <typename K>
        requires requires(T a, K b) { a * b; }
    constexpr auto operator*(const K &right) const noexcept -> Point<std::decay_t<decltype(x * right)>>
    {
        return {x * right, y * right};
    };

    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const K &right) const noexcept -> Point<std::decay_t<decltype(x / right)>>
    {
        return {x / right, y / right};
    };

    template <typename K>
        requires requires(T a, K b) { a *b; }
    constexpr auto operator*(const Point<K> &right) const noexcept -> Point<std::decay_t<decltype(x * right.x)>>
    {
        return {x * right.x, y * right.y};
    };

    template <typename K>
        requires requires(T a, K b) { a / b; }
    constexpr auto operator/(const Point<K> &right) const noexcept -> Point<std::decay_t<decltype(x / right.x)>>
    {
        return {x / right.x, y / right.y};
    };

    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const Point<K> &right) const noexcept -> Point<std::decay_t<decltype(x - right.x)>>
    {
        return {x - right.x, y - right.y};
    };
    template <typename K>
        requires requires(T a, K b) { a - b; }
    constexpr auto operator-(const K &right) const noexcept -> Point<std::decay_t<decltype(x - right)>>
    {
        return {x - right, y - right};
    };

    template <typename K>
        requires requires(T a, K b) { a + b; }
    constexpr auto operator+(const Point<K> &right) const noexcept -> Point<std::decay_t<decltype(x + right.x)>>
    {
        return {x + right.x, y + right.y};
    };
    
    template <typename K>
        requires requires(T a, K b) { a + b; } 
    constexpr auto operator+(const K &right) const noexcept -> Point<std::decay_t<decltype(x + right)>>
    {
        return {x + right, y + right};
    };

    constexpr Point operator-() const
        requires requires(T a) { -a; }
    {
        return {-x, -y};
    };
    constexpr Point rotated(Rotation rot) const noexcept
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
    constexpr Point rotated(Rotation rot, Point pivot) const noexcept
        requires requires(T a) { -a; }
    {
        return (*this-pivot).rotated(rot)+pivot;
        
    };
    constexpr Point<int> sgn() const noexcept
        requires requires(T a) { ::sgn(a); }
    {
        return {::sgn(x),::sgn(y)};
    };
};
using PointI = Point<int>;
using PointF = Point<float>;
#endif