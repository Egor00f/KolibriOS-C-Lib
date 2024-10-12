#ifndef __BUF2D_HPP__
#define __BUF2D_HPP__

#include <C_Layer/INCLUDE/kolibri_buf2d.h>
#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <assert.h>



/**
 * @brief Оболочка для библиотеки Buf2d
 */
namespace buf2d
{
	/**
	 * @brief Загрузщик либы
	 * @details Автоматически загружет билиотеку
	 */
	class Loader final
	{
	public:
		/**
		 * @brief Конструктор
		 */
		Loader();
	};

	/**
	 * @brief Экземпляр загрузщика
	 */
	extern Loader buf2dLoader;

	/// @brief Глубина цвета
	enum class BPP : std::uint8_t
	{
		/// @brief  8 битный цвет
		bpp8 = 8,

		/// @brief 24 битный цвет
		RGB = 24,

		/// @brief 24 битный цвет + прозрачность
		RGBA = 32
	};

	/**
	 * @brief Вращение
	 */
	enum class RotateEnum
	{
		/// @brief повернуть на 90 градусов
		rotate_90 = 90,

		/// @brief Повернуть 180 градусов
		rotate_180 = 180,

		/// @brief повернуть на 270 градусов
		rotate_270
	};

	/**
	 * @brief Создать буффер
	 * @param size размеры буфера
	 * @param color цвет
	 * @param bpp бит на пиксель
	 * @return указатель на созданный буффер
	 */
	inline buf2d_struct* Create(const KolibriLib::Size& size, KolibriLib::Colors::Color color, BPP bpp)
	{
		return buf2d_create(0U, 0U, 
			static_cast<unsigned>(size.x), 
			static_cast<unsigned>(size.y), 
			color, 
			static_cast<std::uint8_t>(bpp));
	}

	/**
	 * @brief Создать буффер
	 * @param size размеры буфера
	 * @param color цвет
	 * @param bpp бит на пиксель
	 * @return указатель на созданный буффер
	 */
	inline void Create(buf2d_struct* buff, const KolibriLib::Size& size, KolibriLib::Colors::Color color, BPP bpp)
	{
		buff = Create(size, color, bpp);
	}

	/**
	 * @brief Удалить буффер
	 * @param buff 
	 */
	inline void Delete(buf2d_struct* buff)
	{
		buf2d_delete(buff);
	}

	/**
	 * @brief Очистить буффер цветом
	 * @param buff буффер
	 * @param color цвет
	 */
	inline void Clear(buf2d_struct* buff, KolibriLib::Colors::Color color)
	{
		buf2d_clear(buff, color);
	}

	/**
	 * @brief Нарисовать буфер
	 * @param buff буфер
	 * @note только RGB буфера
	 */
	inline void Draw(buf2d_struct* buff)
	{
		assert(buff->color_bit == static_cast<uint8_t>(BPP::RGB));

		buf2d_draw(buff);
	}

	/**
	 * @brief Изменить пиксель
	 * @param buff 
	 * @param coord координаты пикселя
	 * @param color 
	 */
	inline void SetPixel(buf2d_struct* buff, const KolibriLib::Coord& coord, KolibriLib::Colors::Color color)
	{
		buf2d_set_pixel(buff, static_cast<unsigned>(coord.y), static_cast<unsigned>(coord.x), color);
	}

	/**
	 * @brief Получить пиксель
	 * @param buff буффер
	 * @param coord координаты пикселя
	 * @return цвет пикселя
	 */
	inline KolibriLib::Colors::Color GetPixel(buf2d_struct* buff, const KolibriLib::Coord& coord)
	{
		return buf2d_get_pixel(buff, static_cast<unsigned>(coord.x), static_cast<unsigned>(coord.y));
	}

	/**
	 * @brief Изменить размер буффера
	 * @param buff буфер
	 * @param NewSize новый размер
	 * @param scale растягивать ли изображение
	 */
	inline void Resize(buf2d_struct* buff, const KolibriLib::Size& NewSize, bool scale = true)
	{
		buf2d_resize(buff, static_cast<unsigned>(NewSize.y), static_cast<unsigned>(NewSize.x), 1 + scale);
	}

