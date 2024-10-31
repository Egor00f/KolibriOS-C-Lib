#ifndef __OS_HPP__
#define __OS_HPP__

#include <kolibriLib/color.hpp>
#include "thread.hpp"
#include <kolibriLib/filesystem/filesystem.hpp>

#include <vector>

namespace KolibriLib
{
	/// @brief Работа с системой
	namespace OS
	{

		/// @brief Перечисление всех ивентов
		enum class Event
		{
			/// @brief Ивента не было
			/// @details Имеет смысл для функций Event WaitEvent(TimeOut) и CheckEvent()
			None = KSYS_EVENT_NONE,

			/// @brief Перерисовка окна
			Redraw = KSYS_EVENT_REDRAW,

			/// @brief Активность со стороны клавиатуры
			Key = KSYS_EVENT_KEY,

			/// @brief Нажата кнопка
			Button = KSYS_EVENT_BUTTON,

			/// @brief Ивент от экрана
			Desktop = KSYS_EVENT_DESKTOP,

			/// @brief Активность мыши
			Mouse = KSYS_EVENT_MOUSE,

			/// @brief Программу открыли в дебагере
			Debug = KSYS_EVENT_DEBUG,

			/// @brief Выход
			/// @details Можно получить только от window::Window::Handler().
			/// означает что была нажата кнопка X (эта та что закрывает окно)
			Exit
		};

		/// @brief Список языков системы
		enum class lang
		{
			/// @brief Английский
			Eng = 0,

			/// @brief Финский
			Fi = 1,

			/// @brief Немецкий
			Ger = 2,

			/// @brief Русский
			Rus = 3
		};

		/// @brief коды ошибок для функций SetTime, SetDayOfWeek, SetAlarm
		enum class SetTimeOrDate
		{
			/// @brief успешно
			Successfully = 0,

			/// @brief параметр задан неверно
			WrongArgs = 1,

			/// @brief CMOS-батарейки разрядились
			CMOS = 2
		};

		/// @brief Битовые флаги для маски ивентов
		enum Mask
		{
			/**
			 * @brief
			 */
			RedrawEvent = KSYS_EVM_REDRAW,

			/**
			 * @brief
			 */
			KeyEvent = KSYS_EVM_KEY,

			/**
			 * @brief
			 */
			ButtonEvent = KSYS_EVM_BUTTON,

			/**
			 * @brief
			 */
			MouseEvent = KSYS_EVM_MOUSE,

			/**
			 * @brief
			 */
			DescktopEvent = KSYS_EVM_BACKGROUND,

			/// @brief неактивное окно получает события от мыши
			MouseEventInInactiveWindow = KSYS_EVM_MOUSE_FILTER,

			/// @brief если курсор за пределами окна
			MouseCursorInWindow = KSYS_EVM_CURSOR_FILTER,

			/**
			 * @brief
			 */
			AllMouseEvents = (MouseEvent | MouseEventInInactiveWindow | MouseCursorInWindow),

			/// @brief маска по умолчанию
			/// @details разрешены ивенты перерисовки, клавиатуры и кнопок
			DefaultEventMask = 0b111
		};

		/// @brief Версия ядра
		struct CoreVersion
		{
			union
			{
				/// @brief Версия
				std::uint32_t version;
				struct
				{
					std::uint8_t a;
					std::uint8_t b;
					std::uint8_t c;
					std::uint8_t d;
				};
			};

			/// @brief Ревизия
			std::uint16_t Rev;

			CoreVersion &operator=(const CoreVersion &) = default;

			/**
			 * @brief оператор сравнения
			 * @param ver с чем сравнивать
			 * @return true если равны, иначе false
			 */
			bool operator==(const CoreVersion &ver) const;

			/**
			 * @brief оператор сравнения
			 * @param ver с чем сравнивать
			 * @return false если равны, иначе true
			 */
			bool operator!=(const CoreVersion &ver) const;
		};

		/// @brief Получить системные цвета
		/// @return Таблица системных цветов
		inline Colors::ColorsTable GetSystemColors()
		{
			Colors::ColorsTable a;

			_ksys_get_system_colors(&a);

			return a;
		}

		/// @brief Изменить системные цвета
		/// @param Указатель на таблицу системных цветов
		inline void SetSystemColors(Colors::ColorsTable *table)
		{
			asm_inline(
				"int $0x40" ::"a"(48), "b"(2), "c"(table), "d"(sizeof(Colors::ColorsTable)));
		}

		/// \brief Ждать ивента
		/// \return Ивент
		inline Event WaitEvent()
		{
			return static_cast<Event>(_ksys_wait_event());
		}

		/// \brief Ждать ивента
		/// \param Таймаут (в 1/100 секунды), время сколько ждать
		/// \return Ивент
		inline Event WaitEvent(const uint32_t &TimeOut)
		{
			return static_cast<Event>(_ksys_wait_event_timeout(TimeOut));
		}

		/// \brief Проверить пришёл ли ли ивент
		/// \return Ивент
		inline Event CheckEvent()
		{
			return static_cast<Event>(_ksys_check_event());
		}

		/// @brief Изменить маску ивентов
		/// @param mask маска
		/// @return прошлая маска
		inline uint32_t SetEventMask(uint32_t mask)
		{
			return _ksys_set_event_mask(mask);
		}

