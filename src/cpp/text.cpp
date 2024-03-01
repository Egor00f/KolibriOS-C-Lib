#include <kolibriLib/UI/text.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace text;

text::TextLabel::TextLabel(const Coord &coord, const Size &size, const std::string &text, const unsigned &FontSize, bool TextScale, const Colors::Color &TextColor, const unsigned &Margin) : UIElement(coord, size, TextColor, Margin)
{
#if DEBUG == true
	_ksys_debug_puts("TextLabel Constructor \n");
#endif;
	Add(text);
	_TextScale = TextScale;
}

KolibriLib::UI::text::TextLabel::TextLabel(const Coord &coord, const Size &size, const std::string &text, const Fonts::Font &Font, const Colors::Color &TextColor, const Colors::Color &BackgroundColor, bool TextScale, const unsigned &Margin)
{
	Add(text);
	_TextScale = TextScale;
	SetFont(Font);
}

text::TextLabel::~TextLabel()
{
}

void text::TextLabel::Render() const
{
	unsigned* FontSize = new unsigned;
	*FontSize = 0;
	if (_TextScale) // Если текст нужно подстраивать размер, то
	{			   // Постраиваем
		*FontSize = _size.x / lenghtPX();
		if (!_Aligned)
		{
			for (int i = 0; i < _data.size(); i++)
			{
				Fonts::Font buff = _data[i].GetFont();
				_data[i].SetFont(Fonts::Font(buff._Font, *FontSize, buff._Flags));
			}
			_Aligned = true;
		}
	}
	delete FontSize;
	Print({_coord.x + ((int)_size.x / 2), _coord.y + ((int)_size.y / 2)});
}

void text::TextLabel::SetScale(bool scale)
{
	_TextScale = scale;
}


text::TextLabel &KolibriLib::UI::text::TextLabel::operator=(const TextLabel &a)
{
	_coord	  = a._coord;
	_size	   = a._size;
	_MainColor  = a._MainColor;
	_Margin	 = a._Margin;
	_TextScale  = a._TextScale;
	return *this;
}

bool KolibriLib::UI::text::TextLabel::operator==(const TextLabel &a) const
{
	return (_coord == a._coord) && 
			(_size == a._size) && 
			(_MainColor == a._MainColor) && 
			(_Margin == a._Margin) && 
			(_TextScale == a._TextScale);
}

bool KolibriLib::UI::text::TextLabel::operator!=(const TextLabel &a) const
{
	return (_coord == a._coord) ||
		   (_size == a._size) ||
		   (_MainColor == a._MainColor) ||
		   (_Margin == a._Margin) ||
		   (_TextScale == a._TextScale);
}

KolibriLib::UI::text::Char::Char(char c, const Fonts::Font &font, const Colors::Color &TextColor, const Colors::Color &BackgroundColor)
{
	_c			  		= new char(c);
	_font 				= font;
	_TextColor			= new Colors::Color(TextColor);
	_BackgroundColor	= new Colors::Color(BackgroundColor);
	_type				= Type::Text;
}

KolibriLib::UI::text::Char::Char(const Images::img &img, const Fonts::Font& font)
{
	_img	= new Images::img(img);
	_font	= font;
	_type	= Type::Image;
}

KolibriLib::UI::text::Char::~Char()
{
	Char::Free();
}

short KolibriLib::UI::text::Char::GetType() const
{
	return _type;
}

void KolibriLib::UI::text::Char::Set(const char c, const Fonts::Font &font, const Colors::Color &TextColor, const Colors::Color &BackgroundColor)
{
	Char::Free();   //Пофиг какой раньше тип был, просто тупо сносим и делаем всё заново
	_font 			 = font;
	_TextColor 		 = new Colors::Color(TextColor);
	_BackgroundColor = new Colors::Color(BackgroundColor);
	_c 				 = new char(c);
	_type			 = Type::Text;
}

void KolibriLib::UI::text::Char::Set(const Images::Image &img, const Fonts::Font& size)
{
	Char::Free();
	_img  = new Images::Image(img);
	_font = size;
	_type = Type::Image;
}

void KolibriLib::UI::text::Char::SetFlags(unsigned flags)
{
	assert(_type == Type::Text);
	_font._Flags = flags;
}

void KolibriLib::UI::text::Char::SetTextColor(const Colors::Color &NewColor)
{
	assert(_type == Type::Text);
	*_TextColor = NewColor.val;
}

void KolibriLib::UI::text::Char::SetBackgroundColor(const Colors::Color & NewColor)
{
	assert(_type == Type::Text);
	*_BackgroundColor = NewColor.val;
}

void KolibriLib::UI::text::Char::SetFont(const Fonts::Font &newFont) const
{
	assert(_type == Type::Text);
	_font = newFont;
}

char KolibriLib::UI::text::Char::GetChar() const
{
	assert(_type == Type::Text);
	return *_c;
}

const Images::img &KolibriLib::UI::text::Char::GetImg() const
{
	assert(_type == Type::Image);
	return *_img;
}

const Fonts::Font &KolibriLib::UI::text::Char::GetFont() const
{
	assert(_type == Type::None);
	return _font;
}

unsigned KolibriLib::UI::text::Char::GetFlags() const
{
	assert(_type == Type::Text);
	return _font._Flags;
}

void KolibriLib::UI::text::Char::Free()
{
	switch (_type)
	{
	case Type::Image:
		delete _img;
		break;
	case Type::Text:
		delete _c;
		delete _TextColor;
		delete _BackgroundColor;
		break;
	default:
		break;
	}
}

