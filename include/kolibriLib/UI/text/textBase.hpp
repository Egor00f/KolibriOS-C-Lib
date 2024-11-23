#ifndef __TEXTBASE_HPP__
#define __TEXTBASE_HPP__

#include <kolibriLib/color.hpp>
#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <kolibriLib/globals.hpp>

// #include "font.hpp"

namespace KolibriLib
{
	namespace UI
	{
		/**
		 * @brief Работа с текстов
		 */
		namespace text
		{
			/**
			 * @brief Список кодировок текста для системной функции 4
			 */
			enum class TextEncoding
			{
				/**
				 * @brief CP866 (размер символов 6x9)
				 */
				cp866_6x9 = 0,

				/**
				 * @brief CP866 (размер символов 8x16)
				 */
				cp866_8x16 = 1,

				/**
				 * @brief UTF16LE (размер символов 8x16)
				 */
				UTF16LE = 2,

				/**
				 * @brief UTF8 (размер символов 8x16)
				 */
				UTF8 = 3
			};

			/**
			 * @brief сглаживание текста
			 */
			enum class FontSmoothing
			{
				/**
				 * @brief Выключить сглаживание текста
				 */
				Disable = 0,

				/**
				 * @brief Нормальное сглаживание текста
				 */
				Normal = 1,

				/**
				 * @brief Субпиксельное сглаживание текста
				 */
				SubPixel = 2
			};

			struct Buffer
			{
				/**
				 * @brief Размер буфера по X
				 */
				std::uint16_t sizeX;

				/**
				 * @brief Размер буфера по Y
				 */
				std::uint16_t SizeY;

				/**
				 * @brief Буфер
				 * @details размер sizeX * SizeY
				 */
				std::uint32_t image[];
			};

			/// \brief Получить размер текста
			/// \return текущая высота текста
			inline std::uint32_t GetTextSize()
			{
				std::uint32_t Size;

				asm_inline(
					"int $0x40"
					: "=c"(Size)
					: "a"(48), "b"(11));

				return Size;
			}

			/// \brief Изменить размер текста
			/// \param newSize высота текста в px
			inline void SetTextSize(std::uint8_t newSize)
			{
				asm_inline(
					"int $0x40" ::"a"(48), "b"(12), "c"(newSize));
			}

			/// \brief Просто вывести текст
			/// \param text текст
			/// \param coord координаты
			/// \param color цвет текста
			/// @note Для изменения высоты шрифта используйте SetTextSize()
			inline void DrawText(const std::string &text, const Coord &coord, std::uint8_t size = 9, const Colors::Color &color = Globals::SystemColors.work_text)
			{
				SetTextSize(size);
				_ksys_draw_text(
					text.c_str(),
					static_cast<std::uint32_t>(coord.x),
					static_cast<std::uint32_t>(coord.y),
					text.length(),
					color);
			}

			/**
			 *  @brief Вывести текст в окно
			 * @param text текст
			 * @param coord координаты текста
			 * @param color цвет текста
			 * @param encoding кодировка
			 * @param scale множитель размера(по умолчанию 1x), максимум 8x (0 = 1x, 7 = 8x)
			 * @note Если scale = 0, текст может сглаживаться в зависимости от системной настройки
			 */
			inline void DrawText(const std::string &text, const Coord &coord, Colors::Color color = Globals::SystemColors.work_text, TextEncoding encoding = TextEncoding::UTF8, std::uint8_t scale = 0)
			{
				color._a = 0;
				color._a = static_cast<std::uint8_t>(static_cast<std::uint8_t>(encoding) << 4 | scale);

				asm_inline(
					"int $0x40" ::
						"a"(4),
					"b"(coord.operator ksys_pos_t()),
					"c"(color.val),
					"d"(text.c_str()),
					"S"(text.length()));
			}

			/**
			 *  @brief Вывести текст в окно
			 * @param text текст
			 * @param coord координаты текста
			 * @param BackgroundColor Цвет фона
			 * @param color цвет текста
			 * @param encoding кодировка текста
			 * @param scale множитель размера(по умолчанию 1x), максимум 8x (0 = 1x, 7 = 8x)
			 * @note Если scale = 0, текст может сглаживаться в зависимости от системной настройки
			 */
			inline void DrawText(const std::string &text, const Coord &coord, Colors::Color BackgroundColor, Colors::Color color = Globals::SystemColors.work_text, TextEncoding encoding = TextEncoding::UTF8, std::uint8_t scale = 0)
			{
				color._a = 0;
				color._a = static_cast<std::uint8_t>((true << 6) | static_cast<std::uint8_t>(encoding) << 4 | scale);

				asm_inline(
					"int $0x40" ::
						"a"(4),
					"b"(coord.operator ksys_pos_t()),
					"c"(color.val),
					"d"(text.c_str()),
					"S"(text.length()),
					"D"(BackgroundColor.val));
			}

			/**
			 * @brief Вывести текст в буфер
			 * @param buff указатель на буфер
			 * @param text текст
			 * @param coord координаты текста
			 * @param color цвет текста
			 * @param encoding кодировка текста
			 * @param scale множитель размера(по умолчанию 1x), максимум 8x (0 = 1x, 7 = 8x)
			 * @note Если scale = 0, текст может сглаживаться в зависимости от системной настройки
			 */
			inline void DrawText(Buffer *buff, const std::string &text, const Coord &coord, Colors::Color color = Globals::SystemColors.work_text, TextEncoding encoding = TextEncoding::UTF8, std::uint8_t scale = 0)
			{
				color._a = 0;
				color._a = static_cast<std::uint8_t>(static_cast<std::uint8_t>(encoding) << 4 | true << 3 | scale);

				asm_inline(
					"int $0x40" ::
						"a"(4),
					"b"(coord.operator ksys_pos_t()),
					"c"(color.val),
					"d"(text.c_str()),
					"S"(text.length()),
					"D"(buff));
			}

			/**
			 * @brief Получить настройку сглаживания шрифтов
			 * @return
			 */
			inline FontSmoothing GetFontSmoothing()
			{
				FontSmoothing ret;

				asm_inline(
					"int $0x40"
					: "=a"(ret)
					: "a"(48), "b"(9));

				return ret;
			}

			/**
			 * @brief Изменить настройку сглаживания шрифтов
			 * @param NewFontSmoothing
			 */
			inline void SetFontSmoothing(FontSmoothing NewFontSmoothing)
			{
				asm_inline(
					"int $0x40" ::"a"(48), "b"(10), "c"(NewFontSmoothing));
			}
		} // namespace text
	} // namespace UI
} // namespace KolibriLib

#endif // __TEXTBASE_HPP__