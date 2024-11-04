#include <kolibriLib/UI/buttons/TextButton.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

/*
	Constructors
*/

buttons::TextButton::TextButton(const UDim &coord, const UDim &size, unsigned Margin, Colors::Color ButtonColor)
	: Button(coord, size, Margin, ButtonColor),
	  Txt("Button")
{
	logger << microlog::LogLevel::Debug << "TextButton constructor" << std::endl;
}

KolibriLib::UI::buttons::TextButton::TextButton(const Txt &text, const UDim &coord, const UDim &size, unsigned Margin, Colors::Color ButtonColor)
	: Button(coord, size, Margin, ButtonColor),
	  Txt(text)
{
	logger << microlog::LogLevel::Debug << "TextButton constructor" << std::endl;
}

KolibriLib::UI::buttons::TextButton::TextButton(const UDim &coord, const UDim &size, const std::string &text)
	: Button(coord, size),
	  Txt(text)
{
	logger << microlog::LogLevel::Debug << "TextButton constructor" << std::endl;
}

KolibriLib::UI::buttons::TextButton::TextButton(const TextButton &textButton)
	: Button(textButton),
	  Txt(textButton)
{
	logger << microlog::LogLevel::Debug << "TextButton constructor(copy)" << std::endl;
}

/*
	Functions
*/

void buttons::TextButton::Render() const
{
	if (Visible)
	{
		logger << microlog::LogLevel::Debug << "Render TextButton" << std::endl;

		if (IsActive())
		{
			Button::Render();
			Txt::Print(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
		}
		else
		{
			logger << microlog::LogLevel::Warning << ": Button is not Active";
		}
	}
}

void KolibriLib::UI::buttons::TextButton::swap(TextButton &a)
{
	TextButton buff(*this);

	*this = a;
	a = buff;
}

/*
	Operators
*/

bool KolibriLib::UI::buttons::TextButton::operator==(const TextButton &element) const
{
	return static_cast<Button>(*this) == static_cast<Button>(element) &&
		   static_cast<Txt>(*this) == static_cast<Txt>(element);
}

bool KolibriLib::UI::buttons::TextButton::operator!=(const TextButton &element) const
{
	return static_cast<Button>(*this) != static_cast<Button>(element) ||
		   static_cast<Txt>(*this) != static_cast<Txt>(element);
}