void KolibriLib::UI::text::Char::Print(const UI::Coord &coord) const
{
	switch (_type)
	{
	case Type::Image:
		_img->Render(coord, _font.size);
		break;
	case Type::Text:
		DrawText(std::string(_c), coord, _font.size, _font._Flags, NULL, *_TextColor, *_BackgroundColor);
		break;
	default:
		break;
	}
}

const std::string &KolibriLib::UI::text::Text::GetText() const
{
	std::string result;
	for (int i = 0; i < _data.size(); i++)
	{
		if(_data[i].GetType() == Char::Type::Text)
		{
			result += _data[i].GetChar();
		}
	}
	return result;
}





KolibriLib::UI::text::Text::Text()
{

}

KolibriLib::UI::text::Text::~Text()
{

}

void KolibriLib::UI::text::Text::Add(const Char &c)
{
	_data.push_back(c);
}

void KolibriLib::UI::text::Text::Add(const std::string &txt)
{
	_data.push_back(Char(txt[0]));	//На случай если вектор пустой
	for(int i = 1; i < txt.length(); i++)
	{
		_data.push_back(Char(txt[i]));
	}
}

void KolibriLib::UI::text::Text::Add(const Images::img &img)
{
	_data.push_back(Char(img));
}
void KolibriLib::UI::text::Text::insert(const Char &c, int i)
{
	_data.insert(_data.begin() + i, c);
}

void KolibriLib::UI::text::Text::insert(const std::string &txt, int i)
{
	for(int j = txt.length(); j > 0; j++)
	{
		_data.insert(_data.begin() + i, Char(txt[j], _data[i].GetFont()));
	}
}

void KolibriLib::UI::text::Text::insert(const Images::img &img, int i)
{
	_data.insert(_data.begin() + i, Char(img));
}

void KolibriLib::UI::text::Text::Delete(int i)
{
	_data.erase(_data.begin() + i);
}

void KolibriLib::UI::text::Text::Print(const UI::Coord &coord) const
{
	int buff = 0;
	for(int i = 0; i < _data.size(); i++)
	{
		_data[i].Print({coord.x + buff, coord.y});
		buff += _data[i].GetFont().size.x;
	}
}

void KolibriLib::UI::text::Text::SetFont(const Fonts::Font &Font)
{
	for(int i = 0; i < _data.size(); i++)
	{
		_data[i].SetFont(Font);
	}
}

void KolibriLib::UI::text::Text::SetTextColor(const Colors::Color &Color)
{
	for(int i = 0; i < _data.size(); i++)
	{
		if(_data[i].GetType() == Char::Type::Text)
		{
			_data[i].SetTextColor(Color);
		}
	}
}

void KolibriLib::UI::text::Text::SetBackgroundColor(const Colors::Color &Color)
{
	for(int i = 0; i < _data.size(); i++)
	{
		if (_data[i].GetType() == Char::Type::Text)
		{
			_data[i].SetBackgroundColor(Color);
		}
	}
}

bool KolibriLib::UI::text::Text::operator==(const Text &txt) const
{
	return _data == txt._data;
}

std::size_t KolibriLib::UI::text::Text::length() const
{
	return _data.size();
}

unsigned KolibriLib::UI::text::Text::lenghtPX() const
{
	unsigned l = 0;
	for(unsigned i = 0; i < _data.size(); i++)
	{
		l += _data[i].GetFont().size.x;
	}
	return l;
}

Text &KolibriLib::UI::text::Text::operator=(const Text &txt) 
{
	_data = txt._data;
	return *this;
}

void KolibriLib::UI::text::DrawText(const std::string & text, const Coord & coord, const Size & size, unsigned flags, unsigned margin, const Colors::Color & colorText, const Colors::Color &BackgroundColor)
{
	const unsigned w = ((margin * 2) + size.x);
	const unsigned h = ((margin * 2) + (size.y * text.length()));

	rgb_t *canvas = new rgb_t[w * h];
	
	for(int i = 0; i < w*h; i++)
	{
	  canvas[i].red = BackgroundColor.red;
	  canvas[i].blue = BackgroundColor.blue;
	  canvas[i].green = BackgroundColor.green;
	}

	drawText(canvas, coord.x, coord.y, text.c_str(), text.length(), colorText.val, flags);
	_ksys_draw_bitmap(canvas, coord.x, coord.y, w, h);
	
	delete[] canvas;
}

void KolibriLib::UI::text::DrawText(const std::string & text, const Coord & coord, const Fonts::Font & font, unsigned margin, const Colors::Color & colorText, const Colors::Color & BackgroundColor)
{
	const unsigned w = ((margin * 2) + font.size.x);
	const unsigned h = ((margin * 2) + (font.size.y * text.length()));

	rgb_t *canvas = new rgb_t[w * h];

	for (int i = 0; i < w * h; i++)
	{
		canvas[i].red = BackgroundColor.red;
		canvas[i].blue = BackgroundColor.blue;
		canvas[i].green = BackgroundColor.green;
	}

	drawText(canvas, coord.x, coord.y, text.c_str(), text.length(), colorText.val, font._Flags);
	_ksys_draw_bitmap(canvas, coord.x, coord.y, w, h);

	delete[] canvas;
}

KolibriLib::UI::text::Fonts::Font::Font(const UI::Size& FontFamily, int FontSize, unsigned flags)
{
	_Font = FontFamily;
	_Flags = flags;
	SetFontSize((unsigned)FontSize);
}

void KolibriLib::UI::text::Fonts::Font::SetFontSize(unsigned FontSize)
{
	float buff = FontSize / _Font.x;
	_FontSize = FontSize;
	size = _Font;
	size /= static_cast<unsigned>(buff);	//float преобразется в unsigned так как UI::Size это point<unsigned>
}
