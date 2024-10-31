#include <kolibriLib/UI/text/textlabel.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace text;

/*
	Constructors
*/

TextLabel::TextLabel(const UDim &coord, const UDim &size, const std::string &text, const Size &CharSize, bool TextScale, const Colors::Color &TextColor, const unsigned &Margin)
	: Txt(text, TextColor),
	  UIElement(coord, size, Globals::SystemColors.work_area, Margin)
{
	logger << microlog::LogLevel::Debug << "TextLabel Constructor" << std::endl;

	SetTextSize(CharSize);
	// SetFont(Fonts::Font(Fonts::DefaultFont.font_file, FontSize));
}

KolibriLib::UI::text::TextLabel::TextLabel(const UDim &coord, const UDim &size, const Txt &text)
	: Txt(text),
	  UIElement(coord, size, Globals::SystemColors.work_area)
{
	logger << microlog::LogLevel::Debug << "TextLabel Constructor" << std::endl;
}

TextLabel::TextLabel(const TextLabel &copy)
	: Txt(copy),
	  UIElement(copy)
{
	logger << microlog::LogLevel::Debug << "TextLabel constructor(copy)" << std::endl;
}

/*
	Functions
*/

void text::TextLabel::Render() const
{
	if (Visible)
		Print(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
}

/*
	Operators
*/

bool KolibriLib::UI::text::TextLabel::operator==(const KolibriLib::UI::text::TextLabel &a) const
{
	return static_cast<UIElement>(*this) == static_cast<UIElement>(a) &&
		   static_cast<Txt>(*this) == static_cast<Txt>(a);
}

bool KolibriLib::UI::text::TextLabel::operator!=(const KolibriLib::UI::text::TextLabel &a) const
{
	return static_cast<UIElement>(*this) != static_cast<UIElement>(a) ||
		   static_cast<Txt>(*this) != static_cast<Txt>(a);
}

void KolibriLib::UI::text::TextLabel::swap(TextLabel &a)
{
	TextLabel buff(*this);

	*this = a;
	a = buff;
}