#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "baseButton.hpp"
#include <kolibriLib/UI/UI.hpp>

namespace KolibriLib
{
    namespace UI
    {
        namespace buttons
        {
            /**
             * @brief Просто кнопка
             */
            class Button : public UIElement, public BaseButton
            {
            public:
                /// @brief Конструктор по умолчанию
                Button();

                /// @brief Конструктор
                /// @param coord координаты кнопки
                /// @param size размер кнопки
                /// @param Margin
                /// @param backgroundColor цвет фона
                Button(UDim coord, UDim size, unsigned Margin = DefaultMargin, Colors::Color backgroundColor = Globals::SystemColors.work_button);

                /**
                 * @brief Конструктор копирования
                 * @param button 
                 */
                Button(const Button& button);

                /// @brief отрисовать кнопку
                void Render() const override;

                bool OnButtonEvent(ButtonID PressedButtonID) override;

                /// @brief
			    /// @return
			    buttons::ButtonsIDController *GetButtonIDController() const override;

			    /**
			    * @brief
			    * @param buttonsIDController
			    */
			    void SetButtonIDController(const buttons::ButtonsIDController *buttonsIDController) override;

                /**
                 * @brief 
                 * @param a 
                 */
                void swap(Button &a);

                /// @brief оператор
                /// @param
                /// @return
                Button &operator=(const Button &) = default;

                /**
                 * @brief
                 * @param val то с чем сравнивать
                 * @return
                 */
                bool operator==(const Button &val) const;

                /**
                 * @brief
                 * @param val
                 * @return
                 */
                bool operator!=(const Button &val) const;

            private:
            };
        } // namespace buttons
    } // namespace UI
} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::buttons::Button &element)
{
    return os << static_cast<KolibriLib::UI::buttons::BaseButton>(element) << std::endl
              << static_cast<KolibriLib::UI::UIElement>(element);
}

#endif // __BUTTON_HPP__