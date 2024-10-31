#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/img.hpp>

using namespace KolibriLib;
using namespace graphic;

void graphic::DrawPoint(const Coord &position, const unsigned &size, const Colors::Color &color, bool fill)
{
	fill = !(size < 4); // если круг такого размера то будет ли он закрашен не будет даже видно

	if (!fill)
	{
		graphic::DrawCircle(position, size, color);
	}
	else
	{
		graphic::DrawCircleFill(position, size, color);
	}
}

void graphic::DrawCircle(const Coord &coord, unsigned Radius, const Colors::Color &color)
{
	buf2d::buffer src({static_cast<int>(Radius * 2U), static_cast<int>(Radius * 2U)}, buf2d::BPP::RGBA);
	buf2d::buffer dst({static_cast<int>(Radius * 2U), static_cast<int>(Radius * 2U)}, buf2d::BPP::RGB);

	buf2d::ApplyTransparency(&dst, &src);

	src.top = coord.y;
	src.left = coord.x;
	dst.top = coord.y;
	dst.left = coord.x;

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

	for (unsigned i = Radius; i > static_cast<unsigned>(Radius - (coord.x - n.x)); i--) // Дозакрашивание пробелов между квадратом и границами груга
	{
		graphic::DrawCircle(coord, i, color);
	}
}