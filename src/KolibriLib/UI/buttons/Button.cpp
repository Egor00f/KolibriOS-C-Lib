#include <kolibriLib/UI/buttons/button.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

/*
	Constructors
*/

Button::Button()
	: UIElement()
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Button constructor" << std::endl;
#endif
}

Button::Button(UDim coord, UDim size, unsigned Margin, Colors::Color backGroundColor)
	: UIElement(coord, size, backGroundColor, Margin)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Button constructor" << std::endl;
#endif
}

KolibriLib::UI::buttons::Button::Button(const Button &button)
	: UIElement(button),
	  BaseButton(button)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Button constructor(copy)" << std::endl;
#endif
}

/*
	Functions
*/

void KolibriLib::UI::buttons::Button::Render() const
{
	BaseButton::Define(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
}

bool buttons::Button::OnButtonEvent(ButtonID PressedButtonID)
{
	_status = PressedButtonID == _id; // Если id нажатой кнопки совпадает к id этой кнопки
	return _status;
}

buttons::ButtonsIDController *KolibriLib::UI::buttons::Button::GetButtonIDController() const
{
	if (_ButtonsIDController != nullptr)
		return _ButtonsIDController;
	else if (std::shared_ptr<GuiObject> s_ptr = Parent.lock())
		return s_ptr->GetButtonIDController();
	else
		return nullptr;
}

void KolibriLib::UI::buttons::Button::SetButtonIDController(const buttons::ButtonsIDController *buttonsIDController)
{
	_ButtonsIDController = const_cast<buttons::ButtonsIDController *>(buttonsIDController);
}

void KolibriLib::UI::buttons::Button::swap(Button &a)
{
	Button buff(*this);

	*this = a;
	a = buff;
}

/*
	Operators
*/

bool KolibriLib::UI::buttons::Button::operator==(const Button &val) const
{
	return static_cast<UIElement>(*this) == static_cast<UIElement>(val) &&
		   static_cast<Button>(*this) == static_cast<Button>(val);
}

bool KolibriLib::UI::buttons::Button::operator!=(const Button &val) const
{
	return static_cast<UIElement>(*this) != static_cast<UIElement>(val) &&
		   static_cast<Button>(*this) != static_cast<Button>(val);
}