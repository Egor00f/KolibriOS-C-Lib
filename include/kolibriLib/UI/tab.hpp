#ifndef __TAB_HPP__
#define __TAB_HPP__

#include <kolibriLib/UI/text/textlabel.hpp>

namespace KolibriLib
{
	namespace UI
	{
		/**
		 * @brief Вкладки
		 * @warning Не готово вообще
		 */
		class Tabs
		{
		public:
			using tab = std::shared_ptr<Frame>;
			using label = std::shared_ptr<text::TextLabel>;
			using node = std::pair<tab, label>;


			Tabs(UDimArea labelsArea, UDimArea tabsArea)
				: _labelsArea(labelsArea),
				  _tabsArea(tabsArea)
			{
				
			}

			Tabs(const Tabs&) = default;

			

		private:

			UDimArea _labelsArea;
			UDimArea _tabsArea;

			std::vector<node> tabs;
		};
	} // namespace UI
	
} // namespace KolibriLib


#endif // __TAB_HPP__
