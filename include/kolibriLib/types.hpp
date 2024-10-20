/*
	Некоторые базовые типы
*/
#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <include_ksys.h>

#include <ostream>

#define X_Y(x, y)(((x) << 16) | (y))

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
		point& operator+=(const point &a);

		/// @details уменьшает x и y на значения из a (из x вычитается a.x, из y вычитается a.y)
		point& operator-=(const point &a);

		point operator+(const point &a) const;
		point operator-(const point &a) const;

		/// @brief 
		/// @param p 
		/// @return 
		point &operator=(const point &p);

		/// @brief Прибавить к обоим кординатам значение
		/// @param p значение
		/// @return 
		point &operator+=(const int &p);

		/// @brief Вычесть из обоих кординат значение
		/// @param p значение
		/// @return
		point &operator-=(const int &p);

		point &operator*=(const int &p);

		point &operator/=(const int &p);

		/// @brief 
		/// @param a 
		/// @return 
		bool operator==(const point &a) const;
		

		/// @brief 
		/// @param a 
		/// @return 
		bool operator!=(const point &a) const;

		bool operator < (const point &obj) const;
		bool operator > (const point &obj) const;
	};

	
	/// @brief Размер
	typedef point Size;

	/// @brief Координаты
	typedef point Coord;

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
			/// @param offset
			Axis(float scale = 0, int offset = 0);

			Axis& operator=(const Axis&) = default;

			/// @brief оперватор сравнения
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
		/// @param XScale
		/// @param XOffset
		/// @param YScale
		/// @param YOffset
		UDim(float XScale, int XOffset, float YScale, int YOffset);

		/// @brief Конструктор
		/// @param x смещение по x
		/// @param y смещение по y
		/// @details только смещение
		UDim(int x = 0, int y = 0);

		UDim(const Axis& x, const Axis& y);

		/// @brief Конструктор
		/// @param x Относительный размер по x
		/// @param y Относительный размер по y
		UDim(float x, float y);

		/// @brief Конструктор
		/// @details делает тоже самое что и UDim(int, int), только x и y берутся из точки
		/// @param p точка
		UDim(const point &p);

		/// @brief получить абсолютные значения(в пикселях) относительно окна
		/// @param Parent координаты/размер того откносительно чего раситывается
		/// @return абсолютные координаты/размер
		point GetAbsolute(const point &Parent) const;

		UDim& operator=(const UDim&) = default;

		bool operator==(const UDim &obj) const;
		bool operator!=(const UDim &obj) const;
	};

//==================================================================================================

} // namespace KolibriLib

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::point& p)
{
	return os << "X: " << p.x << " Y: " << p.y;
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UDim::Axis& p)
{
	return os << "Scale: " << p.Scale << " Offset: " << p.Offset;
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UDim& p)
{
	return os << "X: " << p.X << " Y: " << p.Y;
}


#endif // __TYPES_HPP__