#include "RulesSelect.h"
#include "SelectWidget.h"
#include "../../Players/PcPlayer.h"

#ifndef RulesPcWidgetH
#define RulesPcWidgetH

namespace widget
{
    class PcRules final : public Overlay<RulesSelect, Button<Padder<TextBox> > >
    {
        static auto getUnselectedColor()
        {
            return D2D1::ColorF(D2D1::ColorF::Black);
        }
    public:
        PcRules() : 
            Overlay(
                {RectF{{0.1,0.1},{0.9,0.9}}, RulesSelect{}}, 
                {RectF{{0.7, 0.92}, {0.99, 0.98}},
                 Builder::makeText(L"Начать").addPadding().setBorder().addButton([this](auto _){
                    auto rules = getWidget<0>().getRules();
                    pushWidget(std::make_unique<SelectWidget>(rules,std::make_unique<PcPlayer>(rules)));
                 }).build()
                }){};
    };
}
#endif