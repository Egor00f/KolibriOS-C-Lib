#include <kolibriLib/UI/image.hpp>

#include <kolibriLib/globals.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace Images;

Image::Image(const UDim &coord, const UDim &size)
	: UIElement(coord, size),
	  img(Colors::Color(Globals::SystemColors.work_graph), size.GetAbsolute({0, 0}))
{
	logger << microlog::LogLevel::Debug << "KolibriLib::UI::Images::Image Constructor" << std::endl;
}

KolibriLib::UI::Images::Image::Image(const Image &copy)
	: UIElement(copy),
	  img(copy)
{
	logger << microlog::LogLevel::Debug << "KolibriLib::UI::Images::Image Constructor(copy)" << std::endl;
}

void KolibriLib::UI::Images::Image::Render() const
{
	Draw(GetAbsoluteCoord(), GetAbsoluteSize());
}

void KolibriLib::UI::Images::Image::swap(Image &e)
{
	Image buff(*this);

	*this = e;
	e = buff;
}
