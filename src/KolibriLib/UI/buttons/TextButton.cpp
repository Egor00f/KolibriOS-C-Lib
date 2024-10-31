#include <kolibriLib/UI/buttons/TextButton.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

buttons::TextButton::TextButton(const UDim &coord, const UDim &size, unsigned Margin, const Colors::Color &ButtonColor)
	:	Button	(coord, size, Margin, ButtonColor),
		Txt("Button")
{
	logger << microlog::LogLevel::Debug << "TextButton constructor" << std::endl;
}

KolibriLib::UI::buttons::TextButton::TextButton(const Txt &text, const UDim &coord, const UDim &size, unsigned Margin, const Colors::Color &ButtonColor)
	:	Button	(coord, size, Margin, ButtonColor),
		Txt(text)
{
	logger << microlog::LogLevel::Debug << "TextButton constructor" << std::endl;

	SetMargin(Margin);

	_MainColor	= ButtonColor;
}

KolibriLib::UI::buttons::TextButton::TextButton(const UDim &coord, const UDim &size, const std::string &text)
	:	Button(coord, size),
		Txt(text)
{
	logger << microlog::LogLevel::Debug << "TextButton constructor(copy)" << std::endl;
}

bool KolibriLib::UI::buttons::TextButton::operator==(const TextButton &element) const
{
	return static_cast<Button>(*this) == static_cast<Button>(element)	&&
	       static_cast<Txt>(*this) == static_cast<Txt>(element);
}

bool KolibriLib::UI::buttons::TextButton::operator!=(const TextButton &element) const
{
	return static_cast<Button>(*this) != static_cast<Button>(element)	||
	       static_cast<Txt>(*this) != static_cast<Txt>(element);
}

bool buttons::TextButton::OnButtonEvent(ButtonID PressedButtonID)
{
	_status = (PressedButtonID == _id); // Если id нажатой кнопки совпадает к id этой кнопки
	return _status;
}

void buttons::TextButton::Render() const
{
	if(Visible)
	{
		logger << microlog::LogLevel::Debug << "Render TextButton" << std::endl;

		if (IsActive())
		{
			Button::Render();
			Txt::Print(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
		}
		else
		{
			logger << microlog::LogLevel::Warning << ": Button Is not Active";
		}
	}
}

buttons::ButtonsIDController *KolibriLib::UI::buttons::TextButton::GetButtonIDController() const
{
	if (_ButtonsIDController != nullptr)
		return _ButtonsIDController;
	else if (std::shared_ptr<GuiObject> s_ptr = Parent.lock())
		return s_ptr->GetButtonIDController();
	else
		return nullptr;
}

void KolibriLib::UI::buttons::TextButton::SetButtonIDController(const buttons::ButtonsIDController *buttonsIDController)
{
	_ButtonsIDController = const_cast<buttons::ButtonsIDController *>(buttonsIDController);
}

void KolibriLib::UI::buttons::TextButton::swap(TextButton &a)
{
	TextButton buff(*this);

	*this = a;
	a = buff;
}