#include <kolibriLib/window/window.hpp>

using namespace KolibriLib;
using namespace window;

KolibriLib::window::Window_t::Window_t(const std::string &Title, const KolibriLib::Colors::ColorsTable &colors, bool Resize, bool RealtimeRedraw, bool Gradient, unsigned Transparency, const unsigned &Margin)
	: _title(Title),
	  _colors(colors),
	  _Transparency(Transparency)
{

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "Window:" << std::endl;
#endif

	if (Resize)
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Debug << "- with skin" << std::endl;
#endif
		_style = WindowStyle::withSkin;
	}
	else
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Debug << "- with skin and fixed size" << std::endl;
#endif
		_style = WindowStyle::FixSizeWithSkin;
	}

	if (Title != "")
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Debug << "- Have title" << std::endl;
#endif

		_settings |= WindowSettings::WindowHaveTitle;
	}

	if (Gradient)
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Debug << "- Gradient workspace" << std::endl;
#endif
		_settings |= WindowSettings::GradientDraw;
	}

	SetMargin(Margin);
}

KolibriLib::window::Window_t::Window_t(const std::string &Title, const Colors::ColorsTable &colors, WindowStyle style, WindowSettings WindowSettings)
	: _title(Title),
	  _colors(colors),
	  _style(style),
	  _settings(WindowSettings)
{
}

void KolibriLib::window::Window_t::SetStyle(WindowStyle NewStyle)
{
	_style = NewStyle;
}

std::string KolibriLib::window::Window_t::GetTitle() const
{
	return _title;
}

void KolibriLib::window::Window_t::SetTitle(const std::string &NewTitle)
{
	_title = NewTitle;
}

/*
	Window
*/

KolibriLib::window::Window::Window(const std::string &Title, const Size &size, const Coord &coord, const Colors::ColorsTable &colors, bool Resize, bool RealtimeRedraw, bool Gradient, unsigned Transparency, Pos position, const unsigned &Margin)
	: Window_t(Title, colors, Resize, RealtimeRedraw, Gradient, Transparency, Margin)

{

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "Window constructor" << std::endl;
#endif

	_coord = coord;
	_size = size;

	SetPosition(position);

	if (Globals::DefaultButtonsIDController == nullptr)
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Debug << "Set DefaultButtonsIDController" << std::endl;
#endif

		Globals::DefaultButtonsIDController = &_buttonsController;
	}

	window::CreateWindow(
		coord,
		size,
		Title,
		colors.work_area,
		colors.grab_text,
		_style);
}

KolibriLib::window::Window::Window(const Window_t &wndw)
	: Window_t(wndw)
{

	if (Globals::DefaultButtonsIDController == nullptr)
		Globals::DefaultButtonsIDController = &_buttonsController;

	window::CreateWindow(
		DefaultWindowCoord,
		DefaultWindowSize,
		_title,
		_colors.work_area,
		_colors.grab_text,
		_style,
		_settings);
}

