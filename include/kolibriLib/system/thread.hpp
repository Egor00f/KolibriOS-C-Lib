#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <include_ksys.h>
#include <kolibriLib/input/keyboard.hpp>
#include <kolibriLib/types.hpp>
#include <kolibriLib/window/enums.hpp>
#include <kolibriLib/enumBitfield.hpp>
#include <type_traits>
#include <string>

namespace KolibriLib
{
    /// \brief Подождать
    /// \param time время задержки(в 1/100 секунды)
    inline void Wait(const uint32_t &time)
    {
        _ksys_delay(time);
    }

    /// \brief Подождать, минимальное кол-во времени
    /// @details Функция передаёт управление другим процессам
    inline void Wait()
    {
        _ksys_thread_yield();
    }

    /// \brief Работа с потоками
    namespace Thread
    {
        /// @brief Слот окна
        /// @details Слоты нумеруются с 1.
        using Slot = int;

        /// @brief ID процесса
        using PID = int;

        /// @brief Информация о потоке
        struct ThreadInfo
        {
            /// @brief Состояние слота
            enum class SlotState : std::uint16_t
            {
                /// @brief поток выполняется
                Running = 0,

                /// @brief поток приостановлен
                Suspended = 1,

                /// @brief поток приостановлен в момент ожидания события
                SuspendedWaitEvent = 2,

                /// @brief поток завершается в результате вызова функции -1 или насильственно как следствие вызова подфункции 2 функции 18 или завершения работы системы
                Term = 3,

                /// @brief поток завершается в результате исключения
                ExceptionTerm = 4,

                /// @brief поток ожидает события
                Waitevent = 5,

                /// @brief запрошенный слот свободен, вся остальная информация о слоте не имеет смысла
                Free = 9
            };

            /// @brief Состояние окна
            /// @details Битовое поле
            enum class WindowStatus : std::uint8_t
            {
                /// @brief Окно маскимизировано
                Maximized = 1,

                /// @brief Окно минимизировано в панель задач
                Minimized = 2,

                /// @brief Окно свёрнуто в заголовок
                CollapsedToTitle = 4
            };

            /// @brief Конструктор
            /// @param t
            ThreadInfo(const ksys_thread_t &t);

            ThreadInfo &operator=(const ThreadInfo &) = default;

            /// @brief
            /// @warning недоделан
            operator ksys_thread_t() const;

            // поля

            /// @brief имя процесса
            /// @details имя запущенного файла - исполняемый файл без расширения
            std::string name;

            /// @brief Координаты окна
            Coord WindowCoord;

            /// @brief Размер окна
            Size WindowSize;

            /// @brief Координаты клиентской области
            Coord ClientCoord;

            /// @brief Размер клиентской области
            Size ClientSize;

            /// @brief Использование Процессора
            /// @details тактов процессора
            uint32_t cpu_usage;

            /// @brief Слот окна
            Slot num_window_stack;

            /// @brief адрес процесса в памяти
            uint32_t memstart;

            /// @brief размер используемой памяти
            uint32_t memused;

            /// @brief идентификатор (PID/TID)
            PID pid;

            /// @brief
            SlotState slot_state;

            /// @brief Позиция окна в оконном стеке
            window::Pos pos_in_window_stack;

            /// @brief Состояние окна
            /// @note Битовое поле
            WindowStatus window_state;

            /// @brief маска ивентов
            uint8_t event_mask;

            /// @brief Режим ввода клавиатуры
            keyboard::InputMode key_input_mode;
        };

        /// @brief Значение PID текущего процесса.
        /// @details Нужно для функций
        const PID ThisThread = KSYS_THIS_SLOT;

        /// \brief Создать поток
        /// \param ThreadEntry указатель на функцию которую нужно запустить в новом потоке
        /// @param ThreadStackSize Размер стека нового потока в байтах
        /// \return ID потока
        /// @note есть немалый шанс проебаться с размером стека
        PID CreateThread_(void *ThreadEntry, std::size_t ThreadStackSize = 8192);

        template <class T>
        /// \brief Создать поток
        /// \param ThreadEntry указатель на функцию которую нужно запустить в новом потоке
        /// @param ThreadStackSize Размер стека нового потока в байтах
        /// \return ID потока
        /// @note есть шанс проебаться с размером стека
        inline PID CreateThread(T ThreadEntry, std::size_t ThreadStackSize = 8192)
        {
            static_assert(::std::is_pointer<T>::value, "Only pointers!");
            return CreateThread_(reinterpret_cast<void *>(ThreadEntry), ThreadStackSize);
        }

        /// @brief Завершить процесс/поток
        /// @param PID ID Процесса/потока
        /// @return false если произошла ошибка
        /// @note Нельзя завершить поток операционной системы OS/IDLE (номер слота 1), можно завершить любой обычный поток/процесс
        inline bool TerminateThread(PID pid)
        {
            int ret;

            asm_inline(
                "int $0x40"
                : "=a"(ret)
                : "a"(18), "b"(18), "c"(pid));

            return ret;
        }

        /// @brief Завершить процесс/поток
        /// @return false если произошла ошибка
        inline bool TerminateThread()
        {
            _ksys_exit();
            return false; // 'no return statement in function returning non-void [-Wreturn-type]' очень бесит
        }

        /// @brief Получить информацию о потоке
        /// @param thread слот потока
        /// @return информация о потоке
        /// @details по умолчанию возвращается информация о текущем потоке
        ThreadInfo GetThreadInfo(const Slot &thread = ThisThread);

        /// @brief Получить информацию о потоке
        /// @param thread слот потока
        /// @return информация о потоке
        /// @details по умолчанию возвращается информация о текущем потоке
        ThreadInfo GetThreadInfo(const Slot &thread, int &ec);

        /// @brief Получить информацию о потоке
        /// @param info Ссылка на структуру для данных
        /// @param thread слот потока
        void GetThreadInfo(ThreadInfo &info, Slot thread = ThisThread);

        /// @brief Получить PID текущего процесса(тот в котором была вызвана эта функция)
        /// @details Обычно бесполезно ведь вы можете использовать ThisThread
        /// @return PID текущего процесса
        inline PID GetThisThreadPID()
        {
            return GetThreadInfo().pid;
        }

        /// @brief Получить слот потока
        /// @param pid поток, по умолчанию поток который вызывает эту функцию
        /// @return Слот потока pid
        inline Slot GetThreadSlot(PID pid)
        {
            return _ksys_get_thread_slot(pid);
        }

        /// @brief Получить слот потока
        /// @return Слот потока pid
        inline Slot GetThreadSlot()
        {
            return GetThreadInfo().num_window_stack;
        }

        /// @brief Кривой всратый и тупящий мьютекс, который существует только потому что я не разобрался в фьютексах
        /// @details Реально лучше не используйте это убожество. оно сильно тормозит систему
        /// @details использует подфункцию 1 функции 68
        class Mutex
        {
        public:
            void lock()
            {
                _locked = true;
            }

            void unlock()
            {
                _locked = false;
            }

            void waitPoint()
            {
                while (_locked)
                    Wait();
            }

        private:
            bool _locked = false;
        };

    } // namespace Thread

} // namespace KolibriLib

template <>
struct is_flag<KolibriLib::Thread::ThreadInfo::WindowStatus> : std::true_type
{
};

#endif // __THREAD_HPP__