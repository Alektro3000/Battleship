#include "PcPlayer.h"

PointI PCPlayer::GetMove()
{
    if (target == PointI{-1})
    {
        int clearCount = std::count(playerHits.begin(), playerHits.end(), Results::Clear);
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return target;
}
void PCPlayer::BuildShipLocations()
{
    int x = 0;
    int y = 0;
    for (int i = _rules.getMaxShipLength(); i >= 1; i--)
        for (int j = 0; j < _rules.getShipAmount(i); j++)
        {
            BattleShip a = {PointI(x, y), i};
            x += i + 1;
            if (x + i > _rules.getSize().x)
            {
                y += 2;
                x = 0;
            }
            ships.emplace_back(a);
            shipHits.emplace_back(0);
        }
    auto last = ships.back();
    ships.pop_back();
    int rand = std::abs(int(mt()));
    
}

void PCPlayer::ReturnResult(Results res)
{
    playerHits[_rules.flatIndex(target)] = res;
    std::array adj{PointI{1, 0}, PointI{0, 1}, PointI{-1, 0}, PointI{0, -1}};
    std::array corner{PointI{1, 1}, PointI{-1, 1}, PointI{-1, -1}, PointI{1, -1}};
    if (res == Results::Hit)
    {
        for (auto q : corner)
            if (_rules.isValidIndex(target + q))
                playerHits[_rules.flatIndex(target + q)] = Results::Miss;
        if (prevHit != PointI{-1})
        {
            if (_rules.isValidIndex(target + (target - prevHit).sgn()))
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
            for (auto q : adj)
                if (_rules.isValidIndex(target + q) &&
                    playerHits[_rules.flatIndex(target + q)] == Results::Clear)
                {
                    target = target + q;
                    break;
                }
        }
        return;
    }
    if (res == Results::Miss)
    {
        if (isWalking)
        {
            target = prevHit + (prevHit - target).sgn();
            return;
        }

        if (prevHit != PointI{-1})
        {
            for (auto q : adj)
                if (_rules.isValidIndex(prevHit + q) &&
                    playerHits[_rules.flatIndex(prevHit + q)] == Results::Clear)
                    target = prevHit + q;
            return;
        }
    }
    if (res == Results::Destroy)
    {
        for (auto q : adj)
            if (_rules.isValidIndex(prevHit + q) &&
                playerHits[_rules.flatIndex(prevHit + q)] == Results::Clear)
                playerHits[_rules.flatIndex(prevHit + q)] = Results::Miss;
        prevHit = {-1};
        isWalking = false;

        for (auto q : adj)
            if (_rules.isValidIndex(target + q) &&
                playerHits[_rules.flatIndex(target + q)] == Results::Clear)
                playerHits[_rules.flatIndex(target + q)] = Results::Miss;
        for (auto q : corner)
            if (_rules.isValidIndex(target + q))
                playerHits[_rules.flatIndex(target + q)] = Results::Miss;
    }
    target = {-1};
};