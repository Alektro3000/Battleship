#include "Button.h"
#include "Padder.h"
#include "TextBox.h"
#include "List.h"
#include "Overlay.h"
#include "WidgetPtr.h"
#include "Switch.h"
#include "Empty.h"
#include "Selecter.h"

#ifndef WidgetBuilderH
#define WidgetBuilderH

namespace widget
{
    class Builder
    {

    public:
        template <TWidget T>
        class BuildingWidget final
        {
            T _state;

        public:
            BuildingWidget(T &&widget) : _state(std::move(widget)) {};

            BuildingWidget<Padder<T>> addPadding(RectF padding = {{0}, {0}}, D2D1::ColorF color = D2D1::ColorF{0, 0}) &&
            {
                return {Padder<T>{std::move(_state), padding, color}};
            };

            BuildingWidget setBorder(D2D1::ColorF color = D2D1::ColorF(0, 1), int width = 1, int radius = 10) &&
                requires requires(T a) { a.setBorder(D2D1::ColorF(0, 0), 1, 1); }
            {
                _state.setBorder(color, width, radius);
                return std::move(*this);
            };
            BuildingWidget setEditableText() &&
            {
                _state.isEditable = true;
                return std::move(*this);
            };

            template <std::invocable<MouseButton> FDown, std::invocable<MouseButton> FUp>
            BuildingWidget<Button<T>> addButton(FDown &&callbackDown, FUp &&callbackUp) &&
            {
                return {Button<T>{std::move(_state),
                                  std::forward<FDown>(callbackDown), std::forward<FUp>(callbackUp)}};
            };
            template <std::invocable<MouseButton> FDown>
            BuildingWidget<Button<T>> addButton(FDown &&callbackDown) &&
            {
                return {Button<T>{std::move(_state),
                                  std::forward<FDown>(callbackDown), std::function<void(MouseButton)>{}}};
            };
            BuildingWidget<Switch<T>> addSwitch(bool enabled = true) &&
            {
                return {Switch<T>{std::move(_state), enabled}};
            };
            template<typename FSel, typename FUnsel>
            BuildingWidget<T> setSelecter(FSel &&onSelect, FUnsel &&onUnSelect, std::size_t selected = 0) &&
            {
                _state.initSelecter(std::forward<FSel>(onSelect), 
                                std::forward<FUnsel>(onUnSelect), 
                                selected);
                return std::move(*this);
            };
            BuildingWidget setSize(RectF newSize) &&
            {
                _state.onResize(newSize);
                return std::move(*this);
            };

            T build() &&
            {
                return std::move(_state);
            }
            std::unique_ptr<T> buildPtr() &&
            {
                return std::make_unique<T>(std::move(_state));
            }
            WidgetPtr buildWidgetPtr() &&
            {
                return WidgetPtr(std::move(*this).buildPtr());
            }
        };

        static auto makeText(std::wstring_view str, float size = -1, D2D1::ColorF color = D2D1::ColorF(0, 1))
        {
            return BuildingWidget{TextBox{std::wstring(str), size, color}};
        };
        static auto makeEmpty()
        {
            return BuildingWidget{Empty{}};
        };
        template <TWidget T, typename ... Args>
        static auto makeList(Args ... args)
        {
            return BuildingWidget{ListFinal<T>(args...)};
        };
        template <TWidget T,  typename ... Args>
        static auto makeSelecter(Args ... args)
        {
            return BuildingWidget{Selecter<T>(args...)};
        };
        template <TWidget... Args>
        static auto makeOverlay(std::pair<RectF, Args &&>... args)
        {
            return BuildingWidget{OverlayFinal<Args...>(std::move(args)...)};
        };
        static auto makePopUpNotification(std::wstring_view message, RectF position)
        {
            return makeOverlay<Padder<TextBox>>({RectF({0.3, 0.4}, {0.7, 0.6}),makeText(message)
                .addPadding(RectF{{30, 0}}, D2D1::ColorF(D2D1::ColorF::LightSlateGray))
                .setBorder()
                .build()}).setSize(position).buildPtr();
        }
        template <std::invocable<MouseButton> FDown>
        static auto makePopUpNotification(std::wstring_view message, RectF position, std::wstring_view messageButton, FDown &&callbackDown)
        {
            return makeOverlay<Padder<OverlayFinal<TextBox, Button<TextBox> >> >(
                    {RectF({0.3, 0.3}, {0.7, 0.6}),
                        makeOverlay<TextBox, Button<TextBox>>(
                            {RectF({0, 0}, {1, 0.7 }), makeText(message).build()},
                            {RectF({0, 0.7}, {1, 0.95}), makeText(messageButton)
                                .addButton(std::forward<FDown>(callbackDown)).build()})
                        .addPadding(RectF{{30, 0}}, D2D1::ColorF(D2D1::ColorF::LightSlateGray))
                        .setBorder().build()}
                    ).setSize(position).buildPtr();
        }
    };
}
#endif