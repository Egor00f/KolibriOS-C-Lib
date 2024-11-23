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
				using node = std::pair<ButtonID, std::vector<BaseButton *>>;

				/**
				 * @brief Список нод
				 */
				using List = std::vector<node>;

				/**
				 * @brief Конвертировать List в список кнопок
				 * @param list
				 * @details Нахуй оно тут?
				 * @return
				 */
				static ButtonIDList ListToButtonIDList(const List &list);

				/**
				 * @brief Получить свободный ID кнопки из списка
				 * @param ptr Указатель на кнопку, которая получает ID. Можно nullptr
				 * @return ID кнопки, который не занят
				 * @todo Надо оптимизировать алгоритм поиска, а то он кривой и медленный
				 */
				ButtonID GetFreeButtonID(BaseButton *ptr);

				/**
				 * @brief Занять ID кнопки
				 * @param id ID кнопки
				 * @param ptr указатель на кнопку
				 * @details Нужно если необходимо повесить более одной кнопки на 1 и тот же ID
				 */
				void TakeUpButtonID(const ButtonID &id, BaseButton *ptr);

				/**
				 * @brief Освободить ID
				 * @param id ID который нужно освободить
				 * @param ptr указатель на кнопку, которая занимала этот ID
				 * @throw если ID найден, а указатель  нет.
				 * @todo Надо оптимизировать алгоритм поиска, а то он кривой и медленный
				 */
				void FreeButtonID(const ButtonID &id, BaseButton *ptr);

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
				 * @return массив указателей на ту самую кнопку
				 */
				const std::vector<BaseButton *>& GetPointerToButton(const ButtonID &ID) const;

				/**
				 * @brief Отсортировать список ID
				 * @details c отсортированным списком функция GetFreeButtonID работает быстрее
				 * есть смысл вызывать эту функцию только если ваша программа постоянно создаёт и удаляет кнопки 
				 */
				void Sort();

			private:

				/**
				 * @brief Список использованных id кнопок
				 * @details По идее CloseButton тоже входить должна в этот список, но не входит так как сразу начинаем со второго ID. Чисто немного оптимизация
				 */
				ButtonsIDController::List _ButtonsIdList{/*CloseButton,*/ {MinimizeButton, {nullptr}}};

				/**
				 * @brief Стартовое значение ButtonsIDController::_top
				 */
				const unsigned _StartTop = 2;

				/**
				 * @brief Типа вершина
				 * @details чтобы по всему вектору не проходиться, отсчёт начинаем с top
				 */
				ButtonID _top = _StartTop;
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
