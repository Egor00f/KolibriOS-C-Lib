#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#include <include_ksys.h>

#include <string>

#include <kolibriLib/UI/UI.hpp>
#include <kolibriLib/UI/buttons/button.hpp>
#include <kolibriLib/color.hpp>

namespace KolibriLib
{
    namespace UI
    {
        /// @brief Класс для работы с чекбоксами
        /// @paragraph

        /**
         * @brief
         * @details Это просто чекбокс, ему можно задать различные стили
         */
        class CheckBox : public buttons::Button
        {
        public:
            /// @brief Стиль Чекбокса
            enum class style
            {
                /// @brief По вид умолчанию, квадрат
                Default,

                /// @brief В виде круга
                Circle,

                /// @brief Квадрат с со сглаженными углами
                /// @warning Не реализованно (пока что)
                Smooth
            };

            CheckBox() = default;

            /// @brief Конструктор
            /// @param coord координаты верхнего левого угла
            /// @param size рамеры
            /// @param Style стиль
            /// @param CheckBoxBorderColor
            /// @param BackgroundColor
            /// @param Margin
            CheckBox(const UDim &coord = point(0), const UDim &size = DefaultSize, const style &Style = style::Default, const Colors::Color &CheckBoxBorderColor = Globals::SystemColors.work_text, const Colors::Color &BackgroundColor = Globals::SystemColors.work_area, const unsigned &Margin = DefaultMargin);

            /// @brief
            /// @param a
            CheckBox(const CheckBox &a);

            /// @brief Отрисовать рамки
            void DrawBorder() const;

            /// @brief Отрисовать Checkbox
            void Render() const override;

            /// @brief Изменить стиль
            /// @param s Новый стиль
            void SetStyle(style s);

            /// @brief Получить состояние чекбокса
            /// @return
            bool GetChecked() const;

            bool OnButtonEvent(buttons::ButtonID PressedButtonID);

            /**
             * @brief Изменить цвет рамки
             * @param NewBorderColor
             */
            void SetBorderColor(const Colors::Color &NewBorderColor);

            /**
             * @brief Получить цвет рамки
             * @return
             */
            Colors::Color GetBorderColor() const;

            void swap(CheckBox &a);

            CheckBox &operator=(const CheckBox &) = default;

            /**
             * @brief
             * @param
             * @return
             */
            bool operator==(const CheckBox &) const;

            /**
             * @brief
             * @param
             * @return
             */
            bool operator!=(const CheckBox &) const;

        private:
            /**
             * @brief Цвет рамки
             */
            Colors::Color _BorderColor;

            /**
             * @brief Стиль чекбокса
             */
            style _style = style::Default;

            /**
             * @brief Состояние чекбокса
             */
            mutable bool checked;
        };
        /** @example Checkbox.cpp
         * пример использования CheckBox
         */
    } // namespace UI

} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::CheckBox &element)
{
    return os << "CheckBox:" << std::endl
              << static_cast<KolibriLib::UI::UIElement>(element) << std::endl
              << static_cast<KolibriLib::UI::buttons::BaseButton>(element);
}

#endif // __CHECKBOX_HPP__