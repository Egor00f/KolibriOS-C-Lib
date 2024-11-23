#ifndef __BACKGROUND_HPP__
#define __BACKGROUND_HPP__

#include <include_ksys.h>

#include <stddef.h>

#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/img.hpp>
#include <kolibriLib/globals.hpp>

namespace KolibriLib
{
	/// @brief Работа с фоном
	namespace Background
	{

		/// @brief Получить размеры фонового изображения
		/// @return размер фонового изображения
		inline Size GetSize()
		{
			ksys_pos_t p;
			asm_inline (
				"int $0x40"
				: "=a"(p)
				: "a"(39), "b"(1)
			);

			return Size(p.x, p.y);
		}

		/// @brief Прочитать точку на фоне
		/// @param Point читаема точка
		/// @return Цвет точки
		inline Colors::Color ReadPoint(const Coord &Point)
		{
			Colors::Color c;

			// Смещение
			int s = GetSize().x * (Point.y - 1) + Point.x; // я думаю, что изображение это двумерный массив

			asm_inline (
				"int $0x40"
				: "=a"(c.val)
				: "a"(39), "b"(2), "c"(s)
			);

			return c;
		}

		/// @brief Перерисовать весь фон
		inline void RedrawBackground()
		{
			_ksys_bg_redraw();
		}

		/// @brief Перерисовать область фона
		/// @param coord координаты верхнего левого угла области
		/// @param size размеры 
		inline void RedrawBackground(const Coord& coord, const Size& size)
		{
			_ksys_bg_redraw_bar(coord.operator ksys_pos_t(), (coord + size).operator ksys_pos_t());
		}

		/// @brief установить размер фонового изображения.
		/// @param size размеры фонового изображения
		inline void SetSize(const Size &size)
		{
			_ksys_bg_set_size (
				static_cast<std::uint32_t>(size.x),
				static_cast<std::uint32_t>(size.y)
			);
		}

		/// @brief Поставить точку на фоне
		/// @param coord координаты точки
		/// @param color цвет точки
		inline void DrawPoint(const Coord &coord, const Colors::rgb &color = Globals::SystemColors.work_graph)
		{
			_ksys_bg_put_pixel (
				static_cast<std::uint32_t>(coord.x), 
				static_cast<std::uint32_t>(coord.y), 
				static_cast<std::uint32_t>(GetSize().x), 
				color.RRGGBB()
			);
		}

		/**
		 * @brief Нарисовать изображение на фоне
		 * @param coord Координаты изображения
		 * @param rgb Указатель на массив
		 * @param len Длинна массива(кол-во элементов в массиве)
		 */
		inline void DrawImage(const Coord &coord, rgb_t *rgb, std::size_t len)
		{
			_ksys_bg_put_bitmap (
				rgb,
				sizeof(rgb_t) * len,
				static_cast<std::uint32_t>(coord.x),
				static_cast<std::uint32_t>(coord.y),
				static_cast<std::uint32_t>(GetSize().x)
			);
		}

		/**
		 * @brief Нарисовать изображение на фоне
		 * @tparam N длинна массива rgb
		 * @param coord Координаты изображения
		 * @param rgb Указатель на массив
		 * @details Лучше используйте DrawImage(const Coord&, rgb_t*, std::size_t), но если вам настолько лень... то ладно
		 */
		template <std::size_t N>
		inline void DrawImage(const Coord &coord, rgb_t (&rgb)[N])
		{
			DrawImage(coord, rgb, N);
		}

		/// @brief Нарисовать линию на фоне
		/// @param p1 точка первая
		/// @param p2 точка вторая
		/// @param color цвет линии
		inline void DrawLine(const Coord& p1, const Coord& p2, const Colors::Color &color = Globals::SystemColors.work_graph)
		{
			for(int i = 0; i < abs(p1.x - p2.x); i++)
			{
				for (int j = 0; j < abs(p1.y - p2.y) / abs(p1.x - p2.x); j++)
				{
					Background::DrawPoint(Coord(p1.x + i, p1.y + j), color);
				}
			}
		}
	} // namespace Background
	
} // namespace KolibriLib


#endif // __BACKGROUND_H__