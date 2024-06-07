#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include <sys/ksys.h>

#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <input.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <UI.hpp>
#include <kolibriLib/graphic/screen.hpp>
#include <kolibriLib/graphic/background.hpp>

namespace KolibriLib
{

	/// \brief Работа с окном
	namespace window
	{

		/// @brief Класс для работы с окном
		/// @paragraph По простому: Окно остаётся привязаным к потоку, в которм бы вызван конструктор
		/// @paragraph Для тех кто знает:
		class Window
		{
		public:
			/// @brief Структура содержащая элемент
			struct Element
			{
				/// @brief Тип элемента
				enum Type
				{
					None,
					Button,
					Image,
					CheckBox,
					Form,
					TextLabel,
					Frame,
					Menu
				};

				void *pointer;

				/// @brief Тип элемента
				unsigned _type = Type::None;

				Element();

				Element(const Element &e);

				/// @brief Деструктор
				~Element();

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::text::TextLabel &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::CheckBox &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::Images::Image &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::buttons::Button &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::Form &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::Frame &elem);

				/// @brief Изменить элемент
				/// @param elem элемент
				void SetElement(const UI::Menu &elem);

				void free();

				/// @brief Отрисовать элемент
				void Render() const;
			};

			/// @brief Номер элемента в Window::_Elements
			typedef int ElementNumber;

			/// @brief Конструктор
			/// @param Title Заголовок окна
			/// @param size Размер окна
			/// @param style стиль окна
			/// @param colors Цвет окна
			/// @param Margin Отступы
			Window(const std::string &Title = "Window", const Size &size = DefaultWindowSize, const Colors::ColorsTable &colors = OS::GetSystemColors(), bool Resize = false, bool RealtimeReadraw = false, bool Gradient = false, unsigned Transparency = 0, const unsigned &Margin = 0);

			/// @brief Полная перересовка окна
			void Redraw();

			/// @brief Отрисовать окно
			void Render(const Coord &coord = DefaultWindowCoord);

			/// @brief Получить рамер отступов в этом окне
			/// @return @link _MARGIN
			unsigned GetMargin() const;

			/// @brief Получить размер окна
			/// @return @link _size
			Size GetSize() const;

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

			/// @brief Получить координаты окна
			/// @return
			Coord GetCoord() const;

			/// @brief Изменить окно
			/// @param coord позиция
			/// @param size размер
			void ChangeWindow(const Coord &coord, const Size &size);

			/// @brief Изменить заголовок окна
			/// @param newTitle новый заголовок
			void ChangeTilte(const std::string &newTitle);

			/// @brief Удалить элемент
			/// @param id idшник того элемента, которой нужно удалить
			void DeleteElement(Window::ElementNumber id);

			/// @brief Обработчик окна
			/// @return Ивент
			OS::Event Handler();

			/// @brief Проверить какая нажата
			UI::buttons::ButtonID GetPressedButton();

			/// @brief Получить текст введённый в форму
			/// @param form номер формы в списке
			/// @return Функция возвращает текст введённый в формы
			std::string GetInputFromFrom(int form) const;

			template <class T>
			/// @brief Добавить UI элемент
			/// @param element
			/// @return std::pair<номер элемента в списке, указатель на элемент>
			ElementNumber AddElement(const T &element);

			template <class T>
			/// @brief Изменить элемент
			/// @param i
			/// @param element
			void SetElement(Window::ElementNumber i, const T &element);

			/// @brief Получить Window::Element
			/// @param i ключ
			/// @return
			Window::Element GetElement(int i);

			/// @brief Снять фокус с этого окна
			void Unfocus() const;
			/// @brief Поставить фокус на это окно
			void Focus() const;

			void RenderAllElements() const;

		private:
			/// @brief Заголовок окна
			std::string _title;

			/// @brief Список всех кнопок этого окна
			std::unordered_map<Window::ElementNumber, Element> _Elements;

			/// @brief Цвета окна
			Colors::ColorsTable _colors;

			/// @brief отступы от края окна
			unsigned _MARGIN;

			/// @brief Стиль окна
			unsigned _style;

