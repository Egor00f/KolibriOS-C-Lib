#ifndef __BASEBUTTON_HPP__
#define __BASEBUTTON_HPP__

#include <kolibriLib/debug.hpp>
#include "buttonsBase.hpp"

namespace KolibriLib
{
    namespace UI
    {
        namespace buttons
        {
            /// @brief Базовая кнопка
            class BaseButton
            {
            public:

                /**
                 * @brief Конструктор по умолчанию
                 */
                BaseButton();

				/// @brief Конструтор
				/// @param id ID кнопки
                BaseButton(ButtonID id);

                /**
                 * @brief Конструктор копирования
                 * @param button что копировать
                 */
                BaseButton(const BaseButton &button);

				/// @brief Деструктор
                ~BaseButton();

                /// @brief Получить сосояние кнопки на момент последней обработки
                /// @return _status
                bool GetStatus() const;

                /// @brief Деактивировать кнопку
                /// @details Эта функция устанавливает переменную _active в false
                /// @details В Деактивированном состоянии кнопка "Не нажимается", а её _id становится не действительным
                void Deactivate();

                /// @brief Активировать кнопку
                /// @details Противоположна функции Deactivate, возвращает кнопку в рабочее состояние
                void Activate();

                /// @brief Получить номер кнопки
                /// @return @link _id
                ButtonID GetId() const;

                /// @brief Изменить ID кнопки вручную
                /// @param NewID Новый ID кнопки
                void SetId(const ButtonID &NewID);

                /// @brief Изменить ID кнопки на какой-нибудь другой
                /// @details просто запрашивает новый ID у ButtonsIDController (старый конешноже освобождает)
                void SetId();

                /// @brief Проверить имеет ли кнопка ID
                /// @return true если у кнопки есть ID
                bool IsActive() const;

				/// @brief Объявить кнопку
				/// @param coord Координаты кнопки
				/// @param size размер кнопки
				/// @param color цвет кнокпи
                void Define(const Coord &coord, const Size &size, const Colors::Color &color = Globals::SystemColors.work_button) const;

            protected:
            
                /// @brief ID кнопки
                ButtonID _id = buttons::ButtonIDNotSet;

			    /**
			     * @brief Указатель на контроллер ID кнопок
			     */
			    ButtonsIDController *_ButtonsIDController = nullptr;

                /// @brief Состояние кнопки(Нажата/Ненажата)
                mutable bool _status;

            private:
                /// @brief Тип кнопки
                /// @details false - системная кнопка
                /// true кастомная
                bool _type;
            };

        } // namespace buttons

    } // namespace UI

} // namespace KolibriLib


#endif // __BUTTON_HPP__