#include <kolibriLib/UI/text/freetype.hpp>
#include <kolibriLib/img.hpp>
#include <limits>
#include <algorithm>

using namespace FreeType;

Library FreeType::_lib;

Error::operator std::string() const
{
	std::string ret;

	switch (val)
	{
	case FT_Err_Ok:
		ret = "Ok";
		break;
	case FT_Err_Cannot_Open_Resource:
		ret = "Cannot Open Resource";
		break;
	case FT_Err_Unknown_File_Format:
		ret = "unknown file format";
		break;
	case FT_Err_Invalid_File_Format:
		ret = "broken file";
		break;
	case FT_Err_Invalid_Version:
		ret = "invalid FreeType version";
		break;
	case FT_Err_Lower_Module_Version:
		ret = "module version is too low";
		break;
	case FT_Err_Invalid_Argument:
		ret = "invalid argument";
		break;
	case FT_Err_Unimplemented_Feature:
		ret = "unimplemented feature";
		break;
	case FT_Err_Invalid_Table:
		ret = "broken table";
		break;
	}

	return ret;
}

Error::operator bool() const
{
	return val != FT_Err_Ok;
}

Error::Error(FT_Error v)
	: val(v)
{
}

Error &Error::operator=(const FT_Error &v)
{
	val = v;

	return *this;
}

FreeType::Error::operator FT_Error() const
{
	return val;
}

Library::Library()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Init FreeType Lib" << std::endl;

	lib = 0;

	Error err = FT_Init_FreeType(&lib);

	if (err)
	{
		KolibriLib::logger << microlog::LogLevel::Error << "Error init FreeType lib, Error: " << err << std::endl;

		throw err;
	}
}

FreeType::Library::~Library()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "done FreeType Lib" << std::endl;

	FT_Error err = FT_Done_FreeType(lib);

	if (err)
	{
		KolibriLib::logger << microlog::LogLevel::Error << "Error done FreeType Lib, Error: " << err << std::endl;

		throw err;
	}
}

FreeType::Library::operator FT_Library() const
{
	return lib;
}

FreeType::Face::Face()
	: face(0)
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Face constructor (face=0)\nFace constructor done" << std::endl;
}

FreeType::Face::Face(const Face &c)
	: face(c.face)
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Face constructor(copy)\nFace constructor done" << std::endl;
}

FreeType::Face::Face(const char *file)
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Face constructor" << std::endl;

	face = 0;

	FT_Error err = FT_New_Face(
		_lib,
		file,
		0,
		&face);

	if (err)
	{
		KolibriLib::logger << microlog::LogLevel::Error << "Error init FreeType face, Error: " << err << std::endl;

		throw err;
	}

	KolibriLib::logger << microlog::LogLevel::Debug << "Face constructor done" << std::endl; // На всякий случай
}

FreeType::Face::~Face()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Face destructor" << std::endl;

	Error err = FT_Done_Face(face);

	if (err)
	{
		KolibriLib::logger << microlog::LogLevel::Error << "Error done FreeType face, Error: " << err << std::endl;

		throw err;
	}

	KolibriLib::logger << microlog::LogLevel::Debug << "Face destructor done" << std::endl;
}

Error FreeType::Face::OpenFile(const char *file)
{
	Error err = 0;

	if (face)
	{
		err = FT_Done_Face(face);
	}

	if (err)
	{
		err = FT_New_Face(_lib, file, 0, &face);
	}

	return err;
}

FreeType::Face::operator FT_Face() const
{
	return face;
}

FT_Error FreeType::Face::SetCharSize(const KolibriLib::Size &NewCharSize)
{
	const KolibriLib::Size ScreenSize = KolibriLib::GetScreenSize();

	return FT_Set_Char_Size(
		face,
		NewCharSize.x * 64,
		NewCharSize.y * 64,
		ScreenSize.x,
		ScreenSize.y);
}

FT_Error FreeType::Face::SetCharSizePixels(const KolibriLib::Size &NewCharSize)
{
	return FT_Set_Pixel_Sizes(
		face,
		NewCharSize.x,
		NewCharSize.y);
}

FT_UInt FreeType::Face::GetGlyphIndex(const CharCode &charCode)
{
	return FT_Get_Char_Index(face, charCode);
}

FT_Error FreeType::Face::LoadGlyph(FT_UInt GlyphIndex, FT_Int LoadFlags)
{
	return FT_Load_Glyph(
		face,
		GlyphIndex,
		LoadFlags);
}

FT_Error FreeType::Face::LoadChar(const CharCode &charCode, FT_Int LoadFlags)
{
	return FT_Load_Char(face, charCode, LoadFlags);
}

FT_Error FreeType::Face::RenderGlyph(FT_Render_Mode RenderMode)
{
	return FT_Render_Glyph(face->glyph, RenderMode);
}

