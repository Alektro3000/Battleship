
#include "Player.h"
#include <cassert>

BattleShip::BattleShip(PointI begin, int length, Rotation rotation, int variation)
{
    x = begin.x;
    y = begin.y;
    len = length;
    rot = (unsigned int)rotation;
    var = variation;
}

int BattleShip::IntersectionPosition(PointI point)
{
    auto P = (point - getPoint()).rotated(-getRotation());
    auto shipLength = getLength();
    return (P.x >= 0 && P.x < shipLength && P.y == 0) ? P.x : -1;
}
RectI BattleShip::getRect()
{
    PointI low = getPoint();
    auto P = (PointI{getLength()-1,0}).rotated(getRotation());
    return Rect {low,low + P}.normalized();  
}

bool BattleShip::hasIntersection(BattleShip other)
{
    auto a = getRect();
    auto b = other.getRect();
    auto diff = (a&b).size();

    return diff.x >= 0 && diff.y >= 0;  
}

bool BattleShip::hasIntersectionAdj(BattleShip other)
{
    auto diff = (getRect()&other.getRect()).size();

    return  diff.y >= 0 && diff.x >= -1 || diff.x >= 0 && diff.y >= -1;  
}

bool BattleShip::hasIntersectionCorner(BattleShip other)
{
    auto diff = (getRect()&other.getRect()).size();
    auto fl = diff.x >= -1 && diff.y >= -1;
    return fl;  
}