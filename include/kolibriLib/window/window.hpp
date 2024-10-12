#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <cstdlib>
#include <string>
#include <utility>
#include <algorithm>

#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <input.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <KolibriLibUI.hpp>
#include <kolibriLib/graphic/screen.hpp>
#include <kolibriLib/graphic/background.hpp>
#include <kolibriLib/globals.hpp>

namespace KolibriLib
{
	namespace window
	{
		/// @brief просто класс полями для Window
		class Window_t: public UI::GuiObject
		{
		public:
			Window_t(const std::string &Title, const Colors::ColorsTable &colors = Globals::SystemColors, bool Resize = false, bool RealtimeReadraw = false, bool Gradient = false, unsigned Transparency = 0, const unsigned &Margin = UI::DefaultMargin);

			Window_t(const std::string &Title, const Colors::ColorsTable &colors = Globals::SystemColors, WindowStyle style = WindowStyle::withSkin, WindowSettings WindowSettings = WindowSettings::RelativeCoord | WindowSettings::WindowHaveTitle);

			/// @brief Изменить стиль окна
			/// @param NewStyle новый стиль окна
			void SetStyle(WindowStyle NewStyle);

			/// @brief Получить текущий стиль окна
			/// @return Текущий стиль окна
			WindowStyle GetStyle() const;

			/// @brief 
			/// @param NewSettgins 
			void SetSettings(WindowSettings NewSettgins);

			
			/**
			 * @brief 
			 * @return 
			 */
			std::string GetTitle() const;

			/**
			 * @brief 
			 * @param NewTitle 
			 */
			void SetTitle(const std::string &NewTitle);

		protected:
			/// @brief Заголовок окна
			std::string _title = "Default Window";

			/// @brief Контроллер ID кнопок
			/// @details при создании первого окна(window::Window), Globals::defaultButtonsController = _buttonsController
			UI::buttons::ButtonsIDController _buttonsController;

			/// @brief Цвета окна
			Colors::ColorsTable _colors;

			/// @brief Закешированные координаты окна
			/// @details Зачем? а потому что для получения размера окна нужно вызывать Thread::GetThreadInfo() который вызывает -ое системное прерывание, что уже не очень(каждый раз теребонькать прерывания), кароч GetAbsoluteCoord и GetAbsoluteSize исползуются при рендере, элементов gui многа, и получается каждый раз теребонькать прерывания для рендера окна будет капец долгим
			mutable Coord _coord = DefaultWindowCoord;

			/// @brief Закешированный размер окна
			/// @details с.м. Window_t::_coord
			mutable Size _size = DefaultWindowSize;

			/// @brief Стиль окна
			WindowStyle _style = WindowStyle::withSkin;

			/// @brief 
			WindowSettings _settings = WindowSettings::WindowHaveTitle;

			/// @brief Прозрачность окна
			/// @warning Будет реализованно в будующем
			uint8_t _Transparency = 0;

			/// @brief Окно пересовывается при перетаскивании
			bool _RealtimeRedraw = false;
		};

		/// @brief Класс для работы с окном
		/// @paragraph По простому: Окно остаётся привязаным к потоку, в которм бы вызван конструктор. Если вызывать методы из других потоков, то вести они себя будут неадекватно
		class Window: public Window_t
		{
		public:

			/// @brief Конструктор
			/// @param Title Заголовок окна
			/// @param size Размер окна
			/// @param style стиль окна
			/// @param colors Цвет окна
			/// @param Margin Отступы
			Window(const std::string &Title = "Window", const Size &size = DefaultWindowSize, const Coord &coord = DefaultWindowCoord, const Colors::ColorsTable &colors = Globals::SystemColors, bool Resize = false, bool RealtimeReadraw = false, bool Gradient = false, unsigned Transparency = 0, Pos position = Pos::Normal, const unsigned &Margin = UI::DefaultMargin);

			/// @brief Конструктор, необходим только для windowAttached
			Window(const Window_t &wndw);

			/// @brief Полная перересовка окна
			void Redraw();

			/// @brief Отрисовать окно
			void Render();

			/// @brief Получить размер окна
			/// @return _size
			UDim GetSize() const override;

			/// @brief Получить координаты окна
			/// @return
			UDim GetCoord() const override;

			/**
			 * @brief 
			 * @param NewSize 
			 */
			void SetSize(const UDim &NewSize) override;

			/// @brief 
			/// @param NewCoord 
			void SetCoord(const UDim &NewCoord) override;

			/// @brief 
			/// @param NewSize 
			void SetSize(const Size &NewSize) override;

			/// @brief 
			/// @param NewCoord 
			void SetCoord(const Coord &NewCoord) override;

			/// @brief Получить абсолютный размер окна
			/// @details Нужно только для классов наследуемых из UIElement
			/// @return 
			Coord GetAbsoluteCoord() const override;

