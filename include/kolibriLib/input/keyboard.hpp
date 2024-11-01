#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include <include_ksys.h>
#include <cstdint>

namespace KolibriLib
{
    /**
     * @brief Работа с клавиатурой
     */
    namespace keyboard
    {
        /// @brief Режим ввода
        enum class InputMode : std::uint8_t
        {
            /// @brief используются ASCII символы
            ASCII = KSYS_KEY_INPUT_MODE_ASCII,

            /// @brief используются сканкоды
            SCANCODES = KSYS_KEY_INPUT_MODE_SCANC
        };

        // Хоспади автор ksys.h зачем такие страшные и длинные шифры, нипанятна же ничего, хотя там же C так что понятна, а у меня namespacы
        /// @brief Список управляющих клавиш
        enum class ControlKey
        {
            /// @brief Левый SHIFT
            LeftShift = KSYS_CONTROL_LSHIFT,
            RightShift = KSYS_CONTROL_RSHIFT,
            LeftCtrl = KSYS_CONTROL_LCTRL,
            RightCtrl = KSYS_CONTROL_RCTRL,
            LeftAlt = KSYS_CONTROL_LALT,
            RightAlt = KSYS_CONTROL_RALT,
            CapsLock = KSYS_CONTROL_CAPS,
            NumLock = KSYS_CONTROL_NUM_LOCK,
            ScrollLock = KSYS_CONTROL_SCROLL_LOCK
        };

        /// @brief Режимы раскладки клавиатуры
        /// @details Если нажат Alt, то используется раскладка с Alt;
        /// если не нажат Alt, но нажат Shift, то используется раскладка с Shift;
        /// если не нажаты Alt и Shift, но нажат Ctrl, то используется нормальная раскладка, после чего из кода вычитается 0x60;
        /// если не нажата ни одна из управляющих клавиш, то используется нормальная раскладка.
        enum KeyboardLayoutMode : std::uint8_t
        {
            /// @brief Нормальная раскладка
            Normal = 0,

            /// @brief Раскладка с нажатым Shftом
            Shift = 1,

            /// @brief Раскладка с зажатым Alt
            Alt = 2
        };

        /// @brief Список скапкодов
        enum class Scancodes : std::uint8_t
        {
            /// @brief равен нулю
            ANY = static_cast<std::uint8_t>(KSYS_SCANCODE_ANY),
            ESC = KSYS_SCANCODE_ESC,
            NUMBER_0 = KSYS_SCANCODE_0,
            NUMBER_1 = KSYS_SCANCODE_1,
            NUMBER_2 = KSYS_SCANCODE_2,
            NUMBER_3 = KSYS_SCANCODE_3,
            NUMBER_4 = KSYS_SCANCODE_4,
            NUMBER_5 = KSYS_SCANCODE_5,
            NUMBER_6 = KSYS_SCANCODE_7,
            NUMBER_7 = KSYS_SCANCODE_7,
            NUMBER_8 = KSYS_SCANCODE_8,
            NUMBER_9 = KSYS_SCANCODE_9,
            Key_A = KSYS_SCANCODE_A,
            Key_B = KSYS_SCANCODE_B,
            Key_C = KSYS_SCANCODE_C,
            Key_D = KSYS_SCANCODE_D,
            Key_E = KSYS_SCANCODE_E,
            Key_F = KSYS_SCANCODE_F,
            Key_G = KSYS_SCANCODE_G,
            Key_H = KSYS_SCANCODE_H,
            Key_J = KSYS_SCANCODE_J,
            Key_K = KSYS_SCANCODE_K,
            Key_L = KSYS_SCANCODE_L,
            Key_M = KSYS_SCANCODE_M,
            Key_N = KSYS_SCANCODE_N,
            Key_O = KSYS_SCANCODE_O,
            Key_P = KSYS_SCANCODE_P,
            Key_R = KSYS_SCANCODE_R,
            Key_S = KSYS_SCANCODE_S,
            Key_T = KSYS_SCANCODE_T,
            Key_I = KSYS_SCANCODE_I,
            Key_U = KSYS_SCANCODE_U,
            Key_W = KSYS_SCANCODE_W,
            Key_X = KSYS_SCANCODE_X,
            Key_Y = KSYS_SCANCODE_Y,
            Key_Z = KSYS_SCANCODE_Z,
            F1 = KSYS_SCANCODE_F1,
            F2 = KSYS_SCANCODE_F2,
            F3 = KSYS_SCANCODE_F3,
            F4 = KSYS_SCANCODE_F4,
            F5 = KSYS_SCANCODE_F5,
            F6 = KSYS_SCANCODE_F6,
            F7 = KSYS_SCANCODE_F7,
            F8 = KSYS_SCANCODE_F8,
            F9 = KSYS_SCANCODE_F9,
            F10 = KSYS_SCANCODE_F10,
            SCRLOCK = KSYS_SCANCODE_SCRLOCK,
            BACKSLASH = KSYS_SCANCODE_BACKSLASH,

