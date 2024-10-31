#include <kolibriLib/img.hpp>
#include <kolibriLib/debug.hpp>
#include <assert.h>

using namespace KolibriLib;
using namespace UI;
using namespace Images;

/*
	Constructors
*/

KolibriLib::UI::Images::img::img(img::BPP bpp)
	: _buff(new buf2d::buffer(bpp))
{
}

KolibriLib::UI::Images::img::img(const img &val)
	: _buff(new buf2d::buffer(val._buff.get()))
{
}

KolibriLib::UI::Images::img::img(const Size &size, BPP bpp)
	: _buff(new buf2d::buffer(size, bpp))
{
}

KolibriLib::UI::Images::img::img(const Colors::rgb &color, const Size &size)
	: _buff(new buf2d::buffer(size, color))
{
}

KolibriLib::UI::Images::img::img(const Colors::Color &color, const Size &size)
	: _buff(new buf2d::buffer(size, color))
{
}

/*
	Functions
*/

void KolibriLib::UI::Images::img::Draw() const
{
	logger << microlog::LogLevel::Debug << "Draw img" << std::endl;

	if (static_cast<BPP>(_buff->color_bit) == BPP::RGBA)
	{
		buf2d::buffer image(GetSize(), BPP::RGB);
		buf2d::ApplyTransparency(_buff.get(), image);
		buf2d::Draw(&image);
	}
	else
	{
		buf2d::Draw(_buff.get());
	}
}

void KolibriLib::UI::Images::img::Draw(const Coord &coord) const
{
	_buff.get()->left = coord.x;
	_buff.get()->top = coord.y;

	Draw();
}

void KolibriLib::UI::Images::img::Draw(const Coord &coord, const Size &size) const
{
	if (_buff->width == size.x && _buff->height == size.y)
	{
		Draw(coord);
	}
	else
	{
		img image(*this);
		image.Resize(size, true);
		image.Draw();
	}
}

void KolibriLib::UI::Images::img::SetPixel(const Coord &coord, const Colors::Color &color)
{
	buf2d::SetPixel(_buff.get(), coord, color);
}

Colors::Color KolibriLib::UI::Images::img::GetPixel(const Coord &coord) const
{
	return buf2d::GetPixel(_buff.get(), coord);
}

void KolibriLib::UI::Images::img::Rotate(RotateEnum val)
{
	if (val != RotateEnum::rotate_270)
		buf2d_rotate(_buff.get(), static_cast<unsigned int>(val));
	else
	{
		buf2d_rotate(_buff.get(), 180);
		buf2d_rotate(_buff.get(), 90);
	}
}

Size KolibriLib::UI::Images::img::GetSize() const
{
	return Size(static_cast<int>(_buff->width), static_cast<int>(_buff->height));
}

img::BPP KolibriLib::UI::Images::img::GetBPP() const
{
	return static_cast<BPP>(_buff->color_bit);
}

void KolibriLib::UI::Images::img::Resize(const Size &NewSize, bool resize)
{
	buf2d::Resize(_buff.get(), NewSize, resize);
}

void KolibriLib::UI::Images::img::DrawRectangle(const Coord &coord, const Size &size, Colors::rgb color)
{
	buf2d::DrawRect(_buff.get(), coord, size, color);
}

void KolibriLib::UI::Images::img::DrawFilledRectangle(const Coord &coord, const Size &size, Colors::rgb color)
{
	buf2d::DrawFilledRect(_buff.get(), coord, size, color);
}

void KolibriLib::UI::Images::img::CurveBezier(Coord points[3], Colors::Color color)
{
	buf2d::DrawBezierCurver(_buff.get(), points, color);
}

/*
	Operators
*/

img &KolibriLib::UI::Images::img::operator=(const img &i)
{
	_buff = i._buff;

	return *this;
}

KolibriLib::UI::Images::img::operator std::shared_ptr<buf2d_struct>() const
{
	return _buff;
}