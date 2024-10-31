#ifndef __IMAGEBUTTON_HPP__
#define __IMAGEBUTTON_HPP__

#include "button.hpp"
#include <kolibriLib/img.hpp>
#include "baseButton.hpp"

namespace KolibriLib
{
	namespace UI
	{
		namespace buttons
		{
			/**
			 * @brief Кнопка-изображение
			 */
			class ImageButton : public Button, public Images::img
			{
			public:
				/// @brief Отрисовать изображение
				void Render() const override;

				ImageButton &operator=(const ImageButton &) = default;

				/**
				 * @brief
				 * @param val
				 * @return
				 */
				bool operator==(const ImageButton &val) const;

				/**
				 * @brief
				 * @param val
				 * @return
				 */
				bool operator!=(const ImageButton &val) const;

			private:
			};
		}
	}
}

#endif // __IMAGEBUTTON_HPP__