			/// @brief Прозрачность окна
			unsigned _Transparency;

			/// @brief Активная фарма
			mutable int activeForm = -1;

			/// @brief Окно перерисовывается сейчас (да/нет)
			mutable bool _Redraw = false;

			/// @brief Окно пересовывается при перетаскивании
			bool _RealtimeRedraw;
		};

		//=============================================================================================================================================================

		KolibriLib::window::Window::Element::Element(const Element &e) : _type(e._type)
		{
			switch (e._type)
			{
			case Window::Element::Type::TextLabel:
				pointer = new UI::text::TextLabel(*((UI::text::TextLabel *)e.pointer));
				break;
			case Window::Element::Type::Image:
				pointer = new UI::Images::Image(*((UI::Images::Image *)e.pointer));
				break;
			case Window::Element::Type::Form:
				pointer = new UI::Form(*((UI::Form *)e.pointer));
				break;
			case Window::Element::Type::CheckBox:
				pointer = new UI::CheckBox(*((UI::CheckBox *)e.pointer));
				break;
			case Window::Element::Type::Button:
				pointer = new UI::buttons::Button(*((UI::buttons::Button *)e.pointer));
				break;
			case Window::Element::Type::Frame:
				pointer = new UI::Frame(*((UI::Frame *)e.pointer));
				break;
			case Window::Element::Type::Menu:
				pointer = new UI::Menu(*((UI::Menu *)e.pointer));
				break;
			}
		}

		void KolibriLib::window::Window::Element::SetElement(const UI::buttons::Button &elem)
		{
			free();
			pointer = new UI::buttons::Button(elem);
			_type = Type::Button;
		}

		void KolibriLib::window::Window::Element::SetElement(const UI::Images::Image &elem)
		{
			free();
			pointer = new UI::Images::Image(elem);
			_type = Type::Image;
		}

		void KolibriLib::window::Window::Element::SetElement(const UI::CheckBox &elem)
		{
			free();
			pointer = new UI::CheckBox(elem);
			_type = Type::CheckBox;
		}

		void KolibriLib::window::Window::Element::SetElement(const KolibriLib::UI::Form &elem)
		{
			free();
			pointer = new UI::Form(elem);
			_type = Type::Form;
		}

		void KolibriLib::window::Window::Element::SetElement(const KolibriLib::UI::text::TextLabel &elem)
		{
			free();
			pointer = new UI::text::TextLabel(elem);
			_type = Type::TextLabel;
		}

		void KolibriLib::window::Window::Element::SetElement(const KolibriLib::UI::Menu &elem)
		{
			free();
			pointer = new UI::Menu(elem);
			_type = Type::Menu;
		}

		void KolibriLib::window::Window::Element::SetElement(const KolibriLib::UI::Frame &elem)
		{
			free();
			pointer = new UI::Frame(elem);
			_type = Type::Frame;
		}

		void Window::Element::Render() const
		{
			switch (_type)
			{
			case TextLabel:
				((UI::text::TextLabel *)pointer)->Render();
				break;
			case Button:
				((UI::buttons::Button *)pointer)->Render();
				break;
			case Image:
				((UI::Images::Image *)pointer)->Render();
				break;
			case Form:
				((UI::Form *)pointer)->Render();
				break;
			case CheckBox:
				((UI::CheckBox *)pointer)->Render();
				break;
			case Menu:
				((UI::Menu *)pointer)->Render();
				break;
			case Frame:
				((UI::Frame *)pointer)->Render();
				break;
			default:
				break;
			}
		}

		KolibriLib::window::Window::Element::Element()
		{
#ifdef DEBUG
			_ksys_debug_puts("KolibriLib::window::Window::Element constructor\n");
#endif
		}

		KolibriLib::window::Window::Element::~Element()
		{
			free();
		}

