#ifndef __BUTTONSBASE_HPP__
#define __BUTTONSBASE_HPP__

#include <include_ksys.h>

#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/debug.hpp>
#include <kolibriLib/globals.hpp>
#include <ostream>

#include <vector>
#include <memory>

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
				/// @brief Значение
				std::uint32_t value;

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

				/// @brief
				operator std::uint32_t() const;

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
			 * @brief Автоматическое присвоение ID для кнопок
			 * @details Также сохраняет указатель на кнопку, которая запросила ID
			 * @note Этот класс работает только с ID кнопок и ничего больше
			 */
			class ButtonsIDController
			{
			public:
				/**
				 * @brief Нода
				 */
				struct node
				{
					/**
					 * @brief ID кнопок
					 */
					ButtonID id;

					/**
					 * @brief Указатели на кнопки, исползующие этот ID
					 */
					std::vector<std::weak_ptr<BaseButton>> pointers;

					/**
					 * @brief Конструктор
					 * @param Id
					 */
					node(ButtonID Id);

					/**
					 * @brief Конструктор
					 * @param Id
					 * @param p
					 */
					node(ButtonID Id, std::weak_ptr<BaseButton> p);

					node &operator=(const node &) = default;

					/**
					 * @brief оператор равенства
					 * @param val с чем сравнивать
					 * @return
					 */
					bool operator==(const node &val) const;

					/**
					 * @brief Оператор неравентсва
					 * @param val
					 * @return
					 */
					bool operator!=(const node &val) const;
				};

				/**
				 * @brief Алиас для вектора с нодами
				 */
				using List = std::vector<node>;

				/**
				 * @brief Конвертировать List в список кнопок
				 * @param list
				 * @return
				 */
				static ButtonIDList ListoButtonIDList(const List &list);

				/// @brief Получить свободный ID кнопки из списка
				/// @return ID кнопки, который не занят
				ButtonID GetFreeButtonID(std::weak_ptr<BaseButton> ptr);

				/**
				 * @brief Занять ID кнопки
				 * @param id ID кнопки
				 * @param ptr указатель на кнопку
				 */
				void TakeupButtonID(const ButtonID &id, std::weak_ptr<BaseButton> ptr);

				/// @brief Освободить ID
				/// @param id ID который нужно освободить
				void FreeButtonID(const ButtonID &id);

				/**
				 * @brief Получить список всех занятых ID кнопок
				 * @return указатель на вектор
				 * @details Зачем указатель? копировать долго
				 */
				ButtonsIDController::List &GetButtonsIDList();

				/**
				 * @brief Получить список всех занятых ID кнопок
				 * @return указатель на вектор
				 * @details константная версия
				 */
				const ButtonsIDController::List &GetButtonsIDList() const;

				/**
				 * @brief Получить указатель на кнопку, которая запросила ID
				 * @param ID ID кнопки
				 * @return указатель на ту самую кнопку
				 */
				std::vector<std::weak_ptr<BaseButton>> GetPoinerToButton(const ButtonID &ID) const;

				/**
				 * @brief Убрать лишнее
				 * @details Убирает лишние ID если они повторяются, и если нет ни одного действующего указателя
				 */
				void Sort();

			private:
				/**
				 * @brief Список использованных id кнопок
				 * @details По идее CloseButton тоже входить дожна в этот список, но не входит так как сразу начинаем со второго ID. Чисто немного оптимизация
				 */
				ButtonsIDController::List ButtonsIdList{/*CloseButton,*/ node(MinimizeButton)};

				/**
				 * @brief Стартовое значение ButtonsIDController::_top
				 */
				const unsigned StartTop = 2;

				/**
				 * @brief Типа вершина
				 * @details чтобы по всему вектору не проходиться, отсчёт начинаем с top
				 */
				unsigned _top = StartTop;
			};

			/// @brief Стиль кнопок
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
			 * @param id номер номер кнопки из списка ButtonsIdList
			 * @param ButtonsIdList Указатель на список ID кнопок
			 * @return true если всё ок, иначе false
			 */
			bool FreeButtonId(const ButtonID &id, ButtonIDList &ButtonsIdList);

			/// \brief Создать кнопку, автоматически присвоить ей id
			/// \param coords координаты
			/// \param size размер
			/// \param color цвет
			/// \return id созданной кнопки
			ButtonID autoDefineButton(ButtonIDList &ButtonsIdList, const Coord &coords, const Size &size, const Colors::Color &color = Globals::SystemColors.work_button);

			/// \brief Создать кнопку, вручную
			/// \param coords координаты
			/// \param size размер
			/// \param id idшник кнопки
			/// \param color цвет
			inline void DefineButton(const Coord &coord, const Size &size, const ButtonID &id, Colors::Color color = Globals::SystemColors.work_button)
			{
				_ksys_define_button(
					static_cast<std::uint32_t>(coord.x),
					static_cast<std::uint32_t>(coord.y),
					static_cast<std::uint32_t>(size.x),
					static_cast<std::uint32_t>(size.y),
					static_cast<std::uint32_t>(id),
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
				_ksys_delete_button(static_cast<std::uint32_t>(id));
			}

			/// \brief Удалить кнопу
			/// \param id id удаляемой кнопки
			inline void DeleteButton(ButtonIDList &ButtonsIdList, ButtonID id)
			{
				_ksys_delete_button(id);
				FreeButtonId(id, ButtonsIdList); // Кнопка удалена, теперь этот id не используется
			}

			/// @brief проверить какая кнопка нажата
			/// @return id нажатой кнопки
			inline ButtonID GetPressedButton()
			{
				return ButtonID(_ksys_get_button());
			}

			/// @brief Изменить стиль кнопок
			/// @param новый стиль кнопок
			inline void SetButtonStyle(ButtonStyle style)
			{
				asm_inline(
					"int $0x40" ::"a"(48), "b"(1), "c"(style));
			}

		} // namespace buttons

	} // namespace UI

	namespace Globals
	{
		/**
		 * @brief Контроллер кнопок ID по умолчанию
		 * @details Используется по умолчанию кнопками.
		 * По умолчанию равен nullptr.
		 * Устанавливается классом window::Window
		 */
		extern UI::buttons::ButtonsIDController *DefaultButtonsIDController;
	}
} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::buttons::ButtonID &id)
{
	return os << id.value;
}

#endif // __BUTTONSBASE_HPP__