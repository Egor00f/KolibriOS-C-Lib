#ifndef __BUTTONS_ID_CONTROLLER_HPP__
#define __BUTTONS_ID_CONTROLLER_HPP__

#include <kolibriLib/UI/buttons/buttonsBase.hpp>
#include <memory>

namespace KolibriLib
{
	namespace UI
	{
		namespace buttons
		{
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
					 * @brief Указатели на кнопки, использующие этот ID
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
					 * @brief Оператор не равенства
					 * @param val с чем сравнивать
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
				static ButtonIDList ListToButtonIDList(const List &list);

								/**
				 * @brief Получить свободный ID кнопки из списка
				 * @param ptr Указатель на кнопку, которая получает ID. Можно nullptr
				 * @return ID кнопки, который не занят
				 * @todo Надо оптимизировать алгоритм поиска, а то он кривой и медленный
				 */
				ButtonID GetFreeButtonID(std::weak_ptr<BaseButton> ptr);

				/**
				 * @brief Занять ID кнопки
				 * @param id ID кнопки
				 * @param ptr указатель на кнопку
				 */
				void TakeUpButtonID(const ButtonID &id, std::weak_ptr<BaseButton> ptr);

				/**
				 * @brief Освободить ID
				 * @param id ID который нужно освободить
				 * @todo Надо оптимизировать алгоритм поиска, а то он кривой и медленный
				 */
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
				std::vector<std::weak_ptr<BaseButton>> GetPointerToButton(const ButtonID &ID) const;

				/**
				 * @brief Убрать лишнее
				 * @details Убирает лишние ID если они повторяются, и если нет ни одного действующего указателя
				 */
				void Sort();

			private:
				/**
				 * @brief Список использованных id кнопок
				 * @details По идее CloseButton тоже входить должна в этот список, но не входит так как сразу начинаем со второго ID. Чисто немного оптимизация
				 */
				ButtonsIDController::List _ButtonsIdList{/*CloseButton,*/ node(MinimizeButton)};

				/**
				 * @brief Стартовое значение ButtonsIDController::_top
				 */
				const unsigned _StartTop = 2;

				/**
				 * @brief Типа вершина
				 * @details чтобы по всему вектору не проходиться, отсчёт начинаем с top
				 */
				unsigned _top = _StartTop;
			};
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
	} // namespace Globals

} // namespace KolibriLib

#endif // __BUTTONS_ID_CONTROLLER_HPP__