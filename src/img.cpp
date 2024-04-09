#include <kolibriLib/img.hpp>


using namespace KolibriLib;
using namespace UI;
using namespace Images;

KolibriLib::UI::Images::img::img()
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor(empety)\n");
	#endif

	_buff = buf2d_create(0,0, 32, 32, 0xFFFFFF, 24);
}

KolibriLib::UI::Images::img::img(const Colors::Color *color, const Size &size)
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor\n");
	#endif

	_buff = buf2d_create(0, 0, size.x, size.y, 0xFFFFFF, 24);
	SetColorMap(color, size);
}

KolibriLib::UI::Images::img::img(const Colors::Color &color, const Size &size)
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor\n");
	#endif

	_buff = buf2d_create(0, 0, size.x, size.y, color.val, 24);
}

KolibriLib::UI::Images::img::img(const img & copy)
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor(copy)\n");
	#endif

	_buff = buf2d_create(copy._buff->left, copy._buff->top, copy._buff->width, copy._buff->height, copy._buff->bgcolor, copy._buff->color_bit);
	memcpy(_buff->buf_pointer, copy._buff->buf_pointer, copy._buff->width * copy._buff->height);
}

KolibriLib::UI::Images::img::img(const rgb_t *color, const Size &size)
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor\n");
	#endif

	_buff = buf2d_create(0, 0, size.x, size.y, 0xFFFFFF, 24);
	SetRGBMap(color, size);
}

KolibriLib::UI::Images::img::img(const filesystem::Path &ImageFile)
{
	#ifdef DEBUG
	_ksys_debug_puts("img consturctor\n");
	#endif

	Image_t *buff = new Image_t;
	buff = LoadImageFromFile(ImageFile.GetChars());

	buf2d_create_f_img(_buff, buff->Data);

	img_destroy(buff);
}

UI::Images::img::~img()
{
	#ifdef DEBUG
	_ksys_debug_puts("img desturctor\n");
	#endif
	
	buf2d_delete(_buff);
}

void UI::Images::img::Draw(const Coord &coord, const Size &size) const
{
	#ifdef DEBUG
	_ksys_debug_puts("Draw");
	#endif

	if(size != -1)
	{
		buf2d_struct *buff = buf2d_copy(_buff);

		buff->left = coord.x;
		buff->top = coord.y;

		buf2d_resize(buff, size.x, size.y, ResizeParams::BUF2D_Resize_ChangeSize);

		buf2d_draw(buff);
	}
	else
	{
		buf2d_draw(_buff);
	}
	
}

void UI::Images::img::SetImg(const buf2d_struct *img)
{
	_buff = buf2d_copy(img);
}

void KolibriLib::UI::Images::img::SetPixel(const Colors::Color &color, unsigned x, unsigned y)
{
	buf2d_set_pixel(_buff, x, y, color.val);
}

void KolibriLib::UI::Images::img::SetPixel(const Colors::Color &color, const Coord &coord)
{
	buf2d_set_pixel(_buff, coord.x, coord.y, color.val);
}

Colors::Color KolibriLib::UI::Images::img::GetPixel(unsigned x, unsigned y) const
{
	return buf2d_get_pixel(_buff, x, y);
}

Colors::Color KolibriLib::UI::Images::img::GetPixel(const Coord &coord) const
{
	return buf2d_get_pixel(_buff, coord.x, coord.y);
}

rgb_t *KolibriLib::UI::Images::img::GetRGBMap() const
{
	if(_buff->color_bit == 24)
	{
		return (rgb_t*)_buff->buf_pointer;
	}

}

buf2d_struct *KolibriLib::UI::Images::img::GetBuff() const
{
	return _buff;
}

void KolibriLib::UI::Images::img::FillColor(const Colors::Color &color)
{
	buf2d_clear(_buff, color.val);
}

KolibriLib::UI::Images::img& KolibriLib::UI::Images::img::operator = (const Images::img& im)
{
	_buff = buf2d_copy(im._buff);
	
	return *this;
}

bool KolibriLib::UI::Images::img::operator!=(const img &im) const
{
	if (_buff->height != im._buff->height || _buff->width != im._buff->width) // если размеры изображний не совпадают то значит они точно не одинаковы
	{
		return true;
	}
	for (unsigned i = 0; i < _buff->width * _buff->height; i++)
	{
		if (_buff->buf_pointer[i] != im._buff->buf_pointer[i])
		{
			return true;
		}
	}
	return false;
}

Colors::Color *KolibriLib::UI::Images::img::GetColorsMap() const
{
	return (Colors::Color*)_buff->buf_pointer;
}

Size img::GetSize() const
{
	return Size(_buff->width, _buff->height);
}

void img::LoadImage(const filesystem::Path &Path)
{
	Image_t *buff = LoadImageFromFile(Path.GetChars());

	if (buff->Type != IMAGE_BPP24)
	{
		buff = img_convert(buff, NULL, IMAGE_BPP24, 0, 0); // Convert image to format BPP24
		if (!buff)
		{
			_ksys_debug_puts("Convert error\n");
		}
	}
	
	img_to_rgb2(buff, _buff->buf_pointer);
}

void KolibriLib::UI::Images::img::SetRGBMap(const rgb_t *rgbmap, const Size &size)
{
	if (_buff->width != size.x && _buff->height != size.y) // Если рамеры буфера не соответсвуют размерам rgbmap
	{													   // Изменяится размеры
		buf2d_resize(_buff, size.x, size.y, ResizeParams::BUF2D_Resize_ChangeSize);
	}

	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			buf2d_set_pixel(_buff, j, i, Colors::RGBtoINT(rgbmap[(i * size.x) + j]));
		}
	}
}

void KolibriLib::UI::Images::img::SetColorMap(const Colors::Color * rgbmap, const Size & size)
{
	if(_buff->width != size.x && _buff->height != size.y)	// Если рамеры буфера не соответсвуют размерам rgbmap
	{														// Изменяится размеры
		buf2d_resize(_buff, size.x, size.y, ResizeParams::BUF2D_Resize_ChangeSize);
	}

	for(int i = 0; i < size.y; i++)
	{
		for(int j = 0; j < size.x; j++)
		{
			buf2d_set_pixel(_buff, j, i, rgbmap[(i * size.x) + j].val);
		}
	}
}