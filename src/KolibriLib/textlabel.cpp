#include <kolibriLib/UI/text/textlabel.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace text;

/*
	Constructors
*/

TextLabel::TextLabel(const UDim &coord, const UDim &size, const std::string &text, const Size &CharSize, bool TextScale, Colors::Color TextColor, unsigned Margin)
	: Txt(text, TextColor),
	  UIElement(coord, size, Globals::SystemColors.work_area, Margin)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "TextLabel constructor" << std::endl;
#endif

	SetTextSize(CharSize);
	// SetFont(Fonts::Font(Fonts::DefaultFont.font_file, FontSize));
}

KolibriLib::UI::text::TextLabel::TextLabel(const UDim &coord, const UDim &size, const Txt &text)
	: Txt(text),
	  UIElement(coord, size, Globals::SystemColors.work_area)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "TextLabel constructor" << std::endl;
#endif
}

TextLabel::TextLabel(const TextLabel &copy)
	: Txt(copy),
	  UIElement(copy)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "TextLabel constructor(copy)" << std::endl;
#endif
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