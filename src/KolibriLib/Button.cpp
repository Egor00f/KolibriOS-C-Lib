#include <kolibriLib/UI/buttons/button.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

Button::Button()
	:	UIElement(), 
		BaseButton()
{
	PrintDebug("Button constructor\n");
}

Button::Button(UDim coord, UDim size, unsigned Margin, Colors::Color backgrouncolor)
	:	UIElement(coord, size, backgrouncolor, Margin),
		BaseButton()
{
	PrintDebug("Button constructor\n");
}

void KolibriLib::UI::buttons::Button::Render() const
{
	BaseButton::Define(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
}

bool buttons::Button::OnButtonEvent(ButtonID PressedButtonID) 
{
	_status = PressedButtonID == _id; // Если id нажатой кнопки совпадает к id этой кнопки
	return _status;
}

void KolibriLib::UI::buttons::Button::swap(Button &a)
{
	Button buff(*this);

	*this = a;
	a = buff;
}

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

void KolibriLib::PrintDebug(const UI::buttons::Button &out)
{
	PrintDebug(static_cast<UIElement>(out));
	PrintDebug(static_cast<BaseButton>(out));
}