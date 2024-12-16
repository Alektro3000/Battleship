#include "PcPlayer.h"
#include <ranges>

namespace rg = std::ranges;

PointI PCPlayer::getMove()
{
    if (target == PointI{-1})
    {
        int clearCount = rg::count(playerHits, Results::Clear);
        int rand = std::abs(int(mt()));
        int n = rand % clearCount;
        auto iter = playerHits.begin();
        int i = 0;
        while (i <= n)
            if (*(iter++) == Results::Clear)
                i++;

        iter--;
        int dist = (iter - playerHits.begin());
        target = PointI{dist / 10, dist % 10};
    }
    counter++;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return target;
}



void PCPlayer::buildShipLocations()
{
    int x = 0;
    int y = 0;
    for (int i = rules.getMaxShipLength(); i >= 1; i--)
        for (int j = 0; j < rules.getShipAmount(i); j++)
        {
            BattleShip a = {PointI(x, y), i};
            
            shipHits.emplace_back(0);
            if(i == 1 && j == rules.getShipAmount(i)-1)
                break;
            ships.emplace_back(a);
            x += i + 1;
            if (x + i > rules.getSize().x)
            {
                y += 2;
                x = 0;
            }
        }

    if (rules.getShipAmount(1) == 0)
        return;
    

    int restX = (rules.getSize().x - x);
    int restY = (rules.getSize().y - y - 2) * rules.getSize().x;
    int possible = restX * 2 + restY;

    int rand = std::abs(int(mt())) % possible;
    PointI pos;
    if (rand < restX * 2)
    {
        pos = PointI(x + rand % restX, y + rand / restX);
    }
    else
    {
        rand -= restX * 2;
        pos = PointI(rand % rules.getSize().x, y + 2 + rand / rules.getSize().x);
    }
    ships.emplace_back(pos, 1);
}

std::vector<BattleShip> PCPlayer::showAllShips()
{
    return ships;
}

AttResult PCPlayer::makeMove(PointI x)
{
    auto damagedShip = rg::find_if(ships,
                                    [x](BattleShip y)
                                    { return y.IntersectionPosition(x) != -1; });
    if (damagedShip == ships.end())
        return Results::Miss;
    auto i = damagedShip - ships.begin();
    shipHits[i] |= damagedShip->getHitMask(x);
    return AttResult(*damagedShip,shipHits[i]);
}

size_t PCPlayer::getHashGrid()
{
    return BattleShip::getHash(std::vector(ships));
}
auto PCPlayer::findClear(PointI point)
{
    return [this, point](const PointI& val){
                return rules.isValidIndex(point + val) &&
                    playerHits[rules.flatIndex(point + val)] == Results::Clear;
            } ;
}

void PCPlayer::returnResult(AttResult res)
{
    playerHits[rules.flatIndex(target)] = res.val;

    std::array adj{PointI{1, 0}, PointI{0, 1}, PointI{-1, 0}, PointI{0, -1}};
    rg::shuffle(adj,mt);

    std::array corner{PointI{1, 1}, PointI{-1, 1}, PointI{-1, -1}, PointI{1, -1}};
    if (res.val == Results::Hit)
    {
        for (auto q : corner)
            if (rules.isValidIndex(target + q))
                playerHits[rules.flatIndex(target + q)] = Results::Miss;
        if (prevHit != PointI{-1})
        {
            if (rules.isValidIndex(target + (target - prevHit).sgn()))
            {
                target = target + (target - prevHit).sgn();
                isWalking = true;
            }
            else
                target = prevHit + (prevHit - target).sgn();
        }
        else
        {
            prevHit = target;
            auto foundValidMovement = rg::find_if(adj, findClear(target) );
            if(foundValidMovement != adj.end())
                target = target + *foundValidMovement;
        }
        return;
    }
    if (res.val == Results::Miss)
    {
        if (isWalking)
        {
            target = prevHit + (prevHit - target).sgn();
            return;
        }

        if (prevHit != PointI{-1})
        {
            auto foundValidMovement = rg::find_if(adj,findClear(prevHit));
            if(foundValidMovement != adj.end())
                target = prevHit + *foundValidMovement;
            return;
        }
    }
    if (res.val == Results::Destroy)
    {
        
        for (auto q : adj)
            if (rules.isValidIndex(prevHit + q) &&
                playerHits[rules.flatIndex(prevHit + q)] == Results::Clear)
                playerHits[rules.flatIndex(prevHit + q)] = Results::Miss;
        prevHit = {-1};
        isWalking = false;

        for (auto q : adj)
            if (rules.isValidIndex(target + q) &&
                playerHits[rules.flatIndex(target + q)] == Results::Clear)
                playerHits[rules.flatIndex(target + q)] = Results::Miss;
        for (auto q : corner)
            if (rules.isValidIndex(target + q))
                playerHits[rules.flatIndex(target + q)] = Results::Miss;
    }
    target = {-1};
};