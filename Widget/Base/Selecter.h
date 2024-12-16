#include "List.h"

#ifndef SelecterWidgetH
#define SelecterWidgetH

namespace widget
{
    template <TWidget Child>
    class Selecter final : public List<Child>
    {
        std::function<void(Child &)> OnSelect;
        std::function<void(Child &)> OnUnSelect;
        std::size_t selected = 0;

    public:
        using List<Child>::List;
        using List<Child>::operator=;

        template <std::invocable<Child &> FSel, std::invocable<Child &> FUnsel>
        void initSelecter(FSel &&onSelect, FUnsel &&onUnSelect, std::size_t select = 0)
        {
            OnSelect = std::forward<FSel>(onSelect);
            OnUnSelect = std::forward<FUnsel>(onUnSelect);
            selected = select;
        }
        void onClickDown(MouseButton button) override
        {
            if (selected != 0)
                OnUnSelect(List<Child>::childs[selected - 1]);

            selected = List<Child>::tryClickChild([this](auto &val)
                                                  { OnSelect(val); });
        }
        void onClickUp(MouseButton button) override {};
        std::size_t getSelected()
        {
            return selected;
        }
        void select(std::size_t s)
        {
            if (selected != 0)
                OnUnSelect(this->childs[selected - 1]);
            selected = s;
            OnSelect(this->childs[selected - 1]);
        }
    };
}
#endif