	/**
	 * @brief Нарисовать прямоугольник (линии)
	 * @param buff буффер в котором рисуктся прямоугольник
	 * @param coord Координаты левого верхнего угла прямоугольника
	 * @param size размер прямоугольника
	 * @param color цвет
	 */
	inline void DrawRect(buf2d_struct* buff, const KolibriLib::Coord& coord, const KolibriLib::Size& size, KolibriLib::Colors::rgb color)
	{
		buf2d_rect_by_size(
			buff, 
			static_cast<unsigned>(coord.x), 
			static_cast<unsigned>(coord.y), 
			static_cast<unsigned>(size.x), 
			static_cast<unsigned>(size.y), 
			color);
	}

	/**
	 * @brief Нарисовать прямоугольник (закрашенный)
	 * @param buff буффер в котором рисуктся прямоугольник
	 * @param coord Координаты левого верхнего угла прямоугольника
	 * @param size размер прямоугольника
	 * @param color цвет
	 */
	inline void DrawFilledRect(buf2d_struct* buff, const KolibriLib::Coord& coord, const KolibriLib::Size& size, KolibriLib::Colors::rgb color)
	{
		buf2d_filled_rect_by_size(
			buff, 
			static_cast<unsigned>(coord.x), 
			static_cast<unsigned>(coord.y), 
			static_cast<unsigned>(size.x), 
			static_cast<unsigned>(size.y), 
			color);
	}

	/**
	 * @brief Применить прозрачность
	 * @param dst Буфер на котором рисуется (RGB)
	 * @param src буфер с прозрачностью
	 * @param coord Координаты src на dst. Координаты для вывода изображения, определяют положение рисуемой картинки в buf_0
	 * @details рабоает так же как и BitBlt, но эта применяет прозрачность.
	 * Для расчета прозрачности используются координаты src
	 */
	inline void ApplyTrasparency(buf2d_struct* dst, const buf2d_struct* src, const KolibriLib::Coord& coord = {0,0})
	{
		assert(src->color_bit == static_cast<uint8_t>(BPP::RGBA) && dst->color_bit == static_cast<uint8_t>(BPP::RGB));

		buf2d_bit_blt_transp(dst, coord.x, coord.y, const_cast<buf2d_struct*>(src));
	}

	/**
	 * @brief 
	 * @param dst 
	 * @param src буфер с прозрачностью
	 * @param coord координаты
	 */
	inline void AddTransparency(buf2d_struct* dst, const buf2d_struct* src, const KolibriLib::Coord& coord = {0,0})
	{
		assert(src->color_bit == static_cast<uint8_t>(BPP::bpp8) && dst->color_bit == static_cast<uint8_t>(BPP::RGB));

		buf2d_bit_blt_alpha(
			dst, 
			static_cast<unsigned>(coord.x), 
			static_cast<unsigned>(coord.y), 
			const_cast<buf2d_struct*>(src));
	}


	/**
	 * @brief Разместить один буфер на другом
	 * @param dst буфер на котором рисуется (RGB)
	 * @param src буфер на который рисуется (RGB или RGBA)
	 * @param coord Координаты src на dst. Координаты для вывода изображения, определяют положение рисуемой картинки в buf_0
	 * @details Рисует в буфере изображение из другого буфера в указанных координатах. Буфер в котором рисут (приемник) должен быть 24 битным, а тот который рисуется (источник) 24 или 32 битным. Если буфер источник 32 битный, то его прозрачность при рисовании не учитывается, для учета прозрачности используется функция ApplyTrasparency
	 */
	inline void BitBlt(buf2d_struct* dst, const buf2d_struct* src, const KolibriLib::Coord& coord = {0,0})
	{
		assert( (src->color_bit == static_cast<uint8_t>(BPP::RGBA) || src->color_bit == static_cast<uint8_t>(BPP::RGB) ) && 
		         dst->color_bit == static_cast<uint8_t>(BPP::RGB));

		buf2d_bit_blt(
			dst, 
			static_cast<unsigned>(coord.x), 
			static_cast<unsigned>(coord.y), 
			const_cast<buf2d_struct*>(src));
	}

