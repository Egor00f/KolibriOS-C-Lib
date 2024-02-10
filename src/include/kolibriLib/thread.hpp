#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdlib.h>
#include <sys/ksys.h>

namespace KolibriLib
{
    /// \brief Работа с потоками
    namespace Thread
    {
        /// @brief Слот окна
        typedef int Slot;
        typedef int PID;

        union ThreadInfo
        {
            struct
            {
                uint32_t cpu_usage;             // CPU usage (cycles per secondgoes)
                uint16_t pos_in_window_stack;   // position of the thread window in the window stack
                uint16_t slot_num_window_stack; // slot number in window stack
                uint16_t __reserved1;           // reserved
                char name[12];                  // process/thread name
                uint32_t memstart;              // process address in memory
                uint32_t memused;               // used memory size - 1
                int pid;                        // identifier (PID/TID)
                int winx_start;                 // window x-coordinate
                int winy_start;                 // window y-coordinate
                int winx_size;                  // x-axis flow window size
                int winy_size;                  // y-axis flow window size
                uint16_t slot_state;            // thread slot state
                uint16_t __reserved2;           // reserved
                int clientx;                    // client area start coordinate x-axis
                int clienty;                    // client area start coordinate y-axis
                int clientwidth;                // client area width
                int clientheight;               // client area height
                uint8_t window_state;           // window state - bitfield
                uint8_t event_mask;             // event mask
                uint8_t key_input_mode;         // keyboard input mode
            };
            uint8_t __reserved3[KSYS_THREAD_INFO_SIZE];

            ThreadInfo& operator = (const ksys_thread_t& a);
        };

        /// \brief Создать поток
        /// \param ThreadEntry Имя функции которую нужно запустить в новом потоке
        /// @param ThreadStackSize Размер стека нового потока в байтах
        /// \return ID потока
        PID CreateThread(void(*ThreadEntry)(void*), unsigned ThreadStackSize = 1024);

        /// @brief Завершить процесс/поток
        /// @param PID ID Процесса/потока
        /// @return true если успешно, инач false
        /// @paragraph Нельзя завершить поток операционной системы OS/IDLE (номер слота 1), можно завершить любой обычный поток/процесс
        bool TerminateThread(PID pid);

        /// @brief Получить слот потока
        /// @param pid поток
        /// @return Слот потока pid
        Slot GetThreadSlot(PID pid);


        /// @brief Поличть информацию о потоке
        /// @param thread слот потока
        /// @return информация о потоке
        /// @paragraph если слот -1 то возвращается информация о текущем потоке
        ThreadInfo GetThreadInfo(Slot thread);




        
    } // namespace Thread
    
} // namespace KolibriLib


#endif // __THREAD_H__