FT_Error FreeType::Face::SelectCharmap(FT_Encoding encoding)
{
	return FT_Select_Charmap(face, encoding);
}

void FreeType::Face::SetTransform(FT_Matrix *matrix, FT_Vector *delta)
{
	FT_Set_Transform(
		face,
		matrix,
		delta);
}

FT_Vector Face::GetKerning(CharCode leftCharCode, CharCode rightCharCode, FT_Kerning_Mode KerningMode)
{
	FT_Vector delta;

	FT_Error err = FT_Get_Kerning(face, GetGlyphIndex(leftCharCode), GetGlyphIndex(rightCharCode), KerningMode, &delta);

	if (err)
		throw err;

	return delta;
}

FT_Glyph Face::GetGlyph()
{
	FT_Glyph ret;

	FT_Error err = FT_Get_Glyph(face->glyph, &ret);

	if (err)
	{
		KolibriLib::logger << microlog::LogLevel::Error << "Error Get GLyph, Error: " << err << std::endl;

		throw err;
	}

	return ret;
}

Stroker::Stroker()
{
	Error err = FT_Stroker_New(_lib, &stroker);

	if (err)
	{
		KolibriLib::logger << "Error init FreeType Stroker, Error: " << err << std::endl;

		throw err;
	}
}

Stroker::~Stroker()
{
	FT_Stroker_Done(stroker);
}

void FreeType::Stroker::Set(FT_Fixed radius, FT_Stroker_LineCap cap, FT_Stroker_LineJoin join, FT_Fixed miterLimit)
{
	FT_Stroker_Set(stroker, radius, cap, join, miterLimit);
}

void FreeType::Stroker::Rewind()
{
	FT_Stroker_Rewind(stroker);
}

FreeType::Stroker::operator FT_Stroker() const
{
	return stroker;
}

Error FreeType::Glyph::StrokeBorder(Stroker stroker, FT_Bool inside, FT_Bool destroy)
{
	return FT_Glyph_StrokeBorder(&_glyph, stroker, inside, destroy);
}

Error FreeType::Glyph::ToBitmap(FT_Render_Mode renderMode, FT_Vector *origin, FT_Bool destroy)
{
	return FT_Glyph_To_Bitmap(&_glyph, renderMode, origin, destroy);
}

FT_BitmapGlyph FreeType::Glyph::GetBitmapGlyph(FT_Render_Mode renderMode, FT_Vector *origin, FT_Bool destroy)
{
	ToBitmap(renderMode, origin, destroy);
	return reinterpret_cast<FT_BitmapGlyph>(_glyph);
}

FreeType::Glyph::Glyph(FT_Glyph glyph)
	: _glyph(glyph)
{
}

FreeType::Glyph::Glyph(const Glyph &glyph)
{
	_glyph = glyph.Clone().operator FreeType::FT_Glyph();
}

Glyph FreeType::Glyph::Clone() const
{
	Glyph ret;
	Error err = FT_Glyph_Copy(_glyph, &ret._glyph);

	if (err)
	{
		throw err;
	}

	return ret;
}

FreeType::Glyph::operator FT_Glyph() const
{
	return _glyph;
}

void FreeType::DrawText(const KolibriLib::Coord &coord, const std::string text, Face face)
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Draw Text" << std::endl;

	for (std::size_t i = 0; i < text.size(); i++)
	{
		KolibriLib::logger << microlog::LogLevel::Debug << "Render char:" << text[i] << std::endl;

		face.LoadGlyph(face.GetGlyphIndex(text[i]), FT_RENDER_MODE_LCD);

		FT_Bitmap bitmap = ((FT_Face)face)->glyph->bitmap;
		FT_Glyph_Metrics metrics = ((FT_Face)face)->glyph->metrics;

		uint32_t colStartPos = metrics.horiBearingX / 64;
		uint32_t rowStartPos = metrics.horiBearingY / 64;

		KolibriLib::UI::Images::img image(KolibriLib::Colors::Color(0xFFFFFF), {bitmap.width, bitmap.rows});

		for (uint32_t y = 0; y < bitmap.rows; y++)
		{
			uint32_t row = rowStartPos + y;
			for (uint32_t x = 0; x < bitmap.width; x++)
			{
				uint32_t col = colStartPos + x;
				uint8_t r = bitmap.buffer[y * bitmap.pitch + x * 3];
				uint8_t g = bitmap.buffer[y * bitmap.pitch + x * 3 + 1];
				uint8_t b = bitmap.buffer[y * bitmap.pitch + x * 3 + 2];

				image.SetPixel({x, y}, KolibriLib::Colors::Color(r, g, b, 0));
			}
		}

		image.Draw(coord);
	}
}