
#include "Player.h"
BattleShip::BattleShip(Point begin, int length, Rotation rotation, int variation)
{
    Data = 0;
    Data |= packField(begin.x, 0);
    Data |= packField(begin.y, 1);
    Data |= packField(length, 2);
    Data |= packField((int)rotation, 3);
    Data |= packField(variation, 4);
}

int BattleShip::IntersectionPosition(Point point)
{
    int rot = static_cast<int>(getRotation());
    auto P = (rot > 1) ? (this->getPoint() - point) : (point - this->getPoint());
    rot &= 1;
    auto length = rot == 0 ? P.y : P.x;
    auto width = rot == 0 ? P.x : P.y;
    auto shipLength = getLength();
    return (length >= 0 && length < shipLength && width == 0) ? length : -1;
}
Rect BattleShip::getRect()
{
    Point low = getPoint();
    int rot = static_cast<int>(getRotation());
    auto P = getPoint();
    int len = (rot > 1) ? -getLength() : getLength();
    rot &= 1;
    Point high = low + Point(rot == 1 ? len : 0, rot != 1 ? len : 0);
    
    return len > 0 ? Rect(low,high) : Rect(high,low);  
}

bool BattleShip::hasIntersection(BattleShip other)
{
    auto diff = getRect()&other.getRect();

    return diff <= Rect{{0,0},{0,0}};  
}