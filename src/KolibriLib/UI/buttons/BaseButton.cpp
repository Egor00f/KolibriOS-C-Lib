#include <kolibriLib/UI/buttons/button.hpp>
#include <kolibriLib/globals.hpp>
#include <assert.h>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

BaseButton::BaseButton()
{
	if (KolibriLib::Globals::DefaultButtonsIDController != nullptr)
	{
		_ButtonsIDController = KolibriLib::Globals::DefaultButtonsIDController;

		std::shared_ptr<BaseButton> s_ptr(this);

		_id = KolibriLib::Globals::DefaultButtonsIDController->GetFreeButtonID(s_ptr);
	}
}

BaseButton::BaseButton(ButtonID id)
{
	if (KolibriLib::Globals::DefaultButtonsIDController != nullptr)
	{
		_ButtonsIDController = KolibriLib::Globals::DefaultButtonsIDController;
	}

	SetId(id);
}

KolibriLib::UI::buttons::BaseButton::BaseButton(const BaseButton &button)
	: _ButtonsIDController(button._ButtonsIDController),
	  _type(button._type)
{
	SetId(button._id);
}

buttons::BaseButton::~BaseButton()
{
	Deactivate();
}

void BaseButton::BaseButton::Deactivate()
{
	if (IsActive())
	{
		if (_ButtonsIDController != nullptr)
		{
			_ButtonsIDController->FreeButtonID(_id);
		}

		_id = buttons::ButtonIDNotSet;
	}
	else
	{
		logger << microlog::LogLevel::Warning << "BaseButton already not active" << std::endl;
	}
}

void BaseButton::BaseButton::Activate()
{
	if (!IsActive())
	{
		SetId();
	}
	else
	{
		logger << microlog::LogLevel::Warning << "BaseButton already active" << std::endl;
	}
}

bool KolibriLib::UI::buttons::BaseButton::IsActive() const
{
	return _id != ButtonIDNotSet;
}

bool buttons::BaseButton::GetStatus() const
{
	return _status;
}

buttons::ButtonID buttons::BaseButton::GetId() const
{
	return _id;
}

void KolibriLib::UI::buttons::BaseButton::SetId(const ButtonID &NewID)
{
	logger << microlog::LogLevel::Debug << "SetId(ButtonID)" << std::endl;

	_id = NewID;

	if (_ButtonsIDController != nullptr)
	{
		std::shared_ptr<BaseButton> s_ptr(this);
		_ButtonsIDController->TakeUpButtonID(NewID, s_ptr);
	}
}

void KolibriLib::UI::buttons::BaseButton::SetId()
{
	logger << microlog::LogLevel::Debug << "SetId()" << std::endl;

	if (_ButtonsIDController != nullptr)
	{
		if (IsActive())
			_ButtonsIDController->FreeButtonID(_id);

		std::shared_ptr<BaseButton> s_ptr(this);
		_id = _ButtonsIDController->GetFreeButtonID(s_ptr);
	}
	else
	{
		_id = ButtonIDNotSet;
	}
}

void BaseButton::Define(const Coord &coord, const Size &size, const Colors::Color &color) const
{
	assert(_id == ButtonIDNotSet || _id >= ButtonIDEnd && "Wrong button id!");
	DefineButton(coord, size, _id, color);
}
