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
		
		asm_inline (
			"int $0x40"
			: "=a"(s)
			: "a"(34), "b"(POINT.x), "c"(POINT.y)
		);
		
		return s;
	} 
} // namespace KolibriLib


#endif // __SCREEN_HPP__