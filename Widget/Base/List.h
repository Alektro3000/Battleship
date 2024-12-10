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
        float heightPer;
        List(float padding, float height, float heightPer = 0) : padding(padding), height(height), heightPer(heightPer) {};
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
                makeD2DRectF(this->position),
                D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
            );
            std::for_each(childs.begin(), childs.end(),[](auto& val){val.onRender();});
            Context::getInstance().getRenderTarget()->PopAxisAlignedClip();
        }
        void onClick(MouseButton button) override
        {
            //TODO
            std::for_each(childs.begin(),childs.end(),[button](auto& val){val.tryClick(button);});
        }
        void update()
        {
            onResize(position);
        }
        RectF getNodePosition(int counter)
        {
            auto nodeHeight = heightPer*position.size().y + height;
            return RectF{{position.low.x,position.low.y+ (nodeHeight+padding)*counter}, 
                    {position.high.x,position.low.y+ (nodeHeight+padding)*counter + nodeHeight}};
        }
        int getNodeIndex(PointF point)
        {
            if(!position.isPointInside(point))
                return -1;
            auto y = point.y - position.low.y;
            auto nodeHeight = heightPer*position.size().y + height;
            auto normalized = y%(nodeHeight+padding); 
            return -1;
        }
    };
};
#endif