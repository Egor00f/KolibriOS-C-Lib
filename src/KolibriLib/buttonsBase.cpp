#include <kolibriLib/UI/buttons/buttonsBase.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

UI::buttons::ButtonsIDController *Globals::DefaultButtonsIDController;

ButtonID buttons::GetFreeButtonId(std::vector<ButtonID> *ButtonsIdList)
{
	for(ButtonID i = 2; i < buttons::ButtonIDEnd; i.value++)	// в wiki сказанно что id в промежутке (0, 0x8000)
	{                                   	// CloseButton = 1, поэтому пропускаем и начинаем сразу с 2
		if(std::find(ButtonsIdList->begin(), ButtonsIdList->end(), i) == ButtonsIdList->end())
		{
			ButtonsIdList->push_back(i);
			return i;
		}
	}
	
	return ButtonIDNotSet;
}

void buttons::FreeButtonId(const ButtonID &id, std::vector<ButtonID> *ButtonsIdList)
{
	auto a = std::find(ButtonsIdList->begin(), ButtonsIdList->end(), id);
	if(!(a == ButtonsIdList->end()))
	{
		ButtonsIdList->erase(a);
	}
}

ButtonID buttons::autoDefineButton(std::vector<ButtonID>*ButtonsIdList, const Coord &coords, const Size &size, const Colors::Color &color)
{
	ButtonID id = GetFreeButtonId(ButtonsIdList); // Автоматически получаем id для кнопки
	DefineButton(coords, size, id, color);
	return id;
}

ButtonID buttons::ButtonsIDController::GetFreeButtonID()
{
	return GetFreeButtonId(&ButtonsIdList);
}


void buttons::ButtonsIDController::FreeButtonID(const ButtonID &id)
{
	FreeButtonId(id, &ButtonsIdList);
}

std::vector<ButtonID> * buttons::ButtonsIDController::GetButtonsIDList()
{
	return &ButtonsIdList;
}

const std::vector<ButtonID> * buttons::ButtonsIDController::GetButtonsIDList() const
{
	return &ButtonsIdList;
}

void KolibriLib::PrintDebug(UI::buttons::ButtonID out)
{
	if(out.CheckIsValid())
	{
		if (out.value == ButtonIDNotSet)
		{
			DebugOut("Not set");
		}
		else if (out.value >= ButtonIDEnd)
		{
			DebugOut("Out of acceptable values");
		}
		else
		{
			DebugOut("WTF?");
		}
	}
	else
	{
		PrintDebug((unsigned)out);
	}
}

KolibriLib::UI::buttons::ButtonID::ButtonID(unsigned val)
	:	value(val)
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

KolibriLib::UI::buttons::ButtonID::operator uint32_t() const
{
	return value;
}