		/// @brief Получить маску ивентов
		/// @return
		inline uint32_t GetEventMask()
		{
			uint32_t ret = SetEventMask(0);
			SetEventMask(ret);
			return ret;
		}

		/**
		 * \brief Запустить программу
		 * \param AppName Полное имя исполняемого файла
		 * \param args аргументы. Максимум 256 символов
		 * @param ec ошибка файловой систмеы
		 * @param debug режим дебага
		 * \return PID запущенной программы
		 * @return -1 если произошла ошибка
		 */
		Thread::PID Exec(const filesystem::Path &AppName, const std::string &args, filesystem::FilesystemErrors &ec, bool debug = false) noexcept;

		/**
		 * @brief Запустить программу
		 * @param AppName Полное имя исполняемого файла
		 * @param args аргументы. Максимум 256 символов
		 * @param debug режим дебага
		 * \return PID запущенной программы
		 * @return -1 если произошла ошибка
		 * @throw Код ошибки файловой системы
		 */
		Thread::PID Exec(const filesystem::Path &AppName, const std::string &args, bool debug = false);

		/**
		 * @brief Установить системную  время
		 * @param NewTime Время что будет установленно
		 * @return Код ошибки
		 */
		inline SetTimeOrDate SetTime(ksys_time_bcd_t NewTime)
		{
			SetTimeOrDate ret = SetTimeOrDate::Successfully;

			asm_inline(
				"int $0x40"
				: "=a"(ret)
				: "a"(22), "b"(0), "c"(NewTime));

			return ret;
		}

		/// @brief Установить системную  дату
		/// @param NewData Дата что будет установленна
		inline SetTimeOrDate SetDate(ksys_date_bcd_t NewDate)
		{
			SetTimeOrDate ret = SetTimeOrDate::Successfully;

			asm_inline(
				"int $0x40"
				: "=a"(ret)
				: "a"(22), "b"(1), "c"(NewDate));

			return ret;
		}

		/// @brief Установить день недели
		/// @param NewDayOfWeek День недели от 1 до 7
		/// @note Ценность установки дня недели представляется сомнительной, поскольку он мало где используется(день недели можно рассчитать по дате)
		inline SetTimeOrDate SetDayOfWeek(uint8_t NewDayOfWeek)
		{
			SetTimeOrDate ret = SetTimeOrDate::Successfully;

			asm_inline(
				"int $0x40"
				: "=a"(ret)
				: "a"(22), "b"(2), "c"(NewDayOfWeek));

			return ret;
		}

		/// @brief Установить будильник
		/// @param AlarmTime Время будильника
		/// @details Будильник можно установить на срабатывание в заданное время каждые сутки. При этом отключить его существующими системными функциями нельзя.
		/// @details Срабатывание будильника заключается в генерации IRQ8.
		/// @details Будильник - глобальный системный ресурс; установка будильника автоматически отменяет предыдущую установку
		inline SetTimeOrDate SetAlarm(ksys_time_bcd_t AlarmTime)
		{
			SetTimeOrDate ret = SetTimeOrDate::Successfully;

			asm_inline(
				"int $0x40"
				: "=a"(ret)
				: "a"(22), "b"(3), "c"(AlarmTime));

			return ret;
		}

		/// @brief получить кол-во свободной памяти
		/// @return размер свободной памяти в килобайтах
		inline std::size_t GetFreeMem()
		{
			std::size_t a;

			asm_inline(
				"int $0x40"
				: "=a"(a)
				: "a"(18), "b"(16));

			return a;
		}

		/// @brief Получить кол-во всей памяти
		/// @return Размер всей памяти в килобайтах
		inline std::size_t AllMem()
		{
			std::size_t a;

			asm_inline(
				"int $0x40"
				: "=a"(a)
				: "a"(18), "b"(17));

			return a;
		}

		/// @brief Получить язык системы
		/// @return Значение из списка lang
		inline lang GetLang()
		{
			int a;

			asm_inline(
				"int $0x40"
				: "=a"(a)
				: "a"(26), "b"(5));

			return static_cast<lang>(a);
		}

		/// @brief Изменить язык системы
		/// @param l язык
		inline void SetLang(lang l)
		{
			asm_inline(
				"int $0x40" ::"a"(21), "b"(5), "c"(l));
		}

		/// @brief Получить версию ядра
		/// @return Указатель на структуру версии ядра
		CoreVersion GetCoreVersion();

		/// @brief Получить тактовую частоту процессора
		/// @return тактовая частота (по модулю 2^32 тактов = 4ГГц)
		inline uint32_t GetCPUClock()
		{
			return _ksys_get_cpu_clock();
		}

		/**
		 * @brief получить значение высокоточного счётчика времени
		 * @note функция использует счётчик HPET, если HPET не доступен используется счётчик PIT. В этом случае точность будет уменьшена до 10 000 00 наносекунд.
		 * @return число наносекунд с момента загрузки ядра
		 * @details Сомнительно использовать это. Используйте лучше функции из станартной библиотеки Си
		 */
		inline std::uint64_t GetHighPrecisionTimerCount()
		{
			std::uint32_t a, b;

			asm_inline(
				"int $0x40"
				: "=a"(a), "=d"(b)
				: "a"(26), "b"(10));

			return static_cast<std::uint64_t>((b << 31) | a);
		}

	} // namespace OS

} // namespace KolibriLib

#endif // __OS_H__