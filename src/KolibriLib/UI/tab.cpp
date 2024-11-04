#include <kolibriLib/UI/tab.hpp>

using namespace KolibriLib;
using namespace UI;

KolibriLib::UI::Tabs::Tabs(const Frame &frame, UDimArea tabsArea)
	: Frame(frame),
	  _tabsArea(tabsArea)
{
}

void Tabs::Render() const
{
	_tabs[_activeTab].second->Render();
}

bool Tabs::OnButtonEvent(buttons::ButtonID PressedButtonID)
{
	for (std::size_t i = 0; i < _tabs.size(); i++)
	{
		if (_tabs[i].first->GetId() == PressedButtonID)
		{
			_activeTab = i;
			break;
		}
	}

	return false;
}

Tabs::node KolibriLib::UI::Tabs::GetNode(std::size_t index) const
{
	return _tabs.at(index);
}