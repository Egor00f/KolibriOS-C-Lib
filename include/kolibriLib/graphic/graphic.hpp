#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include <include_ksys.h>

#include <kolibriLib/types.hpp>
#include <kolibriLib/graphic/screen.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/globals.hpp>

#include <cmath>

namespace KolibriLib
{
	/**
	 * @brief Графика
	 */
	namespace graphic
	{
		/// @brief Нарисовать линию
		/// @param a точка первая
		/// @param b точка вторая
		/// @param color Цвет линии
		/// @note Конечная точка также рисуется.
		inline void DrawLine(const Coord &coord, const Coord &b, const Colors::rgb &color = Globals::SystemColors.work_graph)
		{
			_ksys_draw_line (
				coord.x, 
				coord.y, 
				b.x, 
				b.y, 
				color.ZeroRRGGBB()
			);
		}

		/// @brief Нарисовать линию
		/// @param coord Координата от которой рисуется линия
		/// @param length Длина линии
		/// @param angle Угол
		/// @param color Цвет линии
		/// @note Конечная точка также рисуется.
		inline void DrawLine(const Coord &coord, unsigned length, unsigned short angle, const Colors::rgb &color = Globals::SystemColors.work_graph)
		{
			_ksys_draw_line (
				coord.x, 
				coord.y,
				coord.x + lround(length * std::cos(angle)),
				coord.y + lround(length * std::sin(angle)),
				color.ZeroRRGGBB()
			);
		}

		/// @brief Закрасить пиксель точку
		/// @param position координаты
		/// @param color Цвет
		inline void DrawPixel(const Coord &position, const Colors::rgb &color = Globals::SystemColors.work_graph)
		{
			_ksys_draw_pixel (
				static_cast<std::uint16_t>(position.x), 
				static_cast<std::uint16_t>(position.y), 
				color.ZeroRRGGBB()
			);
		}

		/// @brief Нарисовать окружность
		/// @param coord Координаты центра
		/// @param Radius радиус окружности
		/// @param details Детализация прорисовки окружности (то на сколько круг круглый)
		/// @param color Цвет
		/// @paragraph Круг рисуется по detalization линий. Да-да если Детализация = 4, то круг это квадрат.
		void DrawCircle(const Coord& coord, unsigned Radius, const Colors::Color& color = Globals::SystemColors.work_graph);

		/// @brief Нарисовать закрашенный прямоугольник
		/// @param position позиция левого верхнего угла
		/// @param size Размеры
		/// @param color Цвет
		inline void DrawRectangleFill(Coord position, Size size, Colors::rgb color = Globals::SystemColors.work_graph)
		{
			asm_inline (
				"int $0x40"
				::	"a"(13), 
					"b"( X_Y(position.x, size.x) ),
					"c"( X_Y(position.y, size.y) ),
					"d"(color.RRGGBB())
				);
		}

		/// @brief Нарисовать закрашенный прямоугольник градиентной заливки
		/// @param position позиция левого верхнего угла
		/// @param size Размеры
		/// @param color Цвет
		inline void DrawRectangleGradient(Coord position, Size size, Colors::rgb color = Globals::SystemColors.work_graph)
		{
			std::uint32_t val = color.ZeroRRGGBB();
			val |= 0x80000000;

			asm_inline (
				"int $0x40"
				::	"a"(13),
					"b"( X_Y(position.x, size.x) ),
					"c"( X_Y(position.y, size.y) ),
					"d"(val)
				);
		}

		/// @brief Нарисовать круг(закрашенный)
		/// @param coord Координаты центра
		/// @param Radius Радиус круга
		/// @param detalization Детализация круга(то на сколько круг круглый)
		/// @param color Цвет
		void DrawCircleFill(const Coord& coord, const unsigned& Radius, const Colors::Color& color = Globals::SystemColors.work_graph);

		/// @brief Нарисовать точку
		/// @param position Координаты
		/// @param size Радиус точки в рx
		/// @param color Цвет
		/// @param fill Закрашенная/Выколотая точка
		void DrawPoint(const Coord& position, const unsigned& size, const Colors::Color& color = Globals::SystemColors.work_graph, bool fill = false);

		/// @brief Нарисовать прямоугольник
		/// @param a Точка в левом верхнем углу
		/// @param b Точка в правом нижнем углу
		inline void DrawRectangleLines(const Coord &a, const Coord &b, const Colors::Color &color = Globals::SystemColors.work_graph)
		{
			DrawLine(a, Coord(b.x, a.y), color);
			DrawLine(a, Coord(a.x, b.y), color);
			DrawLine(Coord(b.x, a.y), b, color);
			DrawLine(Coord(a.x, b.y), b, color);
		}

		/// @brief Нарисовать треугольник
		/// @param a первая точка
		/// @param b вторая точка
		/// @param c третья точка
		/// @param color цвет линий
		inline void DrawTriangle(const Coord &a, const Coord &b, const Coord &c, const Colors::Color &color = Globals::SystemColors.work_graph)
		{
			DrawLine(a, b, color);
			DrawLine(a, c, color);
			DrawLine(b, c, color);
		}

		/// @brief прочитать цвет точки
		/// @param Point координаты точки
		/// @return Цвет точки на экране
		inline Colors::Color ReadPoint(const Coord Point)
		{
			ksys_color_t c;

			asm_inline (
				"int $0x40"
				: "=a"(c)
				: "a"(35),
				  "b"(Point.x * GetScreenSize().x + Point.y));

			return Colors::Color(c);
		}

		/// @brief Получить область
		/// @param coord 
		/// @param size
		/// @return указатель на массив
		/// @note Не забудьте delete[] 
		inline rgb_t* ReadArea(const Coord &coord, const Size &size)
		{
			rgb_t* result = new rgb_t[size.x * size.y];

			asm_inline (
				"int $0x40"
				:: "a"(36), 
				   "b"(result), 
				   "c"(size.operator ksys_pos_t()), 
				   "d"(coord.operator ksys_pos_t())
			);

			return result;
		}

		/// @brief Вывести изображение
		/// @param bitmap изображение в формате BBGGRR
		/// @param coord координаты
		/// @param size размер изображния
		inline void DrawBitmap(rgb_t *bitmap, const Coord &coord, const Size &size)
		{
			_ksys_draw_bitmap(bitmap, coord.x, coord.y, size.x, size.y);
		}

	} // namespace graphic
} // namespace KolibriLib


#endif // __GRAPHIC_HPP__