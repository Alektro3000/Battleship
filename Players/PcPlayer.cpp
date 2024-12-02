#include "PcPlayer.h"


    PointI PCPlayer::GetMove() 
    {
        if(target == PointI{-1})
        {
            int clearCount = std::count(playerHits.begin(),playerHits.end(), Results::Clear);
            int rand = std::abs(int(mt()));
            int n = rand%clearCount;
            auto iter = playerHits.begin();
            int i = 0;
            while(i<=n)
                if(*(iter++) == Results::Clear)
                    i++;
                
            iter--;
            int dist = (iter - playerHits.begin());
            target = PointI{dist/10,dist%10};
        }
        counter++;
        return target;
    }