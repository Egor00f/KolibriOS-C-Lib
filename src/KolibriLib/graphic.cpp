#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/img.hpp>
#include <cstring>

using namespace KolibriLib;
using namespace graphic;

void graphic::DrawPoint(const Coord &position, const unsigned &size, const Colors::Color &color, bool fill)
{
	if (!(size < 4)) // если круг такого размера то будет ли он закрашен не будет даже видно
		graphic::DrawCircle(position, size, color);
	else
		graphic::DrawCircleFill(position, size, color);
}

void graphic::DrawCircle(const Coord &coord, unsigned Radius, const Colors::Color &color)
{
	const int size = static_cast<int>(Radius * 2U);

	buf2d::buffer src({size, size}, buf2d::BPP::RGBA);
	buf2d::buffer dst({size, size}, buf2d::BPP::RGB);

	src.top = static_cast<std::uint16_t>(coord.y);
	src.left = static_cast<std::uint16_t>(coord.x);
	dst.top = static_cast<std::uint16_t>(coord.y);
	dst.left = static_cast<std::uint16_t>(coord.x);

	std::memset(src.buf_pointer, -1, size*size);

	buf2d::ApplyTransparency(&dst, &src);

	buf2d::DrawCircle(&dst, {Radius, Radius}, Radius, color);	

	buf2d::Draw(&dst);
}

void graphic::DrawCircleFill(const Coord &coord, const unsigned &Radius, const Colors::Color &color)
{
	// хоспаде какой всратый алгоритм

	graphic::DrawCircle(coord, Radius, color);

	auto c = lround(Radius * sin(90 + 45));
	Coord n(coord.x + lround(Radius * cos(90 + 45)), coord.y + c);

	DrawRectangleFill(
		n,
		{(coord.x - n.x) * 2,
		 c * 2},
		color);

	for (unsigned i = Radius; i > static_cast<unsigned>(Radius - (coord.x - n.x)); i--) // Дозакрашивание пробелов между квадратом и границами круга
		graphic::DrawCircle(coord, i, color);
		
}