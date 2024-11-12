#include <kolibriLib/UI/tab.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;

KolibriLib::UI::Tabs::Tabs(const Frame &frame, UDimArea tabsArea)
	: Frame(frame),
	  _tabsArea(tabsArea)
{
}

void KolibriLib::UI::Tabs::AddTab(const node &newTab)
{
	_tabs.push_back(newTab);
}

bool KolibriLib::UI::Tabs::DeleteTab(const node &removableTab)
{
	auto n = std::find(_tabs.begin(), _tabs.end(), removableTab);

	if(n != _tabs.end())
	{
		_tabs.erase(n);
		return true;
	}
	else
	{
		return false;
	}
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