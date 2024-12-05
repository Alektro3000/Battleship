#include <memory>
#include <utility>
#include <bitset>
#include <vector>
#include <array>
#include <numeric>
#include "../Math/Rect.h"
#ifndef PlayerH
#define PlayerH

enum class Results
{
    Clear,
    Miss,
    Hit,
    Destroy
};
struct GameRules
{
    constexpr PointI getSize() {return {10,10};};
    constexpr int getShipAmount(int i) {return ShipsAmounts[i-1];};
    constexpr int getTotalShipAmount() {return std::accumulate(ShipsAmounts.begin(),ShipsAmounts.end(),0);};
    constexpr int getTotalHitAmount() 
    {return std::accumulate(ShipsAmounts.begin(),ShipsAmounts.end(),0,[i = 1](auto prev, auto cur) mutable
    {return prev + cur * i++;});};
    constexpr int getMaxShipLength() {return ShipsAmounts.size();};
    constexpr bool AllowedDiagonals() {return false;};
    constexpr int flatIndex(PointI pos) {return pos.x * getSize().y + pos.y;};
    constexpr bool isValidIndex(PointI pos) {return RectI{0,getSize()}.isPointInsideExcl(pos);};
private:
    constexpr static std::array<char, 5> ShipsAmounts = {4,3,2,1,0};
};

struct BattleShip
{
private:
    int x : 10,
     y : 10;
    unsigned len : 6,
     rot : 2,
     var : 4;
public:
    constexpr auto operator<=>(const BattleShip &left) const = default;

     BattleShip() noexcept : BattleShip({0,0},0) {};
    BattleShip(PointI begin, int length = 1, Rotation rotation = Rotation::Right, int variation = 0) noexcept;
    int getX() const noexcept { return x; };
    int getY() const noexcept { return y;  };
    PointI getPoint() const noexcept { return {getX(), getY()};  };
    int getLength() const noexcept { return len;  };
    Rotation getRotation() const noexcept { return (Rotation)rot; };
    int getVariatin() const noexcept { return var; };
    void setPoint(PointI newStart) noexcept { x=newStart.x; y = newStart.y; };
    void setRotation(Rotation newRotation) noexcept { rot = (int)newRotation; };

    //-1 - if no connection otherwise on [0;Length)
    int IntersectionPosition(PointI PointI) const noexcept;
    RectI getRect() const noexcept;
    bool hasIntersection(BattleShip other) const noexcept;
    bool hasIntersectionAdj(BattleShip other) const noexcept;
    bool hasIntersectionCorner(BattleShip other) const noexcept;

    bool isDestroyed(int hitMask) const noexcept
    {
        return hitMask == ((1<<getLength()) - 1);
    }
    Results isDestroyedRes(int hitMask) const noexcept
    {
        return isDestroyed(hitMask) ? Results::Destroy : Results::Hit;
    }
    int getHitMask(PointI PointI) const noexcept
    {
        return 1<<IntersectionPosition(PointI);
    }
};

struct Player
{
    virtual ~Player() = default;

    virtual std::vector<BattleShip> ShowShips() = 0;
    virtual Results MakeMove(PointI x) = 0;

    virtual PointI GetMove() = 0;
    virtual void ReturnResult(Results Point) = 0; 

    virtual void onDetach() {}; 
};

#endif