#include "BattleShip.h"
#include <boost/functional/hash.hpp>
#include <algorithm>
#include <bit>

BattleShip::BattleShip(PointI begin, int length, Rotation rotation, int variation) noexcept
{
    x = begin.x;
    y = begin.y;
    len = length;
    rot = (unsigned int)rotation;
    var = variation;
}

int BattleShip::IntersectionPosition(PointI point) const noexcept
{
    auto P = (point - getPoint()).rotated(getRotation());
    auto shipLength = getLength();
    return (P.x >= 0 && P.x < shipLength && P.y == 0) ? P.x : -1;
}
RectI BattleShip::getRect() const noexcept
{
    PointI low = getPoint();
    auto P = (PointI{getLength() - 1, 0}).rotated(getRotation());
    return Rect{low, low + P}.normalized();
}

bool BattleShip::hasIntersection(BattleShip other) const noexcept
{
    auto a = getRect();
    auto b = other.getRect();
    auto diff = (a & b).size();

    return diff.x >= 0 && diff.y >= 0;
}

bool BattleShip::hasIntersectionAdj(BattleShip other) const noexcept
{
    auto diff = (getRect() & other.getRect()).size();

    return diff.y >= 0 && diff.x >= -1 || diff.x >= 0 && diff.y >= -1;
}

bool BattleShip::hasIntersectionCorner(BattleShip other) const noexcept
{
    auto diff = (getRect() & other.getRect()).size();
    auto fl = diff.x >= -1 && diff.y >= -1;
    return fl;
}
    std::size_t hash_value(BattleShip const& b)
    {
        boost::hash<int> hasher;
        return hasher(std::bit_cast<int>(b));
    }

std::size_t BattleShip::getHash(std::vector<BattleShip>&& vals)
{
    std::sort(vals.begin(), vals.end(), [](auto a, auto b)
              { return a.getPoint() < b.getPoint(); });
              
    return boost::hash_range(vals.begin(), vals.end());
}