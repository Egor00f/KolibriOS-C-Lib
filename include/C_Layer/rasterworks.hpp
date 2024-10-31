#ifndef __RASTERWORKS_HPP__
#define __RASTERWORKS_HPP__

#include <string>

#include <C_Layer/INCLUDE/kolibri_rasterworks.h>
#include <kolibriLib/types.hpp>
#include <kolibriLib/globals.hpp>

class rasterwoksLib
{
public:
	rasterwoksLib();
};

extern rasterwoksLib rasterworksLibv;


/// @brief Оболочка для Сишной версии
/// @param canvas буфер, на котором будем рисовать
/// @param coord координаты текста в буфере
/// @param size размеры буфера
/// @param text текст
/// @param CharSize Размеры символов
/// @param FontColor цвет текста
/// @param flags флаги
/// @param encoding кодировка текста
/// @return буфер
inline void *drawTextToBuff(const void *canvas, const KolibriLib::Coord &coord, const KolibriLib::Size &size, const std::string &text, const KolibriLib::Size &CharSize, const KolibriLib::Colors::Color &FontColor, const std::uint8_t &flags, const std::uint8_t encoding = RasterworksEncoding::Rasterworks_UTF8)
{
	return drawTextToBuff(
		canvas,
		static_cast<uint8_t>(size.x),
		static_cast<uint8_t>(size.y),
		coord.x,
		coord.y,
		text.c_str(),
		text.length(),
		static_cast<uint8_t>(CharSize.x),
		static_cast<uint8_t>(CharSize.y),
		static_cast<ksys_color_t>(FontColor),
		static_cast<uint8_t>(flags),
		static_cast<uint8_t>(encoding));
}

/// @brief тоже что и drawTextToBuff, но сразу выводится текст
/// @param coord Координаты
/// @param size размеры буфера
/// @param text текст
/// @param CharSize
/// @param FontColor
/// @param BackgroundColor
/// @param flags
/// @param encoding
void drawtext(const KolibriLib::Coord &coord, const KolibriLib::Size &size, const std::string &text, const KolibriLib::Size &CharSize, const KolibriLib::Colors::Color &FontColor = KolibriLib::Globals::SystemColors.work_text, const KolibriLib::Colors::Color &BackgroundColor = KolibriLib::Globals::SystemColors.work_area, const std::uint8_t &flags = 0, const std::uint8_t encoding = RasterworksEncoding::Rasterworks_UTF8);

#endif // __RASTERWORKS_HPP__