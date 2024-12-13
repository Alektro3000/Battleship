#include "BattleGrid.h"
#include <d2d1.h>
#include <algorithm>
#include <cstring>

namespace widget
{
    void BattleGrid::RenderVal(PointI pos, Results res)
    {
        auto ellipse = D2D1::Ellipse(makeD2DPointF(getCoordPosition(pos) + getGridSize() / 2),  getGridSize().x / 4,  getGridSize().y / 4);
        if (res == Results::Miss)
            Context::getInstance().getRenderTarget()->FillEllipse(ellipse, grayBrush);
        else if (res != Results::Clear)
        {
            if (res == Results::Destroy)
                redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkRed));
            Context::getInstance().getRenderTarget()->FillEllipse(ellipse, redBrush);

            redBrush.brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        }
    }
    std::atomic<Results> &BattleGrid::getResult(PointI pos)
    {
        return shots[pos.x+getSize().x*pos.y];
    }
    void BattleGrid::onRender()
    {
        Context::getInstance().getRenderTarget()->DrawRectangle(
            makeD2DRectF(getGridPos()),
            blackBrush, 3);

        for (int i = 0; i < size.x; i++)
        {
            constexpr std::array nums{L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10"};
            auto offset = getPosition().low + PointI{1 + i, 0} * getGridSize();
            Context::getInstance().getRenderTarget()->DrawText(
                nums[i],
                std::wcslen(nums[i]),
                format,
                makeD2DRectF({offset - getGridSize(), offset + getGridSize() * 2}),
                blackBrush);
        }
        for (int i = 0; i < size.y; i++)
        {
            constexpr std::array letters{L"А", L"Б", L"В", L"Г", L"Д", L"Е", L"Ж", L"З", L"И", L"К"};
            auto offset = getPosition().low + PointI{0, 1 + i} * getGridSize();
            Context::getInstance().getRenderTarget()->DrawText(
                letters[i],
                std::wcslen(letters[i]),
                format,
                makeD2DRectF({offset - getGridSize(), offset + getGridSize() * 2}),
                blackBrush);
        }

    }
        void BattleGrid::renderShots()
        {
            
        for (int i = 0; i < getSize().x; i++)
            for (int j = 0; j < getSize().y; j++)
                RenderVal(PointI(i, j), getResult(PointI(i, j)));
        }
}