#include <kolibriLib/UI/buttons/buttonsBase.hpp>
#include <kolibriLib/debug.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

ButtonID buttons::GetFreeButtonId(ButtonIDList &ButtonsIdList, std::uint32_t startID)
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "GetFreeButtonID";
#endif

	for (ButtonID i = startID; i < buttons::ButtonIDEnd; i.value++) // в wiki сказано что id в промежутке (0, 0x8000)
	{																// CloseButton = 1, поэтому пропускаем и начинаем сразу с 2
		if (std::find(ButtonsIdList.begin(), ButtonsIdList.end(), i) == ButtonsIdList.end())
		{
			ButtonsIdList.push_back(i);

#ifndef NO_LOGS

#ifdef VERBOSE
			logger << "OK";
#endif

			logger << std::endl;
#endif

			return i;
		}
	}

#ifndef NO_LOGS
	logger << microlog::LogLevel::Warning << "Free ID not found" << std::endl;
#endif

	return ButtonIDNotSet;
}

bool KolibriLib::UI::buttons::FreeButtonId(ButtonIDList &ButtonsIdList, const ButtonID &id)
{
	assert(id.operator bool());
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "GetFreeButtonID";
#endif

	auto iter = std::find(ButtonsIdList.begin(), ButtonsIdList.end(), id);

	if (iter != ButtonsIdList.end())
	{
		ButtonsIdList.erase(iter);

		return true;
	}
	else
	{

#ifndef NO_LOGS
		logger << microlog::LogLevel::Warning << "ID not found in ButtonsIDList" << std::endl;
#endif

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