		void KolibriLib::window::Window::Element::free()
		{
			switch (_type)
			{
			case Element::Type::TextLabel:
				delete ((UI::text::TextLabel *)pointer);
				break;
			case Element::Type::Button:
				delete ((UI::buttons::Button *)pointer);
				break;
			case Element::Type::Image:
				delete ((UI::Images::Image *)pointer);
				break;
			case Element::Type::Form:
				delete ((UI::Form *)pointer);
				break;
			case Element::Type::CheckBox:
				delete ((UI::CheckBox *)pointer);
				break;
			case Element::Type::Menu:
				delete ((UI::Menu *)pointer);
				break;
			case Element::Type::Frame:
				delete ((UI::Frame *)pointer);
				break;
			default:
				break;
			}
		}

		//=============================================================================================================================================================
		template <class T>
		Window::ElementNumber KolibriLib::window::Window::AddElement(const T &element)
		{
			#ifdef DEBUG
			_ksys_debug_puts("Add element-\n");
			#endif

			Element a;

			a.SetElement(element);

			std::pair<Window::ElementNumber, T *> ret;

			for (int i = 0; i < _Elements.max_size(); i++)
			{
				if (_Elements.count(i) == 0)
				{
					_Elements.emplace(i, a);

					return i;
				}
			}
			return -1;
		}

		template <class T>
		void KolibriLib::window::Window::SetElement(ElementNumber i, const T &element)
		{
			if (_Elements.count(i))
			{
				_Elements[i].SetElement(element);
				return;
			}
			_ksys_debug_puts("KolibriLib::window::Window::SetElement: not found element\n");
			return;
		}

		//=============================================================================================================================================================

		KolibriLib::window::Window::Window(const std::string &Title, const KolibriLib::Size &size, const KolibriLib::Colors::ColorsTable &colors, bool Resize, bool RealtimeRedraw, bool Gradient, unsigned Transparency, const unsigned &Margin)
			: _title(Title), _colors(colors)
		{


			_MARGIN = Margin;
			_Transparency = Transparency;
			_RealtimeRedraw = RealtimeRedraw;

			_style = WindowStyle::Relative | WindowStyle::WindowHaveTitle | WindowStyle::withSkin;
			if (Resize)
			{
				_style |= WindowStyle::CanResize;
			}
			else
			{
				_style |= WindowStyle::FixSize;
			}

			if (Gradient)
			{
				_style |= WindowStyle::GradientDraw;
			}

			if (_Transparency > 0)
			{
				_style |= WindowStyle::NoDrawWorkspace;
			}

			window::CreateWindow(GetCoord(), size, _title, _colors.win_body, _colors.win_title, _style);
		}

		void Window::RenderAllElements() const
		{
			for (auto &it : _Elements)
			{
				it.second.Render();
			}
		}

		Window::Element KolibriLib::window::Window::GetElement(int i)
		{
			return _Elements.at(i);
		}

		void KolibriLib::window::Window::Redraw()
		{
			#ifdef DEBUG
			_ksys_debug_puts("Redraw window:");
			#endif

			StartRedraw();
			window::CreateWindow(GetCoord(), {0,0}, _title, _colors.win_body, _colors.win_title, _style);

			if (_Transparency > 0 && false)
			{
				// когданибудьбудетреализовано
			}
			else
			{
				KolibriLib::graphic::DrawRectangleFill({0, (int)window::GetSkinHeight()}, GetWindowSize(), _colors.win_body);
			}

			RenderAllElements();

			if (_Transparency != 0) // Прозрачность окна
			{
				//for (int i = 0; i < _size.y; i++)
				//{
				//	for (int j = 0; j < _size.x; j++)
				//	{
				//		graphic::DrawPixel({j, i}, Colors::BlendColors(graphic::ReadPoint({j, i}), Background::ReadPoint({j, i}), 100 / _Transparency)); // Пока так, потом может быть станет лучше
				//	}
				//}
			}

			EndRedraw();
		}

		void Window::SetWindowColors(const Colors::ColorsTable &colorTable)
		{
			_colors = colorTable;
		}

		void Window::StartRedraw() const
		{
			_ksys_start_draw();
			_Redraw = true;
		}

		void Window::EndRedraw() const
		{
			if (_Redraw)
			{
				_ksys_end_draw();
				_Redraw = false;
			}
		}

		Coord KolibriLib::window::Window::GetCoord() const
		{
			window::GetWindowCoord();
		}

