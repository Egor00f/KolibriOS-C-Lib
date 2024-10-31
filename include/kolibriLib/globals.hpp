#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include <kolibriLib/color.hpp>
#include <kolibriLib/system/os.hpp>

namespace KolibriLib
{
	/// @brief Список глобальных переменных
	namespace Globals
	{
		/// @brief Таблица систменых цветов
		/// @details По умолчанию в этой переменной реально системные цвета, но вы можете это изменить ()
		extern Colors::ColorsTable SystemColors;

	} // namespace Globals
	
} // namespace KolibriLib

#endif // __GLOBALS_HPP__