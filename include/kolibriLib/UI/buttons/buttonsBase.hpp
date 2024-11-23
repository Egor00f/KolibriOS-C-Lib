#ifndef __BUTTONSBASE_HPP__
#define __BUTTONSBASE_HPP__

#include <include_ksys.h>
#include <assert.h>
#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/debug.hpp>
#include <kolibriLib/globals.hpp>
#include <ostream>
#include <vector>

namespace KolibriLib
{
	namespace UI
	{
		/**
		 * @brief Работа с кнопками
		 */
		namespace buttons
		{
			/// @brief ID кнопки
			struct ButtonID
			{
				/**
				 * @brief Значение
				 */
				std::uint32_t value = 0;

				/**
				 * @brief Конструктор по умолчанию
				 */
				ButtonID() = default;

				/**
				 * @brief Конструктор копирования
				 * @param
				 */
				ButtonID(const ButtonID &) = default;

				/**
				 * @brief Конструктор
				 * @param val значение
				 */
				ButtonID(unsigned val);

				/// @brief является ли этот ID рабочим
				/// @details ID кнопок могут быть только в промежутке (0; 0x8000)
				bool CheckIsValid() const;

				/**
				 * @brief
				 * @param
				 * @return
				 */
				ButtonID &operator=(const ButtonID &) = default;

				/**
				 * @brief Оператор присваивания
				 * @param val значение
				 * @return
				 * @details Просто берёт значение val и запихивает в ButtonID::value
				 */
				ButtonID operator=(const unsigned &val);

				/**
				 * @brief
				 * @param val
				 * @return
				 */
				bool operator==(const ButtonID &val) const;

				/**
				 * @brief
				 * @param val
				 * @return
				 */
				bool operator!=(const ButtonID &val) const;

				/**
				 * @brief Оператор
				 */
				operator bool() const
				{
					return CheckIsValid();
				}

				void swap(ButtonID &val);
			};

			/**
			 * @brief ID кнопки не установлен
			 */
			const ButtonID ButtonIDNotSet = 0;

			/// @brief Конец спика ID кнопок
			const ButtonID ButtonIDEnd = 0x8000;

			/**
			 * @brief Id кнопки закрытия окна
			 * @details Эта кнопка создаётся только если окно объявлено со скином
			 */
			const ButtonID CloseButton = 1;

			/// @brief Id кнопки минимизации окна
			const ButtonID MinimizeButton = 0xffff;

			/**
			 * @brief Список ID кнопок
			 * @details много писать неохота
			 */
			using ButtonIDList = std::vector<ButtonID>;

			class BaseButton;

			/**
			 * @brief Стиль кнопок
			 */
			enum class ButtonStyle
			{
				/// @brief Плоские кнопки
				flat = 0,

				/// @brief Объёмные кнопки
				volumetric = 1
			};

			/// \brief Получить свободный номер id кнопки из списка
			/// \paragraph Эта функция может выполнятся очень долго, если вы уже создали довольно много кнопок. Это становится действительно важно когда у вас объявленно более 2000 кнопок
			/// \return номер кнопки из списка ButtonsIdList
			ButtonID GetFreeButtonId(ButtonIDList &ButtonsIdList, std::uint32_t startID = 2);

			/**
			 * @brief Освободить номер кнопки
			 * @param ButtonsIdList Указатель на список ID кнопок
			 * @param id номер номер кнопки из списка ButtonsIdList
			 * @return true если всё ок, иначе false
			 */
			bool FreeButtonId(ButtonIDList &ButtonsIdList, const ButtonID &id);

			/// \brief Создать кнопку, автоматически присвоить ей id
			/// \param coords координаты
			/// \param size размер
			/// \param color цвет
			/// \return id созданной кнопки
			ButtonID autoDefineButton(ButtonIDList &ButtonsIdList, const Coord &coords, const Size &size, const Colors::Color &color = Globals::SystemColors.work_button);

			/**
			 * \brief Создать кнопку, вручную
			 * \param coords координаты
			 * \param size размер
			 * \param id id кнопки
			 * \param color цвет
			 */
			inline void DefineButton(const Coord &coord, const Size &size, ButtonID id, Colors::Color color = Globals::SystemColors.work_button)
			{
				assert(id.operator bool());

				_ksys_define_button(
					static_cast<std::uint32_t>(coord.x),
					static_cast<std::uint32_t>(coord.y),
					static_cast<std::uint32_t>(size.x),
					static_cast<std::uint32_t>(size.y),
					id.value,
					static_cast<ksys_color_t>(color));
			}

			/**
			 * \brief Создать кнопку, вручную
			 * @param ButtonsIdList Указатель на список ID кнопок
			 * \param coords координаты
			 * \param size размер
			 * \param id ID кнопки
			 * \param color цвет
			 */
			inline void DefineButton(ButtonIDList &ButtonsIdList, const Coord &coord, const Size &size, const ButtonID &id, Colors::Color color = Globals::SystemColors.work_button)
			{
				ButtonsIdList.push_back(id);
				DefineButton(coord, size, id, color);
			}

			/// \brief Удалить кнопу
			/// \param id id удаляемой кнопки
			inline void DeleteButton(ButtonID id)
			{
				assert(id.operator bool());

				_ksys_delete_button(id.value);
			}

			/// \brief Удалить кнопу
			/// \param id id удаляемой кнопки
			inline void DeleteButton(ButtonIDList &ButtonsIdList, ButtonID id)
			{
				assert(id.operator bool());

				_ksys_delete_button(id.value);
				FreeButtonId(ButtonsIdList, id); // Кнопка удалена, теперь этот id не используется
			}

			/// @brief проверить какая кнопка нажата
			/// @return id нажатой кнопки
			inline ButtonID GetPressedButton()
			{
				return ButtonID(_ksys_get_button());
			}

			/**
			 * @brief Изменить стиль кнопок
			 * @param новый стиль кнопок
			 * @details После изменения стиля кнопок необходимо перерисовать экран функцией
			 */
			inline void SetButtonStyle(ButtonStyle style)
			{
				asm_inline(
					"int $0x40" ::"a"(48), "b"(1), "c"(style));
			}

		} // namespace buttons

	} // namespace UI

} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::buttons::ButtonID &id)
{
	return os << id.value;
}

#endif // __BUTTONSBASE_HPP__