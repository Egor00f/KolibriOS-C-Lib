#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include <include_ksys.h>
#include <kolibriLib/types.hpp>
#include <kolibriLib/filesystem/filesystem.hpp>
#include <kolibriLib/color.hpp>

namespace KolibriLib
{
    /// @brief Работа с мышью
    namespace mouse
    {
		/// @brief Список кодов кнопок мыши
		typedef enum MouseButtons
		{
			/// @brief ЛКМ
			LeftButton = 0b00001,

			/// @brief ПКМ
			RightButton = 0b00010,

			/// @brief Центральная кнопка (та что под колёсиком)
			CenterButton = 0b00100,

			/// @brief Кнопка 4
			FourButton = 0b01000,

			/// @brief Кнопка 5
			FiveButton = 0b10000
		} MouseButtons;

		/// @brief Ивенты мыши
		typedef enum MouseEvents
		{
			/// @brief удерживается ЛКМ
			heldLeftButton = 0b00001,

			/// @brief удерживается ПКМ
			heldRightButton = 0b00010,

			/// @brief удерживается Центральная кнопка (та что под колёсиком)
			heldCenterButton = 0b00100,

			/// @brief удерживается Кнопка 4
			heldFourButton = 0b01000,

			/// @brief удерживается Кнопка 5
			heldFiveButton = 0b10000,

			/// @brief удерживается ЛКМ
			PressedLeftButton = 0b100000000,

			/// @brief удерживается ПКМ
			PressedRightButton = 0b1000000000,

			/// @brief удерживается Центральная кнопка (та что под колёсиком)
			PressedCenterButton = 0b10000000000,

			/// @brief отпущена ЛКМ
			ReleasedLeftButton = 0b10000000000000000,

			/// @brief отпущена ПКМ
			ReleasedRightButton = 0b100000000000000000,

			/// @brief отпущена Центральная кнопка (та что под колёсиком)
			ReleasedCenterButton = 0b1000000000000000000,

			/// @brief используется вертикальная прокрутка
			Vertical = 0b1000000000000000,

			/// @brief используется горизонтальная прокрутка
			HorizonTal = 0b100000000000000000000000,

			/// @brief двойной щелчок левой кнопкой мыши
			DoubleClickLeftButton = 0b1000000000000000000000000
		} MouseEvents;

		/// @brief Получить позиция курсора на экране
        /// @return (point) позиция курсора абсолютно
		inline Coord GetMousePositionOnScreen()
		{
			return Coord(_ksys_get_mouse_pos(KSYS_MOUSE_SCREEN_POS));
		}

		/// @brief Получить позиция курсора внутри окна
        /// @return (point) позиция курсора относительно окна
		inline Coord GetMousePositionInWindow()
		{
			return Coord(_ksys_get_mouse_pos(KSYS_MOUSE_WINDOW_POS));
		}

		/// @brief Проверить какие кнопки мыши нажаты
		/// @return состояния кнопок мыши из MouseButtons
		inline MouseButtons GetMouseButtons()
		{
			return static_cast<MouseButtons>(_ksys_get_mouse_buttons());
		}

		/// @brief Получить состояния и события кнопок мыши
		/// @return состояния кнопок мыши и ивенты из MouseEvents
		inline MouseEvents GetMouseEvents()
		{
			return static_cast<MouseEvents>(_ksys_get_mouse_eventstate());
		}

		/// @brief колёсико мыши
        /// @return
		inline uint32_t GetMouseWheels()
		{
			return _ksys_get_mouse_wheels();
		}

		/// @brief Эмулировать нажатия кнопок мыши
		/// @param m занчения из списка MouseButtons
		inline void EmulateMouse(uint8_t m)
		{
			asm_inline (
				"int $0x40" 
				::"a"(18), "b"(19), "c"(5), "d"(m)
			);
		}

		/// @brief Центрировать курсор
		/// @details Функция устанавливает курсор в середину экрана
		inline void CenterCursor()
		{
			asm_inline(
				"int $0x40" 
				::"a"(18), "b"(15)
			);
		}

		/// @brief Хендл курсора мыши
		typedef void* CursorHandle;

		/// @brief Загрузить курсор мыши
		/// @param path путь до файла с курсором
		/// @return Хендл загруженного курсора
		/// @paragraph Файл курсора должен быть в формате .cur, стандартном для MS Windows, причём размером 32*32 пикселя
		inline CursorHandle LoadCursor(const filesystem::Path& path)
		{
			return _ksys_load_cursor (
				const_cast<void *>(static_cast<const void *>(path.c_str())), 
				KSYS_CURSOR_FROM_FILE
			);
		}

		/// @brief Загрузить курсор мыши
		/// @param path путь до файла с курсором
		/// @param hotPoint координаты "горячей точки" курсора
		/// @return Хендл загруженного курсора
		/// @note Файл курсора должен быть в формате .cur, стандартном для MS Windows, причём размером 32*32 пикселя
		inline CursorHandle LoadCursor(const filesystem::Path& path, const Coord &hotPoint)
		{
			return _ksys_load_cursor (
				const_cast<void *>(static_cast<const void *>(path.c_str())), 
				KSYS_CURSOR_FROM_FILE || 2 || (hotPoint.x << 23) || (hotPoint.y << 16)
			);
		}

		/// @brief Загрузить курсор мыши
		/// @param ptr Указатель на изображение
		/// @return Хендл загруженного курсора
		/// @note Файл курсора должен быть в формате .cur, стандартном для MS Windows, причём размером 32*32 пикселя
		inline CursorHandle LoadCursor(void *ptr)
		{
			return _ksys_load_cursor(ptr, KSYS_CURSOR_FROM_MEM);
		}

		/// @brief Загрузить курсор мыши
		/// @param ptr Указатель на изображение
		/// @param hotPoint координаты "горячей точки" курсора
		/// @return Хендл загруженного курсора
		inline CursorHandle LoadCursor(void* ptr, const Coord &hotPoint)
		{
			return _ksys_load_cursor (
				ptr,
				KSYS_CURSOR_FROM_MEM || 2 || (hotPoint.x << 23) || (hotPoint.y << 16));
		}

		// /// @brief Загрузить курсор мыши
		// /// @param cur путь до файла с курсором
		// /// @return Хендл загруженного курсора
		// /// @paragraph Файл курсора должен быть в формате .cur, стандартном для MS Windows, причём размером 32*32 пикселя
		// inline CursorHandle LoadCursor(Colors::ARGB[] cur);

		/// @brief Установить курсор
		/// @param handle хендл устанавливаемого курсора
		/// @return хендл предыдушего курсора
		inline CursorHandle SetCursor(CursorHandle handle)
		{
			return _ksys_set_cursor(handle);
		}

		/// @brief Удалить курсор
		/// @param Хендл удаляемого курсора
		inline void DeleteCursor(CursorHandle handle)
		{
			_ksys_delete_cursor(handle);
		}
	}
} // namespace KolibriLib


#endif // __MOUSE_HPP__