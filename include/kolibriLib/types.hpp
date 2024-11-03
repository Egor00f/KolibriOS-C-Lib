/*
	Некоторые базовые типы
*/
#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <include_ksys.h>

#include <ostream>

/**
 * @brief 
 */
#define X_Y(x, y) (((x) << 16) | (y))

namespace KolibriLib
{

	/// @brief Просто точка
	struct point
	{
		/// @brief координата по оси X, или ширина
		int x;

		/// @brief координата по оси Y, или высота
		int y;

		/// @brief Конструктор по умолчанию
		/// @details координаты {0,0}
		point();

		/// @brief Конструктор
		/// @param X
		/// @param Y
		point(int X, int Y);

		/// @brief Конструктор
		/// @param Number
		point(int Number);

		/// @brief Конструктор
		/// @param pos
		point(const ksys_pos_t &pos);

		/// @brief Конструктор копирования
		/// @param p
		point(const point &p);

		ksys_pos_t GetKsysPost() const;

		operator ksys_pos_t() const;

		/// @details прибавляет к x и y значения из a (к x прибавляется a.x, к y прибавляется a.y)
		point &operator+=(const point &a);

		/// @details уменьшает x и y на значения из a (из x вычитается a.x, из y вычитается a.y)
		point &operator-=(const point &a);

		point operator+(const point &a) const;
		point operator-(const point &a) const;

		/// @brief
		/// @param p
		/// @return
		point &operator=(const point &p);

		/**
		 * @brief Прибавить к обоим координатам значение
		 * @param p значение
		 * @return
		 * @details Прибавляет к обеим координатам p
		 */
		point &operator+=(const int &p);

		/**
		 * @brief Вычесть из обоих координат значение
		 * @param p значение
		 * @return
		 * @details Вычитает из обеих координат p
		 */
		point &operator-=(const int &p);

		/**
		 * @brief
		 * @param p
		 * @return
		 * @details Умножает обе координаты на p
		 */
		point &operator*=(const int &p);

		/**
		 * @brief
		 * @param p
		 * @return
		 * @details Делит обе координаты на p
		 */
		point &operator/=(const int &p);

		/// @brief
		/// @param a
		/// @return
		bool operator==(const point &a) const;

		/// @brief
		/// @param a
		/// @return
		bool operator!=(const point &a) const;

		/**
		 * @brief
		 * @param obj
		 * @return
		 */
		bool operator<(const point &obj) const;

		/**
		 * @brief
		 * @param obj
		 * @return
		 */
		bool operator>(const point &obj) const;
	};

	/// @brief Размер
	using Size = point;

	/// @brief Координаты
	using Coord = point;

	/// @brief Координаты/Размеры для элементов UI
	struct UDim
	{
		/// @brief Ось
		struct Axis
		{
			/// @brief Относительно размера окна
			float Scale;

			/// @brief В пикселях
			int Offset;

			/// @brief Конструктор
			/// @param scale
			/// @param offset смещение в пикселях
			Axis(float scale = 0, int offset = 0);

			Axis &operator=(const Axis &) = default;

			/// @brief оператор сравнения
			/// @param axis с чем сравнивать
			/// @note сравнивает с точностью до тысячных ибо зачем очень большая точность для gui
			/// @return
			bool operator==(const Axis &axis) const;

			/// @brief
			/// @param axis с чем сравнивать
			/// @return
			bool operator!=(const Axis &axis) const;
		};

		/// @brief Ось X
		UDim::Axis X;

		/// @brief Ось Y
		UDim::Axis Y;

		/// @brief Конструктор
		/// @param XScale Относительный размер по оси X
		/// @param XOffset смещение по оси X
		/// @param YScale Относительный размер по оси Y
		/// @param YOffset смещение по оси
		UDim(float XScale, int XOffset, float YScale, int YOffset);

		/// @brief Конструктор
		/// @param x смещение по x
		/// @param y смещение по y
		/// @details только смещение
		UDim(int x = 0, int y = 0);

		/**
		 * @brief Конструктор
		 * @param x
		 * @param y
		 */
		UDim(const Axis &x, const Axis &y);

		/// @brief Конструктор
		/// @param x Относительный размер по x
		/// @param y Относительный размер по y
		UDim(float x, float y);

		/// @brief Конструктор
		/// @details делает тоже самое что и UDim(int, int), только x и y берутся из точки
		/// @param p точка
		UDim(const point &p);

		/// @brief получить абсолютные значения(в пикселях) относительно окна
		/// @param Parent координаты/размер того относительно чего рассчитывается
		/// @return абсолютные координаты/размер
		point GetAbsolute(const point &Parent) const;

		UDim &operator=(const UDim &) = default;

		/**
		 * @brief Оператор сравнения
		 * @param obj с чем сравнивать
		 * @return
		 */
		bool operator==(const UDim &obj) const;

		/**
		 * @brief Оператор сравнения
		 * @param obj с чем сравнивать
		 * @return
		 */
		bool operator!=(const UDim &obj) const;
	};

	/**
	 * @brief Просто область
	 * @tparam T тип
	 */
	template <class T>
	struct Area
	{
		T Coord;
		T Size;

		Area& operator = (const Area&) = default;

		bool operator == (const Area& a) const
		{
			return Coord == a.Coord && Size != a.Size;
		}

		bool operator != (const Area& a) const
		{
			return Coord != a.Coord || Size != a.Size;
		}
	};

	using UDimArea = Area<UDim>;
	using AbsArea = Area<point>;

} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::point &p)
{
	return os << "X: " << p.x << " Y: " << p.y;
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UDim::Axis &p)
{
	return os << "Scale: " << p.Scale << " Offset: " << p.Offset;
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UDim &p)
{
	return os << "X: " << p.X << " Y: " << p.Y;
}

template <class T>
inline std::ostream &operator<<(std::ostream &os, const KolibriLib::Area<T> &p)
{
	return os << "Coord " << p.Coord << std::endl << " Size: " << p.Size;
}


#endif // __TYPES_HPP__
