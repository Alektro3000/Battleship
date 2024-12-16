#include "../Base/Builder.h"

#ifndef RulesWidgetH
#define RulesWidgetH

namespace widget
{
    class RulesSelect final : public Overlay<Selecter<Padder<TextBox> > >
    {
        static auto getUnselectedColor()
        {
            return D2D1::ColorF(D2D1::ColorF::Black);
        }
    public:
        RulesSelect() : 
            Overlay({RectF{{0.1,0.1},{0.4,0.5}}, Builder::makeSelecter<Padder<TextBox>>(6,-6, 0.5f, RectF{2,2}).setSelecter(
                [](auto& widget) {widget.setBorder(D2D1::ColorF(D2D1::ColorF::Red),5);},
                [](auto& widget) {widget.setBorder(getUnselectedColor(),5);}, 1
            ).build()})
            {
                getWidget<0>().childs.push_back(Builder::makeText(L"8x8").addPadding().setBorder(getUnselectedColor(),5).build());
                getWidget<0>().childs.push_back(Builder::makeText(L"10x10").addPadding().setBorder(getUnselectedColor(),5).build());
                getWidget<0>().select(2);
            };
        
        GameRules getRules()
        {
            GameRules a;
            a.size = getWidget<0>().getSelected() == 1 ? PointF(8,8) : PointF(10,10);
            return a; 
        }
    };
}
#endif