#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <include_ksys.h>
#include <kolibriLib/debug.hpp>

namespace KolibriLib
{
    /// @brief Набор функций для работы с цветом
    namespace Colors
    {        
		/// @brief rgb цвет
		/// @details крутая оболочка для rgb_t из sys/ksys.h
        struct rgb: public rgb_t
        {
            rgb(rgb_t val);

            rgb(ksys_color_t val);

			/// @brief Конструктор
			/// @param r красная состовляющаяя
			/// @param g зелёная
            rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);

			/// @brief конструктор по умолчанию
            rgb() = default;
            rgb(const rgb&) = default;

            rgb& operator = (const rgb&) = default;

			bool operator == (const rgb& color) const;
			bool operator != (const rgb& color) const;

            operator ksys_color_t() const;

			/// @brief Получить цвет в формате BBGGRR
			/// @return цвет в формате BBGGRR
            std::uint32_t BBGGRR() const;
			
			/// @brief Получить цвет в формате BBGGRR
			/// @return цвет в формате BBGGRR
			std::uint32_t RRGGBB() const;

			/// @brief Получить цвет в формате BBGGRR00
			/// @return BBGGRR00
            std::uint32_t BBGGRR00() const;
			
			/// @brief Получить цвет в формате RRGGBB00
			/// @return цвет в формате RRGGBB00
			std::uint32_t RRGGBB00() const;
			
			/// @brief Получить цвет в формате 00RRGGBB
			/// @return цвет в формате RRGGBB00
			std::uint32_t ZeroRRGGBB() const;
        };

        /// @brief Цвет
        union Color
        {
			/// @brief Конструктор по умолчанию
            Color() = default;
			
			///Color(const Color *a);
            Color(const Color &a) = default;

            /// @brief Конструктор
            /// @param a 
            Color(const ksys_color_t& a);
			
			/// @brief Конструктор
            Color(const rgb_t &color);

            /// @brief Конструктор
            /// @param R Красная состовляющаяя цвета
            /// @param G Красная состовляющаяя цвета
            /// @param B Красная состовляющаяя цвета
            /// @param A Прозрачность
            Color(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A = 0xFF);

            operator rgb_t() const;
            operator ksys_color_t() const;
            operator rgb() const;

            /// @brief Получить rgb_t
            /// @return 
            rgb_t GetRGB() const;

            Color& operator = (const Color& a) = default;

            Color& operator = (const std::uint32_t& color);

            /// @brief Смешивает два цвета (среднее занчение)
            /// @param a второй цвет
            /// @return 
            Color &operator + (const Color &a);

            bool operator == (const Color& a) const;

            bool operator != (const Color& a) const;

			// Поля

            ksys_color_t val;

            struct
            {
                std::uint8_t blue;
                std::uint8_t green;
                std::uint8_t red;

                /// @brief Альфа канал
                std::uint8_t _a;
            };
        };

        /// @brief Конвертирует rgb_t в ksys_color_t
        /// @param color 
        /// @return 
        ksys_color_t RGBtoINT(const rgb_t &color);
        
        /// @brief 
        /// @param color 
        /// @return 
        rgb_t UINT32toRGB(const std::uint32_t &color);

        /// @brief Смешать два цвета
        /// @param a Первый цвет
        /// @param b Второй цвет
        /// @param k Коофициент, чем он больше, тем больше цвета a, чем меньше, тем больше цвета b
        /// @return получившийся в итоге цвет
        Color BlendColors(const Color &a, const Color &b, float k = 0.5f);


        /// @brief Таблица системных цветов
        struct ColorsTable : public ksys_colors_table_t
        {
            ColorsTable() = default;
            ColorsTable(const ColorsTable&) = default;
            ColorsTable(ColorsTable&&) = default;

            /// @brief Конструктор
            /// @param frameArea 
            /// @param grabBar 
            /// @param grabBarButton 
            /// @param grabText 
            /// @param workArea цвет рабочей области окна
            /// @param workButton 
            /// @param workButtonText 
            /// @param workText цвет текста
            /// @param workGraph 
            ColorsTable (
                Color frameArea,
                Color grabBar,
                Color grabBarButton,
                Color grabText,
                Color workArea,
                Color workButton,
                Color workButtonText,
                Color workText,
                Color workGraph
            );

            ColorsTable& operator = (const ColorsTable&) = default;
        };

        /// @brief Таблица цветов по умолчанию
        const ColorsTable DefaultColorTable = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    void PrintDebug(Colors::Color out);


    void PrintDebug(Colors::ColorsTable out);
    
} // namespace KolibriLib


#endif // __COLOR_H__