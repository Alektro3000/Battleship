#include "../Widget.h"

#ifndef ListWidgetH
#define ListWidgetH
namespace widget
{
    template <TWidget Child>
    class List final : public Widget
    {
    public:
        std::vector<Child> childs;
        float padding;
        float height;
        float heightPercentage;
        List(float padding, float height, float heightPer = 0) : padding(padding), height(height), heightPercentage(heightPer) {};
        List(List &&other) = default;
        List &operator=(List &&other) = default;
        void onResize(RectF newSize) override
        {
            int counter = 0;
            Widget::onResize(newSize);
            std::for_each(childs.begin(), childs.end(), 
                [&counter, this](auto &val){ 
                val.onResize(getNodePosition(counter++));
            });
        }
        void onRender() override
        {
            IWidget::onRender();
            Context::getInstance().getRenderTarget()->PushAxisAlignedClip(
                makeD2DRectF(this->getPosition()),
                D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
            );
            std::for_each(childs.begin(), childs.end(),[](auto& val){val.onRender();});
            Context::getInstance().getRenderTarget()->PopAxisAlignedClip();
        }
        void onClickDown(MouseButton button) override
        {
            //TODO
            std::for_each(childs.begin(),childs.end(),[button](auto& val){val.tryClickDown(button);});
        }
        void onClickUp(MouseButton button) override
        {
            //TODO
            std::for_each(childs.begin(),childs.end(),[button](auto& val){val.tryClickUp(button);});
        }
        void update()
        {
            onResize(getPosition());
        }
        RectF getNodePosition(int counter)
        {
            auto nodeHeight = heightPercentage*getPosition().size().y + height;
            return RectF{{getPosition().low.x,getPosition().low.y+ (nodeHeight+padding)*counter}, 
                    {getPosition().high.x,getPosition().low.y+ (nodeHeight+padding)*counter + nodeHeight}};
        }
        int getNodeIndex(PointF point)
        {
            if(!getPosition().isPointInside(point))
                return -1;
            auto y = point.y - getPosition().low.y;
            auto nodeHeight = heightPercentage*getPosition().size().y + height;
            auto normalized = y%(nodeHeight+padding); 
            return -1;
        }
    };
};
#endif