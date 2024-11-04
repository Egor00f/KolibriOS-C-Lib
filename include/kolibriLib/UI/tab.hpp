#ifndef __TAB_HPP__
#define __TAB_HPP__

#include <kolibriLib/UI/text/textlabel.hpp>
#include <kolibriLib/UI/buttons/button.hpp>

namespace KolibriLib
{
	namespace UI
	{
		/**
		 * @brief Вкладки
		 * @warning Не готово вообще
		 */
		class Tabs : public Frame
		{
		public:
			/**
			 * @brief Пара из указателя на вкладку(first) и на её содержимое(second)
			 */
			using node = std::pair<std::shared_ptr<buttons::Button>, std::shared_ptr<Frame>>;


			Tabs(const Frame& frame, UDimArea tabsArea);

			Tabs(const Tabs&) = default;

			void Render() const override;

			bool OnButtonEvent(buttons::ButtonID PressedButtonID) override;

			node GetNode(std::size_t index) const;

		private:
			/**
			 * @brief Список пар вкладка и её содержимое
			 */
			std::vector<node> _tabs;

			/**
			 * @brief Область в которую вписывается содержимое вкладок
			 */
			UDimArea _tabsArea;

			/**
			 * @brief Цвет выбранной вкладки
			 */
			Colors::Color _selectedTabColor;

			/**
			 * @brief Индекс выбранной сейчас вкладки
			 */
			std::size_t _activeTab;
		};
	} // namespace UI
	
} // namespace KolibriLib


#endif // __TAB_HPP__
