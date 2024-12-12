
#include "DXApp.h"

DXApp *AppPtr;
namespace widget{
    void pushWidget(std::unique_ptr<widget::IWidget> NewWidget, bool pushToStackPrev)
    {
        AppPtr->pushWidget(std::move(NewWidget), pushToStackPrev);
    }
}

DXApp::DXApp(HWND hWnd)
{
    auto &Cont = Context::getInstance();
    Cont.init(hWnd);
    size = makePointF(Cont.getRenderTarget()->GetSize());
}
void DXApp::onWinResize(UINT32 width, UINT32 height)
{

    if(width == 0 && height == 0)
    {
        size = PointF(0);
        return;
    }
    Context::getInstance().getRenderTarget()->Resize(D2D1::SizeU(width, height));
    size = makePointF(Context::getInstance().getRenderTarget()->GetSize());
    currentWidget->onResize({{0}, size});
};
void DXApp::popWidget()
{
    pushWidget(std::move(Stack.top()),false);
    Stack.pop();
}

void DXApp::renderFrame()
{
    if(size == PointF{0})
        return;
    Context::getInstance().getRenderTarget()->BeginDraw();

    currentWidget->onRender();
#ifdef LiveCheck
    SolidBrush brush{RenderTarget, D2D1::ColorF(rand(), 0.5)};
    RenderTarget->FillRectangle(makeD2DRectF({{0, 0}, {100, 100}}), brush);
#endif
    if(!Stack.empty())
    {
        auto size = Stack.size();
        auto bitmap = Context::getInstance().getBitmapFromFile(L"back.png");
        Context::getInstance().getRenderTarget()->DrawBitmap(bitmap, makeD2DRectF(RectF{{0}, {60}}));
    }
    HRESULT hr = Context::getInstance().getRenderTarget()->EndDraw();
}