void Window::RenderAllElements() const
{

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "RenderAllElements:" << std::endl;
#endif

	for (auto i : _Elements)
	{
		i->Render();
	}
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

UDim KolibriLib::window::Window::GetCoord() const
{
	return UDim(_coord);
}

void Window::SetCoord(const UDim &NewCoord)
{
	_coord = NewCoord.GetAbsolute(GetScreenSize());
	window::ChangeWindow(_coord, GetAbsoluteSize());
}

void Window::SetSize(const Size &NewSize)
{
	window::ChangeWindow(GetAbsoluteCoord(), NewSize);
	_size = NewSize;
}

inline void Window::SetCoord(const Coord &NewCoord)
{
	window::ChangeWindow(NewCoord, _size);
}

inline void Window::SetSize(const UDim &NewSize)
{
	window::ChangeWindow(_coord, NewSize.GetAbsolute(GetScreenSize()));
}

inline Coord Window::GetAbsoluteCoord() const
{
	if (_updated) // Если было событие перерисовки. значит возможно размер окна изменился
		Update();

	return _coord;
}

inline Size Window::GetAbsoluteSize() const
{
	if (_updated) // Если было событие перерисовки. значит возможно размер окна изменился
		Update();

	return _size;
}

void Window::ChangeWindow(const Coord &coord, const Size &size)
{
	_coord = coord;
	_size = size;
	window::ChangeWindow(coord, size);
}

void Window::SetTitle(const std::string &newTitle)
{
	Window_t::SetTitle(newTitle);
	_ksys_set_window_title(_title.c_str());
}

void KolibriLib::window::Window::Redraw()
{
	StartRedraw();
	window::CreateWindow(DefaultWindowCoord, DefaultWindowSize, _title, _colors.work_area, _colors.grab_text, _style);

	RenderAllElements();

	EndRedraw();
}

void Window::Render()
{

	StartRedraw();
	window::CreateWindow({0, 0}, {0, 0}, _title, _colors.work_area, _colors.grab_text, _style, _settings);

	KolibriLib::graphic::DrawRectangleFill(
		{0, static_cast<int>(window::GetSkinHeight())},
		GetAbsoluteSize() - static_cast<int>(GetMargin()),
		_colors.work_area);

	RenderAllElements();

	/* if (_Transparency != 0) // Прозрачность окна
	{
		Size size = GetWindowSize();
		for (int i = 0; i < size.y; i++)
		{
			for (int j = 0; j < size.x; j++)
			{
				graphic::DrawPixel({j, i}, Colors::BlendColors(graphic::ReadPoint({j, i}), Background::ReadPoint({j, i}), 100 / _Transparency));
			}
		}
	} */

	EndRedraw();
}

inline UDim Window::GetSize() const
{
	return UDim(_coord.x, _coord.y);
}

OS::Event Window::Handler()
{
	_lastEvent = OS::WaitEvent();

	switch (_lastEvent)
	{
	case OS::Event::Redraw:

		_updated = false;

		Update();

		Redraw();

		break;

	case OS::Event::Button:

	{
		UI::buttons::ButtonID PressedButton = UI::buttons::GetPressedButton();

		if (PressedButton == UI::buttons::CloseButton) // Если нажата кнопка X
		{
			_lastEvent = OS::Event::Exit;
		}
		else
		{
			for (auto it : _Elements)
			{
				it->OnButtonEvent(PressedButton);
			}
		}

		std::shared_ptr<UI::buttons::BaseButton> s_ptr;

		try
		{
			s_ptr = _buttonsController.GetPointerToButton(PressedButton).at(0).lock();
		}
		catch (...)
		{

#ifndef NO_LOGS
			logger << microlog::LogLevel::Error << "Error Get s_ptr" << std::endl;
#endif
		}

		_PressedButton = s_ptr;
	}

	break;

	case OS::Event::Key:

		for (auto it : _Elements)
		{
			it->OnKeyEvent();
		}

		break;

	case OS::Event::Mouse:

		if (mouse::MouseButtons::RightButton && mouse::GetMouseButtons()) // Если нажата правая кнопка
		{
			Coord m = mouse::GetMousePositionInWindow();

			if (m.x < GetAbsoluteSize().x && m.y < static_cast<int>(window::GetSkinHeight()))
			{
			}
		}
		else
		{
			for (auto it : _Elements)
			{
				it->OnMouseEvent();
			}
		}

		break;

	case OS::Event::Desktop:
		break;
	case OS::Event::None:
		break;
	case OS::Event::Exit:
		break;
	case OS::Event::Debug:
		break;
	default:
		break;
	}

	for (auto it : _Elements) // Запуск обработчиков всех используемых элементов
	{
		it->Handler(_lastEvent);
	}

	if (_RealtimeRedraw)
	{
		Coord Mouse = mouse::GetMousePositionInWindow();
		if (((Mouse.x > 0 && Mouse.y > 0) &&
			 Mouse.x < GetWindowSize().x &&
			 Mouse.y < static_cast<int>(GetSkinHeight())) &&
			mouse::GetMouseButtons() == mouse::LeftButton)
		{
			while (mouse::GetMouseButtons() == mouse::LeftButton)
			{
				_ksys_thread_yield();
			}
		}
	}

	return _lastEvent;
}

UI::buttons::ButtonID Window::GetPressedButtonID() const
{
	if (_PressedButton)
		return _PressedButton.get()->GetId();
	else
		return UI::buttons::ButtonIDNotSet;
}

std::shared_ptr<UI::buttons::BaseButton> KolibriLib::window::Window::GetPressedButton() const
{
	return _PressedButton;
}

void KolibriLib::window::Window::Unfocus() const
{
	window::UnfocusWindow();
}

void KolibriLib::window::Window::Focus() const
{
	window::FocusWindow();
}

void KolibriLib::window::Window::SetPosition(const window::Pos &position)
{
	window::SetWindowPos(position);
}

window::Pos KolibriLib::window::Window::GetPosition() const
{
	return window::GetWindowPos();
}

UI::buttons::ButtonsIDController *KolibriLib::window::Window::GetButtonIDController() const
{
	return const_cast<UI::buttons::ButtonsIDController *>(&_buttonsController);
}

void KolibriLib::window::Window::SetButtonIDController(const UI::buttons::ButtonsIDController *)
{
}

void KolibriLib::window::Window::AddElementNoCopy(UIElement *element)
{
	logger << microlog::LogLevel::Debug << "Add element" << std::endl;

	if (element->GetParent() == nullptr)
	{
		element->WindowAsParent(this);
	}

	element->SetButtonIDController(&_buttonsController);

	_Elements.push_back(std::shared_ptr<UIElement>(element));
}

void KolibriLib::window::Window::Update() const
{
	if (!_updated)
	{
		const Thread::ThreadInfo info = Thread::GetThreadInfo();

		_size = info.WindowSize;
		_coord = info.WindowCoord;

		_updated = true;
	}
}

OS::Event KolibriLib::window::Window::GetLastEvent() const
{
	return _lastEvent;
}