	/**
	 * @brief Нарисовать круг
	 * @param buff буффер
	 * @param coord 
	 * @param Radius 
	 * @param color 
	 */
	inline void DrawCircle(buf2d_struct* buff, const KolibriLib::Coord& coord, unsigned Radius, KolibriLib::Colors::Color color)
	{
		buf2d_circle(
			buff, 
			static_cast<unsigned>(coord.x), 
			static_cast<unsigned>(coord.y), 
			Radius,
			color);
	}

	/**
	 * @brief Нарисовать кривую бизье
	 * @param buff буффер
	 * @param points точки по которым строится кривая
	 * @param color цвет прямой
	 */
	inline void DrawBezierCurver(buf2d_struct* buff, KolibriLib::Coord points[3], KolibriLib::Colors::Color color)
	{
		buf2d_curve_bezier(buff, points[0], points[1], points[2], color);
	}

	/**
	 * @brief Нарисовать линию
	 * @param buff буффер
	 * @param p1 точка
	 * @param p2 точка
	 * @param color цвет линии
	 */
	inline void DrawLine(buf2d_struct* buff, const KolibriLib::Coord& p1, const KolibriLib::Coord&p2, KolibriLib::Colors::Color color)
	{
		buf2d_line(
			buff, 
			static_cast<unsigned>(p1.x), 
			static_cast<unsigned>(p1.y), 
			static_cast<unsigned>(p2.x), 
			static_cast<unsigned>(p2.y), 
			color);
	}

	/**
	 * @brief Нарисовать сглаженную линию
	 * @param buff буффер
	 * @param p1 точка
	 * @param p2 точка
	 * @param color цвет линии
	 */
	inline void DrawSmothLine(buf2d_struct* buff, const KolibriLib::Coord& p1, const KolibriLib::Coord&p2, KolibriLib::Colors::Color color)
	{
		buf2d_line(
			buff, 
			static_cast<unsigned>(p1.x), 
			static_cast<unsigned>(p1.y), 
			static_cast<unsigned>(p2.x), 
			static_cast<unsigned>(p2.y), 
			color);
	}

	/**
	 * @brief Обёртка для buf2d_struct
	 * @details Просто обёртка к конструктором и деструктором
	 * Предпочтительнее использовать такой вид объявления:
	 * @code
	 * buffer* buff = new buffer;
	 * @endcode
	 */
	struct buffer : public buf2d_struct
	{
		/**
		 * @brief Конструктор по умолчанию
		 */
		buffer();

		/**
		 * @brief Конструктор
		 * @param bpp глубина увета
		 */
		buffer(BPP bpp);

		/**
		 * @brief Конструктор
		 * @param size размер буфера
		 * @param bpp 
		 */
		buffer(const KolibriLib::Size& size, BPP bpp = BPP::RGB);

		/**
		 * @brief Конструктор
		 * @param size размер
		 * @param color цвет фона
		 * @details создаёт RGB буфер
		 */
		buffer(const KolibriLib::Size& size, KolibriLib::Colors::rgb color);

		/**
		 * @brief Конструктор
		 * @param size размер
		 * @param color цвет фона
		 * @details создаёт RGBA
		 */
		buffer(const KolibriLib::Size& size, KolibriLib::Colors::Color color);

		/**
		 * @brief Конструктор копирования
		 * @param buff указатель на буффер
		 */
		buffer(const buf2d_struct* buff);

		/**
		 * @brief Деструктор
		 */
		~buffer();

		/**
		 * @brief Оператор
		 * @details по сути эта структура таже что и buf2d_struct, только с конструкторм и деструктором
		 */
		operator buf2d_struct* ();

		/**
		 * @brief Поменять значения местами
		 * @param buff с чем менять
		 * @details указатели на изображения не трогает, просто меняет их местами
		 */
		void swap(buffer& buff);
	};
}


#endif // __BUF2D_HPP__