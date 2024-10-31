#include <kolibriLib/UI/buttons/buttonsBase.hpp>
#include <kolibriLib/debug.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

UI::buttons::ButtonsIDController *Globals::DefaultButtonsIDController = nullptr;

ButtonID buttons::GetFreeButtonId(ButtonIDList &ButtonsIdList, std::uint32_t startID)
{
	logger << microlog::LogLevel::Debug << "GetFreeButtonID" << std::endl;

	for (ButtonID i = startID; i < buttons::ButtonIDEnd; i.value++) // в wiki сказано что id в промежутке (0, 0x8000)
	{																// CloseButton = 1, поэтому пропускаем и начинаем сразу с 2
		if (std::find(ButtonsIdList.begin(), ButtonsIdList.end(), i) == ButtonsIdList.end())
		{
			ButtonsIdList.push_back(i);
			return i;
		}
	}

	return ButtonIDNotSet;
}

bool KolibriLib::UI::buttons::FreeButtonId(const ButtonID &id, ButtonIDList &ButtonsIdList)
{
	auto iter = std::find(ButtonsIdList.begin(), ButtonsIdList.end(), id);

	if (!(iter == ButtonsIdList.end()))
	{
		ButtonsIdList.erase(iter);
		return true;
	}
	else
	{
		return false;
	}
}

ButtonID buttons::autoDefineButton(ButtonIDList &ButtonsIdList, const Coord &coords, const Size &size, const Colors::Color &color)
{
	ButtonID id = GetFreeButtonId(ButtonsIdList); // Автоматически получаем id для кнопки

	DefineButton(coords, size, id, color);

	return id;
}

/*
	ButtonsIDController
*/

ButtonID buttons::ButtonsIDController::GetFreeButtonID(std::weak_ptr<BaseButton> ptr)
{
	ButtonIDList buttonsList = ListoButtonIDList(ButtonsIdList);

	ButtonID ret;

	for (std::int8_t i = 0; i < 2; i++) // пробуем 2 раза
	{									// 3 раза уже нету смысла
		ret = GetFreeButtonId(buttonsList, _top);

		if (ret.CheckIsValid())
		{
			_top = ret.value + 1;

			ButtonsIDController::node n(buttonsList.back());

			if (!ptr.expired())
				n.pointers.push_back(ptr);

			ButtonsIdList.push_back(n);

			break;
		}
		else // Если не удалось найти свободный ID
		{	 // то начинаем с начала
			_top = StartTop;
		}
	};

	return ret;
}

void buttons::ButtonsIDController::FreeButtonID(const ButtonID &id)
{
	logger << microlog::LogLevel::Debug << "ButtonsController::FreeButtonID" << std::endl;

	ButtonIDList buttonslist = ListoButtonIDList(ButtonsIdList);

	auto iter = std::find(buttonslist.begin(), buttonslist.end(), id);

	if (iter != buttonslist.end())
	{
		if (id.value == _top - 1)
			_top--;

		const std::size_t index = std::distance(buttonslist.begin(), iter);

		ButtonsIdList.erase(ButtonsIdList.begin() + index);
	}
	else
	{
		logger << microlog::LogLevel::Warning << "ButtonID not found" << std::endl;
	}
}

ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList()
{
	return ButtonsIdList;
}

const ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList() const
{
	return ButtonsIdList;
}

std::vector<std::weak_ptr<BaseButton>> KolibriLib::UI::buttons::ButtonsIDController::GetPoinerToButton(const ButtonID &ID) const
{
	std::vector<std::weak_ptr<BaseButton>> ret;

	std::find_if(ButtonsIdList.begin(), ButtonsIdList.end(),
				 [&ID, &ret](const ButtonsIDController::node &n)
				 {
					 if (n.id == ID)
					 {
						 ret = n.pointers;
						 return true;
					 }
					 return false;
				 });

	return ret;
}

ButtonIDList KolibriLib::UI::buttons::ButtonsIDController::ListoButtonIDList(const List &list)
{
	ButtonIDList ret;
	ret.reserve(list.size());

	for (ButtonsIDController::node i : list)
	{
		ret.push_back(i.id);
	}

	return ret;
}

void KolibriLib::UI::buttons::ButtonsIDController::TakeupButtonID(const ButtonID &id, std::weak_ptr<BaseButton> ptr)
{
	logger << microlog::LogLevel::Debug << "TakeupButtonID" << std::endl;

	ButtonIDList buttonslist = ListoButtonIDList(ButtonsIdList);

	auto it = std::find(buttonslist.begin(), buttonslist.end(), id);

	if (it == buttonslist.end())
	{
		ButtonsIdList.push_back(node(id, ptr));
	}
	else
	{
		const std::size_t i = it - buttonslist.begin();
		ButtonsIdList[i].pointers.push_back(ptr);
	}
}

void KolibriLib::UI::buttons::ButtonsIDController::Sort()
{
	std::vector<ButtonID> IDList;

	IDList.reserve(ButtonsIdList.size());

	for (auto i : ButtonsIdList)
	{
		std::size_t expired = 0;
		for (auto j : i.pointers)
		{
			if (j.expired())
			{
				expired++;
				break;
			}
		}

		if (expired == i.pointers.size())
		{
		}
	}
}

/*
	ButtonsIDController::node
*/

KolibriLib::UI::buttons::ButtonsIDController::node::node(ButtonID Id)
	: id(Id)
{
}

KolibriLib::UI::buttons::ButtonsIDController::node::node(ButtonID Id, std::weak_ptr<BaseButton> p)
	: id(Id),
	  pointers({p})
{
}

bool KolibriLib::UI::buttons::ButtonsIDController::node::operator==(const node &val) const
{
	return id == val.id;
}

bool KolibriLib::UI::buttons::ButtonsIDController::node::operator!=(const node &val) const
{
	return id != val.id;
}

/*
	ButtonID
*/

KolibriLib::UI::buttons::ButtonID::ButtonID(unsigned val)
	: value(val)
{
}

bool KolibriLib::UI::buttons::ButtonID::CheckIsValid() const
{
	return value != ButtonIDNotSet.value && value < ButtonIDEnd.value;
}

ButtonID KolibriLib::UI::buttons::ButtonID::operator=(const unsigned &val)
{
	return ButtonID(val);
}

KolibriLib::UI::buttons::ButtonID::operator std::uint32_t() const
{
	return value;
}

bool KolibriLib::UI::buttons::ButtonID::operator==(const ButtonID &val) const
{
	return value == val.value;
}

bool KolibriLib::UI::buttons::ButtonID::operator!=(const ButtonID &val) const
{
	return value != val.value;
}

void KolibriLib::UI::buttons::ButtonID::swap(ButtonID &val)
{
	std::uint32_t buff = value;

	value = val.value;
	val.value = buff;
}
