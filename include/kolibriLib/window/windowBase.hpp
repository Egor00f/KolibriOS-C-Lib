#ifndef __WINDOW_BASE_HPP__
#define __WINDOW_BASE_HPP__

/*
	Тут тип базовый функционал работы с окном
*/

#include <include_ksys.h>
#include <string>

#include <kolibriLib/types.hpp>
#include <kolibriLib/system/thread.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/globals.hpp>
#include <input.hpp>
#include "enums.hpp"

namespace KolibriLib
{

	/// @brief Работа с окном
	namespace window
	{
		/// @brief Размер окна по умолчанию
		const Size DefaultWindowSize = {600, 400};

		/// @brief Координаты окна по умолчанию
		const Coord DefaultWindowCoord = {100, 100};

		/// @brief Объявить окно
		/// @param coord Координаты окна(его левого верхнего угла) на экране
		/// @param size Размеры окна
		/// @param title Заголовок окна
		/// @param WorkColor цвет рабочей области окна
		/// @param TitleColor Цвет текста заголовка
		/// @param style Стиль окна
		/// @details при size == {0,0} размер окна будет равен размеру экрана
		/// @note Положение и размеры окна устанавливаются при первом вызове этой функции и игнорируются при последующих; для изменения положения и/или размеров уже созданного окна используйте ChangeWindow
		/// @note Окно должно умещаться на экране. Если переданные координаты и размеры не удовлетворяют этому условию, то соответствующая координата (или, возможно, обе) считается нулем, а если и это не помогает, то соответствующий размер (или, возможно, оба) устанавливается в размер экрана
		inline void CreateWindow(const Coord &coord = DefaultWindowCoord,
								 const Size &size = DefaultWindowSize,
								 const std::string &title = "Window",
								 const Colors::Color &WorkColor = Globals::SystemColors.work_area,
								 Colors::Color TitleColor = Globals::SystemColors.work_text,
								 WindowStyle style = WindowStyle::withSkin,
								 WindowSettings settings = WindowSettings::WindowHaveTitle | WindowSettings::RelativeCoord)
		{

			asm_inline(
				"int $0x40" ::
				"a"(0),
				"b"(coord.x << 16 | ((size.x - 1) & 0xFFFF)),
				"c"(coord.y << 16 | ((size.y - 1) & 0xFFFF)),
				"d"(((static_cast<std::uint16_t>(settings) << 28) | (static_cast<std::uint8_t>(style) << 24)) | (static_cast<ksys_color_t>(WorkColor) & 0xFFFFFF)),
				"S"((static_cast<std::uint16_t>(settings) >> 8) | (static_cast<ksys_color_t>(TitleColor) & 0xFFFFFF)),
				"D"(title.c_str())
				: "memory");
		}

		/// @brief Снять фокус с окна
		/// @param slot слот окна
		inline void UnfocusWindow(const Thread::Slot &slot = Thread::GetThreadSlot())
		{
			_ksys_unfocus_window(slot);
		}

		/// @brief Получить размер окна
		/// @param pid PID процесса который создал окно
		/// @return Размер окна
		inline Size GetWindowSize(Thread::Slot pid = Thread::ThisThread)
		{
			return Thread::GetThreadInfo(pid).WindowSize;
		}

		/// @brief Поставить фокус на окно
		/// @param slot слот окна
		inline void FocusWindow(const Thread::Slot &slot = Thread::GetThreadSlot())
		{
			_ksys_focus_window(slot);
		}

		/// @brief Получить слот активного окна
		/// @return Слот активного в данный момент окна
		/// @details Активное окно - окно, находящееся на вершине оконного стека, оно получает сообщения о вводе с клавиатуры. Для него позиция в оконном стеке совпадает с возвращаемым значением.
		inline Thread::Slot GetActiveWindow()
		{
			Thread::Slot s;

			asm_inline(
				"int $0x40"
				: "=a"(s)
				: "a"(18), "b"(7));

			return s;
		}

		/// @brief Свернуть окно (окно текущего потока)
		inline void MinimizeWindow()
		{
			asm_inline(
				"int $0x40" ::"a"(18), "b"(10));
		}

		/// @brief Свернуть все окна
		inline void MinimizeAllWindows()
		{
			asm_inline(
				"int $0x40" ::"a"(18), "b"(23));
		}

		/// @brief получить координаты окна
		/// @param pid PID процесса, который создал окно
		/// @return Координаты окна
		/// @note Если поток еще не определил свое окно вызовом функции 0, то положение и размеры этого окна полагаются нулями
		inline Coord GetWindowCoord(Thread::PID pid = Thread::ThisThread)
		{
			return Thread::GetThreadInfo(Thread::GetThreadSlot(pid)).WindowSize;
		}

		/// @brief Получить положение окна относительно других окон
		/// @return одна из констант из списка Pos
		inline Pos GetWindowPos()
		{
			Pos a;

			asm_inline(
				"int $0x40"
				: "=a"(a)
				: "a"(18), "b"(25), "c"(1));

			return a;
		}

		/// @brief Изменить положение окна относительно других окон
		/// @param pos значение из списка Pos
		/// @param pid процесс окна, по умолчанию текущий
		/// @return false если ошибка
		/// @return true если успешно
		inline bool SetWindowPos(Pos pos, Thread::PID pid = Thread::ThisThread)
		{
			bool ret;

			asm_inline(
				"int $0x40"
				: "=a"(ret)
				: "a"(18), "b"(25), "c"(2), "d"(pid), "S"(pos));

			return ret;
		}

		/// @brief Изменить размер и координаты окна
		/// @param coord новые координаты окна
		/// @param size новый размер окна
		/// @note работает для окна, которое было создано тем потоком, в котором эту функцию запускают
		inline void ChangeWindow(const Coord &coord, const Size &size)
		{
			_ksys_change_window(coord.x, coord.y, size.x, size.y);
		}

		/// @brief Получить высоту скина(заголовка окна)
		/// @return высота скина
		inline unsigned GetSkinHeight()
		{
			return _ksys_get_skin_height();
		}

		/// @brief Получить размер рабочей области окна
		/// @return Размер рабочей области окна
		inline Size GetWorkArea()
		{
			ksys_pos_t a, b;

			asm_inline(
				"int $0x40"
				: "=a"(a), "=b"(b)
				: "a"(48), "b"(5));

			return Size(a.y - a.x + 1, b.y - b.x + 1);
		}

	} // namespace window

} // namespace KolibriLib

#endif // __WINDOW_BASE_HPP__