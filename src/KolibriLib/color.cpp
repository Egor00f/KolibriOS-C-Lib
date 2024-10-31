#include <kolibriLib/color.hpp>

#include <cstdio>

using namespace KolibriLib;
using namespace Colors;

/*
	rgb
*/

KolibriLib::Colors::rgb::rgb(rgb_t val)
{
	red = val.red;
	green = val.green;
	blue = val.blue;
}

KolibriLib::Colors::rgb::rgb(ksys_color_t val)
{
	*this = rgb(UINT32toRGB(val));
}

KolibriLib::Colors::rgb::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

KolibriLib::Colors::rgb::operator ksys_color_t() const
{
	return RGBtoINT(*this);
}

std::uint32_t KolibriLib::Colors::rgb::BBGGRR() const
{
	return ((blue << 16) | (green << 8) | red);
}

std::uint32_t KolibriLib::Colors::rgb::BBGGRR00() const
{
	return ((blue << 24) | (green << 16) | (red << 8)) & 0xFFFFFF00;
}

std::uint32_t KolibriLib::Colors::rgb::RRGGBB() const
{
	return ((red << 16) | (green << 8) | blue);
}

std::uint32_t KolibriLib::Colors::rgb::RRGGBB00() const
{
	return ((red << 24) | (green << 16) | (blue << 8)) & 0xFFFFFF00;
}

std::uint32_t KolibriLib::Colors::rgb::ZeroRRGGBB() const
{
	return ((red << 16) | (green << 8) | blue) | 0;
}

/*
	Color
*/

KolibriLib::Colors::Color::Color(const ksys_color_t &a)
	: val(a)
{
}

KolibriLib::Colors::Color::Color(const rgb_t &color)
	: _a(0xFF),
	  red(color.red),
	  green(color.green),
	  blue(color.blue)
{
}

KolibriLib::Colors::Color::Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	: red(R),
	  green(G),
	  blue(B),
	  _a(A)
{
}

rgb_t KolibriLib::Colors::Color::GetRGB() const
{
	rgb_t ret;
	ret.red = red;
	ret.green = green;
	ret.blue = blue;
	return ret;
}

Color &KolibriLib::Colors::Color::operator=(const uint32_t &color)
{
	val = color;
	return *this;
}

KolibriLib::Colors::Color &KolibriLib::Colors::Color::operator+(const KolibriLib::Colors::Color &a)
{
	_a /= 2;
	_a += a._a / 2;

	red /= 2;
	red += (a.red / 2);

	green /= 2;
	green += (a.green / 2);

	blue /= 2;
	blue += (a.blue / 2);

	return *this;
}

bool KolibriLib::Colors::Color::operator==(const Color &a) const
{
	return val == a.val;
}

bool KolibriLib::Colors::Color::operator!=(const Color &a) const
{
	return val != a.val;
}

ksys_color_t KolibriLib::Colors::RGBtoINT(const rgb_t &color)
{
	return ((color.red << 16) + (color.green << 8) + color.blue) & 0x00FFFFFF;
}

KolibriLib::Colors::Color KolibriLib::Colors::BlendColors(const KolibriLib::Colors::Color &a, const KolibriLib::Colors::Color &b, float k)
{
	if (k > 1.0) // Коэфицент более 1 не имеет смысла
		k = 1.0;

	// буффер
	KolibriLib::Colors::Color buff;

	buff._a = static_cast<uint8_t>((a._a * k) + (b._a * (1.0f - k)));
	buff.red = static_cast<uint8_t>((a.red * k) + (b.red * (1.0f - k)));
	buff.green = static_cast<uint8_t>((a.green * k) + (b.green * (1.0f - k)));
	buff.blue = static_cast<uint8_t>((a.blue * k) + (b.blue * (1.0f - k)));

	return buff;
}

rgb_t KolibriLib::Colors::UINT32toRGB(const uint32_t &color)
{
	rgb_t ret;

	ret.blue = ((Color)color).blue;
	ret.red = ((Color)color).red;
	ret.green = ((Color)color).green;

	return ret;
}

KolibriLib::Colors::Color::operator rgb_t() const
{
	return GetRGB();
}

KolibriLib::Colors::Color::operator ksys_color_t() const
{
	return val;
}

KolibriLib::Colors::Color::operator rgb() const
{
	return rgb(red, green, blue);
}

KolibriLib::Colors::ColorsTable::ColorsTable(Color frameArea, Color grabBar, Color grabBarButton, Color grabText, Color workArea, Color workButton, Color workButtonText, Color workText, Color workGraph)
{
	frame_area = frameArea;
	grab_bar = grabBar;
	grab_bar_button = grabBarButton;
	grab_text = grabText;
	work_area = workArea;
	work_button = workButton;
	work_button_text = workButtonText;
	work_text = workText;
	work_graph = workGraph;
}
