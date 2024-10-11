#include <kolibriLib/input/clipboard.hpp>
#include <cstdlib>

using namespace KolibriLib;

KolibriLib::Clipboard::clipboard::clipboard(const std::string &text, bool filepath)
{
	std::size_t s = 4;

	if(filepath)
		s += (2 + text.length());
	else
		s += 4;
	
	_struct = static_cast<clipboard_struct *>(std::malloc(s));
	
	_struct->size = s;
	
	if(filepath)
	{
		_struct->type = clipboard_struct::Type::Filepath;

		_struct->res = 0x0001;

		for(std::size_t i = 0; i < text.length(); i++)
			_struct->filepath[i] = text[i];
	}
	else
	{
		_struct->type = clipboard_struct::Type::Text;
	}
}

KolibriLib::Clipboard::clipboard::clipboard(const UI::Images::img &image)
{
	const std::size_t size = 4 + 4 + (4 * 4) + sizeof(void*) + sizeof(rgb_t*);
	_struct = static_cast<clipboard_struct *>(std::malloc(size));
	
	_struct->X	= static_cast<std::uint32_t>(image.GetSize().x);
	_struct->Y	= static_cast<std::uint32_t>(image.GetSize().y);
	_struct->size	= size;
	_struct->type	= clipboard_struct::Type::Image;
	_struct->depht	= static_cast<uint32_t>(image.GetBPP());
	_struct->image	= (rgb_t*)((std::shared_ptr<buf2d_struct>)image).get()->buf_pointer;
	_struct->pallete	= 0;
	_struct->imageSize	= _struct->X * _struct->Y * _struct->depht;
	_struct->palleteSize	= 0;
}

KolibriLib::Clipboard::clipboard::~clipboard()
{
	if(_struct != nullptr)
		free(_struct);
}

KolibriLib::Clipboard::clipboard::operator clipboard_struct *() const
{
	return _struct;
}