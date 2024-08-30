#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <sys/ksys.h>
#include <kolibriLib/types.hpp>
#include <kolibriLib/system/filesystem.hpp>
#include <kolibriLib/color.hpp>

namespace KolibriLib
{
    /// @brief Работа с мышью
    namespace mouse
    {
		/// @brief Список кодов кнопок мыши
		enum MouseButtons
		{
			/// @brief ЛКМ
            LeftButton	= 0b00001,

            /// @brief ПКМ
            RightButton	= 0b00010,

            /// @brief Центральная кнопка (та что под колёсиком)
            CenterButton	= 0b00100,

			/// @brief Кнопка 4
            FourButton	= 0b01000,

            /// @brief Кнопка 5
            FiveButton	= 0b10000
        };
		
        /// @brief Получить позицияю курсора на экране
        /// @return (point) позиция курсора абсолютно
		inline Coord GetMousePositionOnSreen()
		{
			ksys_pos_t a = _ksys_get_mouse_pos(KSYS_MOUSE_SCREEN_POS);

			return Coord(a.x, a.y);
		}

		/// @brief Получить позицияю курсора внутри окна
        /// @return (point) позиция курсора относительно окна
		inline Coord GetMousePositionInWindow()
		{
			ksys_pos_t a = _ksys_get_mouse_pos(KSYS_MOUSE_WINDOW_POS);

			return Coord(a.x, a.y);
		}

		/// @brief Проверить какие кнопки мыши нажаты
		/// @return значения из списка MouseButtons
		inline uint32_t GetMouseButtons()
		{
			return _ksys_get_mouse_buttons();
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
			asm_inline(
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
			return _ksys_load_cursor((void*)(path.operator const char *()), KSYS_CURSOR_FROM_FILE);
		}

		/// @brief Загрузить курсор мыши
		/// @param ptr Указатель на изображение
		/// @return Хендл загруженного курсора
		/// @note Файл курсора должен быть в формате .cur, стандартном для MS Windows, причём размером 32*32 пикселя
		inline CursorHandle LoadCursor(void *ptr)
		{
			return _ksys_load_cursor(ptr, KSYS_CURSOR_FROM_MEM);
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


#endif // __MOUSE_H__