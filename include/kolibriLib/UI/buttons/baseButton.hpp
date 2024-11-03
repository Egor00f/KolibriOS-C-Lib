#ifndef __BASEBUTTON_HPP__
#define __BASEBUTTON_HPP__

#include <kolibriLib/debug.hpp>
#include "ButtonsIDController.hpp"

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

                /// @brief Конструктор
                /// @param id ID кнопки
                BaseButton(ButtonID id);

                /**
                 * @brief Конструктор копирования
                 * @param button что копировать
                 */
                BaseButton(const BaseButton &button);

                /// @brief Деструктор
                ~BaseButton();

                /// @brief Получить состояние кнопки на момент последней обработки
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
                /// @details просто запрашивает новый ID у ButtonsIDController (старый конечно же освобождает)
                void SetId();

                /// @brief Проверить имеет ли кнопка ID
                /// @return true если у кнопки есть ID
                bool IsActive() const;

                /// @brief Объявить кнопку
                /// @param coord Координаты кнопки
                /// @param size размер кнопки
                /// @param color цвет кнопки
                void Define(const Coord &coord, const Size &size, const Colors::Color &color = Globals::SystemColors.work_button) const;

            protected:
                /// @brief ID кнопки
                ButtonID _id = buttons::ButtonIDNotSet;

                /**
                 * @brief Указатель на контроллер ID кнопок
                 */
                ButtonsIDController *_ButtonsIDController = nullptr;

                /// @brief Состояние кнопки(Нажата/Не нажата)
                mutable bool _status = false;

            private:
                /**
                 * @brief Тип кнопки
                 * @details false - системная кнопка, true кастомная
                 * @warning Не завершено
                 */
                bool _type;
            };

        } // namespace buttons

    } // namespace UI

} // namespace KolibriLib

/**
 * @brief Оператор
 * @param os поток
 * @param element кнопка
 * @return 
 */
inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::buttons::BaseButton &element)
{
    return os << "BaseButton:\nID: " << element.GetId() << std::endl;
}

#endif // __BUTTON_HPP__