		void Window::ChangeWindow(const Coord &coord, const Size &size)
		{
			_ksys_change_window(coord.x, coord.y, size.x, size.y);
		}

		void Window::ChangeTilte(const std::string &newTitle)
		{
			_ksys_set_window_title(newTitle.c_str());
		}		

		void Window::Render(const Coord &coord)
		{

			StartRedraw();
			window::CreateWindow(coord, DefaultWindowSize, _title, _colors.win_body, _colors.win_title, _style);

			RenderAllElements();


			if (_Transparency != 0) // Прозрачность окна
			{
				Size size = GetWindowSize();
				for (int i = 0; i < size.y; i++)
				{
					for (int j = 0; j < size.x; j++)
					{
						graphic::DrawPixel({j, i}, Colors::BlendColors(graphic::ReadPoint({j, i}), Background::ReadPoint({j, i}), 100 / _Transparency));
					}
				}
			}

			EndRedraw();
		}

		unsigned Window::GetMargin() const
		{
			return _MARGIN;
		}

		Size Window::GetSize() const
		{
			return window::GetWindowSize();
		}

		void Window::DeleteElement(ElementNumber id)
		{
			if (_Elements.count(id))
			{
				if (activeForm == id)
				{
					activeForm = -1;
				}
				_Elements.erase(_Elements.find(id));
			}
			_ksys_debug_puts("element this this id not found\n");
		}

		OS::Event Window::Handler()
		{
			OS::Event event = OS::WaitEvent();

			switch (event)
			{
			case OS::Events::Redraw:

				Render();

				break;
			case OS::Events::Button:

				if (UI::buttons::GetPressedButton() == 1) // Если нажата кнопка X
				{
					return OS::Events::Exit;
				}

				for (const auto &it : _Elements) // Запуск обработчиков всех используемых элементов
				{
					switch (it.second._type)
					{
					case Element::Type::Form:
						((UI::Form *)it.second.pointer)->ButtonHandler();
						break;
					case Element::CheckBox:
						((UI::CheckBox *)it.second.pointer)->Handler();
						break;
					case Element::Button:
						((UI::buttons::Button *)it.second.pointer)->Handler();
						break;
					}
				}

				break;
			case OS::Events::Key:

				if (activeForm != -1 && _Elements[activeForm]._type == Element::Type::Form)
				{
					((UI::Form *)_Elements[activeForm].pointer)->Handler(); // Обработчик активной на текущий момент формы
				}

				break;
			}
			if (_RealtimeRedraw)
			{
				Coord Mouse = mouse::GetMousePositionInWindow();
				if (((Mouse.x > 0 && Mouse.y > 0) && Mouse.x < GetWindowSize().x && Mouse.y < GetSkinHeight()) && mouse::GetMouseButtons() == mouse::LeftButton)
				{
					while (mouse::GetMouseButtons() == mouse::LeftButton)
					{
						Render(mouse::GetMousePositionOnSreen() - Mouse);
						_ksys_thread_yield();
					}
				}
			}

			return event;
		}

		UI::buttons::ButtonID Window::GetPressedButton()
		{
			for (const std::pair<int, Element> &n : _Elements)
			{
				if (n.second._type == Element::Type::Button)
				{
					if (((UI::buttons::Button *)n.second.pointer)->GetStatus())
					{
						return ((UI::buttons::Button *)n.second.pointer)->GetId();
					}
				}
			}
		}
		std::string Window::GetInputFromFrom(int form) const
		{
			auto it = _Elements.find(form);
			if (it->second._type == Window::Element::Type::Form)
			{
				return ((UI::Form *)it->second.pointer)->GetInput();
			}
		}

		void KolibriLib::window::Window::Unfocus() const
		{
			_ksys_unfocus_window(Thread::GetThreadSlot(Thread::GetThreadInfo(-1).pid));
		}

		void KolibriLib::window::Window::Focus() const
		{
			_ksys_focus_window(Thread::GetThreadSlot(Thread::GetThreadInfo(-1).pid));
		}

	} // namespace window

}

#endif