			/// @brief Получить абсолютный размер окна
			/// @details Нужно только для классов наследуемых из UIElement
			/// @return
			Size GetAbsoluteSize() const override;

			/// @brief Задать стандартные цвета окна
			/// @param colorTable таблица цветов
			void SetWindowColors(const Colors::ColorsTable &colorTable);

			/// @brief Начать перересовку окна
			/// @note Стирает всё что было нарисованно в окне
			/// @note Обязательно после должна быть вызвана функция #EndRedraw()
			void StartRedraw() const;

			/// @brief Закончить перересовку окна
			/// @note Обязательно после должна быть вызвана функция #StartRedraw()
			void EndRedraw() const;

			/// @brief Изменить окно
			/// @param coord позиция
			/// @param size размер
			void ChangeWindow(const Coord &coord, const Size &size);

			/// @brief Изменить заголовок окна
			/// @param newTitle новый заголовок
			void SetTilte(const std::string &newTitle);

			/// @brief Обработчик окна
			/// @return Ивент
			OS::Event Handler();

			/**
			 * @brief Получить ID последней нажатой кнопки
			 * @return ID Послдней нажатой кнопки
			 */
			UI::buttons::ButtonID GetPressedButtonID() const;

			/**
			 * @brief Получить последнюю нажатую кнопку
			 * @return Указатель на последнюю нажатую кнопку
			 * @details Да, возрващает эта функция указатель на UIElement, но это вполне себе указатель на кнопку
			 */
			std::shared_ptr<UI::buttons::Button> GetPressedButton() const;

			/// @brief Добавить UI элемент напрямую
			/// @param element указатель на элемент
			/// @details Зачем добавлять в окно элементы ui? Да чтоб при перерисовке окна не нужнобыло отрисовывать все ручками
			/// Отличается от AddElement только тем что не создаёт указатель и копирует, а сразу использует указатель из аргумента
			/// @return указатель на элемент(новый)
			void AddElementNoCopy(UIElement *element);

			template <class T>
			/// @brief Добавить UI элемент
			/// @param element сам элемент
			/// @details Зачем добавлять в окно элементы ui? Да чтоб при перерисовке окна не нужнобыло отрисовывать все ручками
			/// @return указатель на элемент(новый)
			T* AddElement(const T &element);

			/// @brief Удалить элемент из окна
			template <class T >
			bool DeleteElement (std::shared_ptr<UIElement> element);

			/// @brief Снять фокус с этого окна
			void Unfocus () const;

			/// @brief Поставить фокус на это окно
			void Focus () const;

			/// @brief Отрисовать все элементы
			void RenderAllElements() const;

			/// @brief Изменить позицию окна относительно одних
			/// @param
			void SetPosition(const Pos &position);

			/// @brief Получить позицию окна относительно одних
			/// @return
			Pos GetPosition() const;

			UI::buttons::ButtonsIDController* GetButtonIDController() const override;

			void SetButtonIDController(const UI::buttons::ButtonsIDController* buttonsIDController) override;

			/// @brief Обновить кешированные значения
			void Update() const;

			/// @brief Получить прошлый ивент
			/// @return 
			OS::Event GetLastEvent() const;

		private:

			/// @brief Список всех кнопок этого окна
			std::vector<std::shared_ptr<UIElement>> _Elements;

			/**
			 * @brief 
			 */
			mutable std::shared_ptr<UI::buttons::Button> _PressedButton;

			/// @brief прошлый ивент
			mutable OS::Event _lastEvent;

			/// @brief Окно перерисовывается сейчас (да/нет)
			mutable bool _Redraw = false;

			/**
			 * @brief Обновленно ли окно
			 */
			mutable bool _updated = false;
		};

		//=============================================================================================================================================================

		#ifndef __MakeStaticLib__

		template <class T>
		T* KolibriLib::window::Window::AddElement(const T &element)
		{
			static_assert(std::is_base_of<UIElement, T>::value, "Ты че сюда пихаещь!?! думаешь раз шаблон то можно любой тип сюда запихнуть, а вот и нет! Иди кури документацию");

			PrintDebug("Add element\n");

			T *p = new T(element);

			if (p->GetParent())
				p->WindowAsParent(this);

			p->SetButtonIDController(&_buttonsController);

			std::shared_ptr<UIElement> s_ptr(static_cast<UIElement*>(p));

			_Elements.push_back(s_ptr);

			return static_cast<T*>(_Elements.back().get());
		}

		template <class T>
		bool KolibriLib::window::Window::DeleteElement(std::shared_ptr<UIElement> element)
		{
			auto i = std::find(_Elements.begin(), _Elements.end(), element);

			if(i != _Elements.end())
			{
				_Elements.erase(i);

				return true;
			}
			else
			{
				return false;
			}
		}

		#endif
	} // namespace window

} // namespace KolibriLib

#endif // __WINDOW_HPP__
