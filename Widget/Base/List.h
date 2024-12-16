#include "Widget.h"
#include <concepts>

#ifndef ListWidgetH
#define ListWidgetH
namespace widget
{
    template <TWidget Child>
    class List : public Widget
    {
    protected:
        List(List &&other) = default;
        List &operator=(List &&other) = default;

    public:
        std::vector<Child> childs;
        float distance;
        float height;
        float heightPercentage;
        RectF innerPadding;
        List(float distance, float height, float heightPer = 0, RectF innerPadding = {0,0}) 
            : distance(distance), height(height), heightPercentage(heightPer),
                innerPadding(innerPadding) {};
        void onResize(RectF newSize) override
        {
            int counter = 0;
            Widget::onResize(newSize);
            std::for_each(childs.begin(), childs.end(),
                          [&counter, this](auto &val)
                          {
                              val.onResize(getNodePosition(counter++));
                          });
        }
        void onRender() override
        {
            IWidget::onRender();
            Context::getInstance().getRenderTarget()->PushAxisAlignedClip(
                makeD2DRectF(this->getPosition()),
                D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            std::for_each(childs.begin(), childs.end(), [](auto &val)
                          { val.onRender(); });
            Context::getInstance().getRenderTarget()->PopAxisAlignedClip();
        }
        void onClickDown(MouseButton button) override
        {
            tryClickChild([button](auto &val)
                          { val.onClickDown(button); });
        }
        template <std::invocable<Child &> F>
        std::size_t tryClickChild(const F &func)
        {
            // TODO
            return std::accumulate(childs.begin(), childs.end(), std::size_t(),
                                   [i = std::size_t(0), cursor = Context::getInstance().getCursor(), &func](std::size_t num, auto &val) mutable
                                   {
                                       i++;
                                       if (!val.getPosition().isPointInsideExcl(cursor))
                                           return num;
                                       func(val);
                                       return i;
                                   });
        }
        void onClickUp(MouseButton button) override
        {
            tryClickChild([button](auto &val)
                          { val.onClickUp(button); });
        }
        void update()
        {
            onResize(getPosition());
        }
        RectF getNodePosition(int counter)
        {
            auto nodeHeight = heightPercentage * getPosition().size().y + height;
            return RectF{innerPadding.low + getPosition().low + PointF{0.f, (nodeHeight + distance) * counter},
                         -innerPadding.high + getPosition().low + PointF{getPosition().size().x, (nodeHeight + distance) * counter + nodeHeight}};
        }
        int getNodeIndex(PointF point)
        {
            if (!getPosition().isPointInside(point))
                return -1;
            auto y = point.y - getPosition().low.y;
            auto nodeHeight = heightPercentage * getPosition().size().y + height;
            auto normalized = y % (nodeHeight + distance);
            return -1;
        }
    };
    template <TWidget Arg>
    class ListFinal final : public List<Arg>
    {
    public:
        using List<Arg>::List;
        using List<Arg>::operator=;
    };
};
#endif