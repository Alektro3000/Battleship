#include "Grid.h"
#include <algorithm>
#include <d2d1.h>

void DrawShip(BattleShip ship, RectF position,  ID2D1HwndRenderTarget* drawTarget)
{
    ID2D1SolidColorBrush *pBlackBrush = nullptr;
    drawTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush);
    drawTarget->FillRectangle(
            makeD2DRectF(position),
            pBlackBrush);
    pBlackBrush->Release();
}