            /// @brief Пробел
            SPACE = KSYS_SCANCODE_SPACE,
            NUMLOCK = KSYS_SCANCODE_NUMLOCK,
            NUMPAD_0 = KSYS_SCANCODE_NUMPAD_0,
            NUMPAD_1 = KSYS_SCANCODE_NUMPAD_1,
            NUMPAD_2 = KSYS_SCANCODE_NUMPAD_2,
            NUMPAD_3 = KSYS_SCANCODE_NUMPAD_3,
            NUMPAD_4 = KSYS_SCANCODE_NUMPAD_4,
            NUMPAD_5 = KSYS_SCANCODE_NUMPAD_5,
            NUMPAD_6 = KSYS_SCANCODE_NUMPAD_6,
            NUMPAD_7 = KSYS_SCANCODE_NUMPAD_7,
            NUMPAD_8 = KSYS_SCANCODE_NUMPAD_8,
            NUMPAD_9 = KSYS_SCANCODE_NUMPAD_9,

            /// @brief знак "-" на нампаде
            NUMPAD_MINUS = KSYS_SCANCODE_NUMPAD_MINUS,

            /// @brief знак "+" на нампаде
            NUMPAD_PLUS = KSYS_SCANCODE_NUMPAD_PLUS,

            HOME = KSYS_SCANCODE_EXT_HOME,
            END = KSYS_SCANCODE_EXT_END,

            /// @brief Запятая
            NUMPAD_COMMA = KSYS_SCANCODE_NUMPAD_COMMA,

            BACKSPACE = KSYS_SCANCODE_BACKSPACE
        };

        /// @brief Сканкод
        class Scancode
        {
        public:
            Scancode() = default;
            Scancode(const Scancode&) = default;
            Scancode(char c);
            Scancode(Scancodes v);

            operator char() const;
            operator std::uint8_t() const;
            operator Scancodes() const;

            Scancode& operator=(const Scancode&) = default;
            bool operator==(const Scancode& val);
            bool operator!=(const Scancode& val);
        private:
            Scancodes val;
        };

        /// @brief Ввод с клавиутры
        /// @details В зависимости от режима возваржаются ASCII или Сканкоды. 
        /// По умолчанию ASCII
        /// Сканкоды возвращаются непосредственно клавиатурой и фиксированы.
        /// ASCII-коды получаются с использованием таблиц преобразования, которые можно установить
        union Input
        {
            char ASCII;
            Scancode scancode;

            /// @brief конструктор
            /// @details он просто есть
            Input(uint8_t i = 0) : ASCII(static_cast<char>(i)) {}

			Input& operator=(const Input&) = default;

			/// @brief Получить значение 
			operator std::uint8_t() const;
            operator char() const;
            operator Scancode() const;
        };

        /// @brief Раскладка клавиатуры
        /// @details Представляет из себя массив ASCII символов, где номер элемента массива равен сканкоду
        /// @details Тоесть KeyboardLayout[3] это ASCII код сканкода 3
        class KeyboardLayout
        {
        public:
            KeyboardLayout &operator=(const KeyboardLayout &) = default;

