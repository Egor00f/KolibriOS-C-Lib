#ifndef __TEXT_LABEL_HPP__
#define __TEXT_LABEL_HPP__


#include <include_ksys.h>

#include <string>

#include <kolibriLib/types.hpp>
#include <kolibriLib/UI/UI.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <kolibriLib/UI/image.hpp>
#include <kolibriLib/UI/text/text.hpp>
//#include "font.hpp"
//#include "fontslist.hpp"

//#include <kolibri_rasterworks.h>

namespace KolibriLib
{
    namespace UI
    {

        /// @brief Работа с тектсом
        namespace text
        {
            /// @brief Текстовая метка
            /// @details Простая текстовая метка, ничего особенного.
            class TextLabel: public Txt, public UI::UIElement
            {
            public:

                /// @brief Конструктор
                /// @param coord координата
                /// @param size Размер элемента, игнорируется если TextScale = false
                /// @param text Текст текста
                /// @param FontSize Размер текста
                /// @param TextScale Масштабировать текст, чтобы он не выходил за границы элемента
                /// @param Margin Отступы от границ
                TextLabel(const UDim& coord = point(0), const UDim& size = DefaultSize, const std::string& text = "TextLabel", const Size &CharSize = {8, 16}, bool TextScale = true, const Colors::Color& TextColor = Globals::SystemColors.work_text, const unsigned& Margin = 0);
             
                /// @brief конструктор
                /// @param coord координаты
                /// @param size размер
                /// @param text текст
                TextLabel(const UDim &coord, const UDim &size = DefaultSize, const Txt &text = Txt());

                /// @brief Конструктор копирования
                /// @param copy объект который будет копироваться
                TextLabel(const TextLabel &copy);

                /// @brief Отрисовать текстовую метку
                void Render() const override;

                /// @brief 
                /// @param a 
                /// @return 
                TextLabel& operator = (const TextLabel& a) = default;

                /// @brief 
                /// @param a 
                /// @return 
                bool operator == (const TextLabel& a) const;

                /**
                 * @brief Оператор сравнения
                 * @param a с чем сравнивать
                 * @return 
                 */
                bool operator != (const TextLabel& a) const;
                
                /// @brief 
                /// @param a 
                void swap(TextLabel& a);
            };
        }
    } // namespace UI
    
} // namespace KolibriLib


#endif // __TEXT_H__