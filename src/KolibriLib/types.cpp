#include <kolibriLib/types.hpp>
#include <cmath>

using namespace KolibriLib;

KolibriLib::point::point()
	: x(0),
	  y(0)
{
}

KolibriLib::point::point(int X, int Y)
	: x(X),
	  y(Y)
{
}

KolibriLib::point::point(const point &p)
	: x(p.x),
	  y(p.y)
{
}

KolibriLib::point::point(const ksys_pos_t &pos)
	: x(pos.x),
	  y(pos.y)
{
}

KolibriLib::point::point(int Number)
	: x(Number),
	  y(Number)
{
}

ksys_pos_t KolibriLib::point::GetKsysPost() const
{
	ksys_pos_t buff;
	buff.x = x;
	buff.y = y;
	return buff;
}

point &KolibriLib::point::operator+=(const point &a)
{
	x += a.x;
	y += a.y;
	return *this;
}

point &KolibriLib::point::operator-=(const point &a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}

point KolibriLib::point::operator+(const point &a) const
{
	return point(x + a.x, y + a.y);
}

point KolibriLib::point::operator-(const point &a) const
{
	return point(x - a.y, y - a.y);
}

point &KolibriLib::point::operator=(const point &p)
{
	x = p.x;
	y = p.y;
	return *this;
}

point &point::operator+=(const int &p)
{
	x += p;
	y += p;
	return *this;
}

point &point::operator-=(const int &p)
{
	x -= p;
	y -= p;
	return *this;
}

point &point::operator*=(const int &p)
{
	x *= p;
	y *= p;
	return *this;
}

point &point::operator/=(const int &p)
{
	x /= p;
	y /= p;
	return *this;
}

bool point::operator==(const point &a) const
{
	return x == a.x && y == a.y;
}

bool point::operator!=(const point &a) const
{
	return x != a.x || y != a.y;
}

KolibriLib::point::operator ksys_pos_t() const
{
	return GetKsysPost();
}

bool KolibriLib::point::operator<(const point &obj) const
{
	return (x < obj.x) || (y < obj.y);
}

bool KolibriLib::point::operator>(const point &obj) const
{
	return (x > obj.x) || (y > obj.y);
}

/*
	UDim
*/

KolibriLib::UDim::Axis::Axis(float scale, int offset)
	: Scale(scale),
	  Offset(offset)
{
}

KolibriLib::UDim::UDim(float XScale, int XOffset, float YScale, int YOffset)
	: X(XScale, XOffset),
	  Y(YScale, YOffset)
{
}

KolibriLib::UDim::UDim(int x, int y)
	: X(0, x),
	  Y(0, y)
{
}

KolibriLib::UDim::UDim(float x, float y)
	: X(x, 0),
	  Y(y, 0)
{
}

KolibriLib::UDim::UDim(const Axis &x, const Axis &y)
	: X(x),
	  Y(y)
{
}

KolibriLib::UDim::UDim(const point &p)
	: X(0, p.x),
	  Y(0, p.y)
{
}

bool UDim::Axis::operator==(const UDim::Axis &axis) const
{
	return Offset == axis.Offset && std::fabs(Scale - axis.Scale) < 0.001f;
}

bool UDim::Axis::operator!=(const UDim::Axis &axis) const
{
	return Offset != axis.Offset || std::fabs(Scale - axis.Scale) < 0.001f;
}

point KolibriLib::UDim::GetAbsolute(const point &Parent) const
{
	return point(lround(X.Scale * static_cast<float>(Parent.x)) + X.Offset,
				 lround(Y.Scale * static_cast<float>(Parent.y)) + Y.Offset);
}

bool KolibriLib::UDim::operator==(const UDim &obj) const
{
	return X == obj.X && Y == obj.Y;
}

bool KolibriLib::UDim::operator!=(const UDim &obj) const
{
	return X != obj.X || Y != obj.Y;
}