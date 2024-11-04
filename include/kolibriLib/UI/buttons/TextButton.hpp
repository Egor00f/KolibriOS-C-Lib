#ifndef __TEXTBUTTON_HPP__
#define __TEXTBUTTON_HPP__

#include "button.hpp"
#include <kolibriLib/UI/text/text.hpp>

namespace KolibriLib
{
	namespace UI
	{
		namespace buttons
		{
			/// \brief Класс для работы с кнопками
			class TextButton : public Button, public text::Txt
			{
			public:
				/// \brief Это конструктор
				/// \param coord координата
				/// \param size размер
				/// \param text текст
				/// \param Margin отступы текста от границ
				/// \param ButtonColor цвет кнопки
				/// \param TextColor цвет текста
				TextButton(const UDim &coord = point(0), const UDim &size = DefaultSize, unsigned Margin = UI::DefaultMargin, Colors::Color ButtonColor = Globals::SystemColors.work_button);

				/// \brief Это конструктор
				/// \param coord координата
				/// \param size размер
				/// \param text текст
				/// \param Margin отступы текста от границ
				/// \param ButtonColor цвет кнопки
				/// \param TextColor цвет текста
				TextButton(const Txt &text, const UDim &coord = point(0), const UDim &size = DefaultSize, unsigned Margin = UI::DefaultMargin, Colors::Color ButtonColor = Globals::SystemColors.work_button);

				/**
				 * @brief Конструктор
				 * @param coord координаты
				 * @param size размер
				 * @param text текст
				 */
				TextButton(const UDim &coord, const UDim &size, const std::string &text);

				/// @brief Конструктор копирования
				/// @param textButton Кнопка которую будут копировать
				TextButton(const TextButton &textButton);

				/// @brief Отрисовать кнопку
				void Render() const override;

				TextButton &operator=(const TextButton &element) = default;

				/**
				 * @brief Оператор сравнения
				 * @param element с чем сравнивать
				 * @return true или false
				 */
				bool operator==(const TextButton &element) const;

				/**
				 * @brief Оператор сравнения
				 * @param element с чем сравнивать
				 * @return true или false
				 */
				bool operator!=(const TextButton &element) const;

				/**
				 * @brief
				 * @param a
				 */
				void swap(TextButton &a);

			private:
			};
		}
	}
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::buttons::TextButton &element)
{
	return os << "TextButton:" << std::endl
			  << static_cast<KolibriLib::UI::buttons::Button>(element) << std::endl
			  << static_cast<KolibriLib::UI::text::Txt>(element);
}

#endif // __TEXTBUTTON_HPP__