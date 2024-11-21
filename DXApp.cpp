
#include "DXApp.h"


D2D_SIZE_U operator/ (D2D_SIZE_U left, int right)
{
    return D2D1::SizeU(left.width/right,left.height/right);
}

std::strong_ordering operator<=> (D2D_SIZE_U left, D2D_SIZE_U right)
{
    if(left.width == right.width)
        return left.height <=> right.height;
    else
        return left.width <=> right.width;
}