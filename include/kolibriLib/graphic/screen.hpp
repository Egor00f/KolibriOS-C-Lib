#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include <include_ksys.h>

#include <kolibriLib/types.hpp>
#include <kolibriLib/system/thread.hpp>

namespace KolibriLib
{
	/// @brief Получить разрешение экрана
	/// @return размер экрана
	inline Size GetScreenSize()
	{
		return Size(_ksys_screen_size());
	}

	/// @brief Получить принадлежность точки
	/// @param POINT точка на экране
	/// @return слот окна которому принадлежит точка
	inline Thread::Slot ScreenPointAffiliation(Coord POINT)
	{
		Thread::Slot s;

		asm_inline(
			"int $0x40"
			: "=a"(s)
			: "a"(34), "b"(POINT.x), "c"(POINT.y));

		return s;
	}

	/**
	 * @brief Работа с экраном
	 */
	namespace Screen
	{
		/**
		 * @brief Применить настройки экрана
		 * @details Перерисовывает экран после вызова функций SetButtonStyle и SetSystemColors
		 */
		inline void ApplySettings()
		{
			asm_inline(
				"int $0x40" ::"a"(48), "b"(0), "c"(0));
		}

		/**
		 * @brief Получить рабочую область экрана
		 * @details Рабочая область экрана определяет положение и координаты максимизированного окна
		 * @return Рабочая область экрана
		 */
		inline AbsArea GetWorkArea()
		{
			ksys_pos_t coords, topAndBottom;

			asm_inline(
				"int $0x40"
				: "=a"(coords), "=b"(topAndBottom)
				: "a"(48), "b"(5));

			return AbsArea(coords, Coord(topAndBottom) - coords);
		}

	} // namespace Screen

} // namespace KolibriLib

#endif // __SCREEN_HPP__