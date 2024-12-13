#include "Button.h"
#include "Padder.h"
#include "TextBox.h"
#include "List.h"
#include "Overlay.h"

#ifndef WidgetBuilderH
#define WidgetBuilderH

namespace widget
{

    class Builder
    {
    public:
        template <TWidget T>
        class BuildingWidget
        {
            T _state;

        public:
            BuildingWidget(T &&widget) : _state(std::move(widget)) {};

            BuildingWidget<Padder<T>> addPadding(RectF padding = {{0},{0}}, D2D1::ColorF color = D2D1::ColorF{0, 0}) &&
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

            BuildingWidget<Button<T>> addButton(auto &&callback) &&
            {
                return {Button<T>{std::move(_state), std::move(callback)}};
            };
            T build() &&
            {
                return std::move(_state);
            }
            // Build with setup size
            T build(RectF newSize) &&
            {
                _state.onResize(newSize);
                return std::move(_state);
            }
            std::unique_ptr<T> buildPtr() &&
            {
                return std::make_unique<T>(std::move(_state));
            }
            // Build with setup size
            std::unique_ptr<T> buildPtr(RectF newSize) &&
            {
                _state.onResize(newSize);
                return std::make_unique<T>(std::move(_state));
            }
        };
        
        static auto makeText(std::wstring_view str, float size = -1, D2D1::ColorF color = D2D1::ColorF(0, 1))
        {
            return BuildingWidget{TextBox{std::wstring(str), size, color}};
        };
        template<TWidget T>
        static auto makeList(float padding, float height, float heightPercentage = 0)
        {
            return BuildingWidget{List<T>(padding, height, heightPercentage)};
        };
        template <TWidget... Args>
        static auto makeOverlay(std::pair<RectF, Args &&> ... args)
        {
            return BuildingWidget{Overlay<Args...>(std::move(args)...)};
        };
        static auto makePopUpNotification(std::wstring_view message, RectF size)
        {
            return makeText(message)
                .addPadding(RectF{{30, 0}}, D2D1::ColorF(D2D1::ColorF::LightSlateGray))
                .setBorder()
                .buildPtr(size);
        }
    };
}
#endif