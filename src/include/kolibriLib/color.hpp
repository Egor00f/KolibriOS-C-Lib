#ifndef __COLOR_H__
#define __COLOR_H__

#include <sys/ksys.h>
#include <kolibriLib/types.hpp>

namespace KolibriLib
{
    /// @brief Набор функций для работы с цветом
    namespace Colors
    {
        /// @brief Тип данных для работы с argb цветом
        struct ARGB
        {
            uint8_t alpha;
            uint8_t blue;
            uint8_t green;
            uint8_t red;
        };
        
        /// @brief Цвет
        union Color
        {
            ksys_color_t val;
            struct
            {
                uint8_t _a;
                uint8_t blue;
                uint8_t green;
                uint8_t red;
            };
            
            /// @brief Конструктор
            /// @param a 
            Color(const ksys_color_t& a = 0);
            Color(const Color &a);
            Color(const ARGB &a);

            Color& operator = (const Color& a);

            /// @brief Смешивает два цвета (среднее занчение)
            /// @param a 
            /// @return 
            Color &operator+(const Color &a);

            bool operator == (const Color& a) const;

            bool operator != (const Color& a)const;
        };
        
        /// @brief Смешать два цвета
        /// @param a Первый цвет
        /// @param b Второй цвет
        /// @param k Коофициент, чем он больше, тем больше цвета a, чем меньше, тем больше цвета b
        /// @return получившийся в итоге цвет
        Color BlendColors(const Color &a, const Color &b, const float k = 0.5)
        {
            Color buff;
            buff._a = (a._a * static_cast<int>(k)) + (b._a * (1 - static_cast<int>(k)));
            buff.red = (a.red * static_cast<int>(k)) + (b.red * (1 - static_cast<int>(k)));
            buff.green = (a.green * static_cast<int>(k)) + (b.green * (1 - static_cast<int>(k)));
            buff.blue = (a.blue * static_cast<int>(k)) + (b.blue * (1 - static_cast<int>(k)));
            return buff;
        }

        /// @brief Таблица цветов по умолчанию
        const ksys_colors_table_t DefaultColorTable = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        /// @brief Таблица цветов
        struct ColorsTable : public ksys_colors_table_t
        {
            ColorsTable(ksys_colors_table_t table = DefaultColorTable);
            
            bool operator == (const ColorsTable &table) const;
            bool operator != (const ColorsTable &table) const;
        };

        KolibriLib::Colors::Color::Color(const ksys_color_t &a)
        {
            val = a;
        }

        KolibriLib::Colors::Color::Color(const Color &a)
        {
            val = a.val;
        }

        KolibriLib::Colors::Color::Color(const ARGB &a)
        {
            _a = a.alpha;
            red = a.red;
            green = a.green;
            blue = a.blue;
        }

        Color &KolibriLib::Colors::Color::operator=(const Color &a)
        {
            val = a.val;
            return *this;
        }

        Color &KolibriLib::Colors::Color::operator+(const Color &a)
        {
            _a /= 2;
            _a += a._a / 2;
            red /= 2;
            red += (a.red / 2);
            green /= 2;
            green += (a.green / 2);
            blue /= 2;
            blue += (a.blue / 2);
            return *this;
        }

        bool KolibriLib::Colors::Color::operator==(const Color &a) const
        {
            return val == a.val;
        }

        bool KolibriLib::Colors::Color::operator!=(const Color &a) const
        {
            return val != a.val;
        }

        KolibriLib::Colors::ColorsTable::ColorsTable(ksys_colors_table_t table)
        {
            work_area = table.work_area;
            work_button = table.work_button;
            work_button_text = table.work_button_text;
            work_graph = table.work_graph;
            work_text = table.work_text;
        }

        bool KolibriLib::Colors::ColorsTable::operator==(const ColorsTable &table) const
        {
            return (frame_area == table.frame_area) &&
                   (grab_bar == table.grab_bar) &&
                   (grab_bar_button == table.grab_bar_button) &&
                   (grab_button_text == table.grab_button_text) &&
                   (work_area == table.work_area) &&
                   (work_button == table.work_button) &&
                   (work_button_text == table.work_button_text) &&
                   (work_graph == table.work_graph) &&
                   (work_text == table.work_text);
        }

        bool KolibriLib::Colors::ColorsTable::operator!=(const ColorsTable &table) const
        {
            return (frame_area != table.frame_area) ||
                   (grab_bar != table.grab_bar) ||
                   (grab_bar_button != table.grab_bar_button) ||
                   (grab_button_text != table.grab_button_text) ||
                   (work_area != table.work_area) ||
                   (work_button != table.work_button) ||
                   (work_button_text != table.work_button_text);
        }
    }
} // namespace KolibriLib


#endif // __COLOR_H__