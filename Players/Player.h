#include <memory>
#include <utility>
#include <bitset>
#include <vector>
#include <array>
#include "../Point.h"
#ifndef PlayerH
#define PlayerH

enum class Results
{
    Miss,
    Hit,
    Destroy
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

     BattleShip(): BattleShip({0,0},0) {};
    BattleShip(PointI begin, int length = 1, Rotation rotation = Rotation::Right, int variation = 0);
    int getX() const { return x; };
    int getY() const { return y;  };
    PointI getPoint() const { return {getX(), getY()};  };
    int getLength() const { return len;  };
    Rotation getRotation() const { return (Rotation)rot; };
    int getVariatin() const  { return var; };
    void setPoint(PointI newStart) { x=newStart.x; y = newStart.y; };
    void setRotation(Rotation newRotation) { rot = (int)newRotation; };

    int IntersectionPosition(PointI PointI);
    RectI getRect();
    bool hasIntersection(BattleShip other);
    bool hasIntersectionAdj(BattleShip other);
    bool hasIntersectionCorner(BattleShip other);
};
struct GameRules
{
    constexpr PointI getSize() {return {10,10};};
    constexpr int getShipAmount(int i) {return ShipsAmounts[i];};
    constexpr int getMaxShipLength() {return ShipsAmounts.size();};
    constexpr bool AllowedDiagonals() {return false;};
    constexpr bool AllowedAdjacent() {return false;};
private:
    constexpr static std::array<char, 5> ShipsAmounts = {4,3,2,1};
};

struct Player
{
    virtual ~Player() = default;

    virtual std::vector<BattleShip> ShowShips() = 0;
    virtual Results MakeMove(PointI x) = 0;
};

#endif