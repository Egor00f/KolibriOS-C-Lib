#include <C_Layer/buf2d.hpp>
#include <kolibriLib/debug.hpp>

buf2d::Loader buf2d::buf2dLoader;

buf2d::Loader::Loader()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Init Buf2d:";

	int err = kolibri_buf2d_init();

	if (err == -1)
	{
		KolibriLib::logger << microlog::LogLevel::Fatal << "Error" << std::endl;
		throw err;
	}
	else
	{
		KolibriLib::logger << "OK";
	}

	KolibriLib::logger << std::endl;
}

/*
	buffer
*/

buf2d::buffer::buffer()
{
	Create(this, {32, 32}, 0xFFFFFF, BPP::RGB);
}

buf2d::buffer::buffer(BPP bpp)
{
	Create(this, {32, 32}, 0xFFFFFF, bpp);
}

buf2d::buffer::buffer(const KolibriLib::Size &size, BPP bpp)
{
	Create(this, size, 0xFFFFFF, bpp);
}

buf2d::buffer::buffer(const KolibriLib::Size &size, KolibriLib::Colors::rgb color)
{
	Create(this, size, color, BPP::RGB);
}

buf2d::buffer::buffer(const KolibriLib::Size &size, KolibriLib::Colors::Color color)
{
	Create(this, size, color, BPP::RGBA);
}

buf2d::buffer::buffer(const buf2d_struct *buff)
{
	buf2d_copy(buff, this);
}

buf2d::buffer::~buffer()
{
	Delete(this);
}

buf2d::buffer::operator buf2d_struct *()
{
	return this;
}

void buf2d::buffer::swap(buffer &buff)
{
	unsigned int *_buff_p = buf_pointer;

	unsigned int _buff = bgcolor;

	buf_pointer = buff.buf_pointer;
	buff.buf_pointer = _buff_p;

	bgcolor = buff.bgcolor;
	buff.bgcolor = _buff;

	_buff = color_bit;
	color_bit = buff.color_bit;
	buff.color_bit = static_cast<uint8_t>(_buff);

	_buff = width;
	width = buff.width;
	buff.width = _buff;

	_buff = height;
	height = buff.height;
	buff.height = _buff;

	_buff = left;
	left = buff.left;
	buff.left = static_cast<uint16_t>(_buff);

	_buff = top;
	top = buff.top;
	buff.top = static_cast<uint16_t>(_buff);
}