            char &operator[](std::uint8_t i);
            char operator[](std::uint8_t i) const;

        private:
            char table[128];
        };

        struct FullKeyboardLayout
        {
            KeyboardLayout normal, shift, alt;
        };

        /// @brief Проверить какая клавиша клавиатуры нажата
        /// @return
        inline Input CheckKeyboard()
        {
            return Input(_ksys_get_key().code);
        }

        /// @brief Изменить режим ввода
        /// @param mode режим ввода
        /// @details При вызове этой функции приложением с неактивным окном считается, что буфер нажатых клавиш пуст
        inline void SetInputMode(InputMode mode)
        {
            _ksys_set_key_input_mode(static_cast<ksys_key_input_mode_t>(mode));
        }

        /// @brief Получить режим ввода
        /// @return режим ввода. см. InputMode
        inline InputMode GetInputMode()
        {
            InputMode ret;

            asm_inline(
                "int $0x40"
                : "=a"(ret)
                : "a"(66), "b"(2));

            return ret;
        }

        /**
         * @brief Получить раскладку клавиатуры
         * @param ret Указатель на раскладку клавиатуры
         * @param mode Режим раскладки клавиатуры
         * @return Указатель на раскладку клавиатуры
         */
        inline KeyboardLayout* GetKeyboardLayoutPointer(KeyboardLayout* ret, KeyboardLayoutMode mode = KeyboardLayoutMode::Normal)
        {
            asm_inline(
                "int $0x40" 
                :: "a"(26), "b"(2), "c"(mode), "d"(ret));

            return ret;
        }

        /// @brief Получит раскладку клавиатуры
        /// @param mode Режим раскладки клавиатуры
        /// @return Раскладка клавиатуры
        inline KeyboardLayout GetKeyboardLayout(KeyboardLayoutMode mode = KeyboardLayoutMode::Normal)
        {
            KeyboardLayout ret;

            GetKeyboardLayoutPointer(&ret, mode);

            return ret;
        }

        /// @brief Изменить раскладку клавиатуры
        /// @param layout Раскладка
        /// @param mode Режим раскладки
        /// @return true, если параметр задан неверно
        inline bool SetKeyboardLayout(KeyboardLayout layout, KeyboardLayoutMode mode)
        {
            bool ret;

            asm_inline(
                "int $0x40"
                : "=a"(ret)
                : "a"(21), "b"(2), "c"(mode), "d"(layout));

            return ret;
        }

        /// @brief Получить состояние управляющих клавиш
        /// @return Возвращает имя контрольной клавиши из списка ControlKeys
        inline ControlKey GetControlKey()
        {
            return static_cast<ControlKey>(_ksys_get_control_key_state());
        }

        /// @brief Установить "горячую клавишу"
        /// @param controlKeys клавиши
        /// @return true если успешно, иначе false
        /// @warning not done!
        inline bool InstallHotKey(Scancode scancode, uint16_t controlKeys)
        {
            return !_ksys_set_sys_hotkey(scancode, controlKeys);
        }

        /// @brief Удалить "горячую клавишу"
        /// @return true если успешно, иначе false
        /// @warning not done!
        inline bool DeleteHotKey(Scancode scancode, uint16_t controlKeys)
        {
            return _ksys_del_sys_hotkey(scancode, controlKeys);
        }

        /// @brief Получить сканкод по ASCII символу
        /// @param ascii символ
        /// @param mode Режим раскладки клавиатуры
        /// @return Сканкод
        /// @return Если символ не был найден в раскладке клавиатуры, возвращается Scancode::ANY
        Scancode GetScancodeByASCII(char ascii, KeyboardLayoutMode mode);

        /// @brief Получить сканкод по ASCII символу
        /// @param ascii символ
        /// @return Сканкод
        /// @return Если символ не был найден в раскладке клавиатуры, возвращается Scancode::ANY
        Scancode GetScancodeByASCII(char ascii);

    } // namespace keyboard

} // namespace KolibriLib

#endif // __KEYBOARD_HPP__