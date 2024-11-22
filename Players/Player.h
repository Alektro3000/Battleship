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

enum class Rotation : int
{
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

struct BattleShip
{
private:
    int Data;

    constexpr static std::array<char, 5> FieldSizes = {10,10,6,2,4};
    constexpr static std::array<char, 5> FieldOffsets = {0,10,20,26,28};
    int packField(int val, int id)
    {
        return (val & ((1 << FieldSizes[id]) - 1)) << FieldOffsets[id];
    }
    int unpackField(int val, int id)
    {
        return (val >> FieldOffsets[id])  & ((1 << FieldSizes[id]) - 1);
    }
public:
    BattleShip(Point begin, int length, Rotation rotation, int variation = 0);
    int getX() { return unpackField(Data,0); };
    int getY() { return unpackField(Data,1);  };
    Point getPoint() { return {getX(), getY()};  };
    int getLength() { return unpackField(Data,2);  };
    Rotation getRotation() { return (Rotation)unpackField(Data,3); };
    int getVariatin() { return unpackField(Data,4); };

    int IntersectionPosition(Point Point);
    Rect getRect();
    bool hasIntersection(BattleShip other);
};
struct GameRules
{
    int getSizeX() {return 10;};
    int getSizeY() {return 10;};
    int getShipAmount(int i) {return ShipSizes[i];};
    int getMaxShipLength() {return ShipSizes.size();};
    bool AllowedDiagonals() {return false;};
    bool AllowedAdjacent() {return false;};
private:

    constexpr static std::array<char, 5> ShipSizes = {4,3,2,1,0};
};

struct IPlayer
{
    virtual std::unique_ptr<IPlayer> copy() const = 0;
    virtual ~IPlayer() = default;

    virtual void Init(GameRules rules) = 0;
    virtual void SetBeginPosition(std::vector<BattleShip> Ships) = 0;
    virtual Results MakeMove(Point x) = 0;
};

#endif