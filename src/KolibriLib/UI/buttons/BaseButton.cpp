#include <kolibriLib/UI/buttons/button.hpp>
#include <kolibriLib/globals.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

BaseButton::BaseButton()
{
#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor" << std::endl;
#endif
#endif

	if (KolibriLib::Globals::DefaultButtonsIDController != nullptr)
	{
		_ButtonsIDController = KolibriLib::Globals::DefaultButtonsIDController;

		_id = KolibriLib::Globals::DefaultButtonsIDController->GetFreeButtonID(this);
	}

#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor: done" << std::endl;
#endif
#endif
}

BaseButton::BaseButton(ButtonID id)
{
#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor" << std::endl;
#endif
#endif

	if (KolibriLib::Globals::DefaultButtonsIDController != nullptr)
	{
		_ButtonsIDController = KolibriLib::Globals::DefaultButtonsIDController;
		SetId(id);
	}

#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor: done" << std::endl;
#endif
#endif
}

KolibriLib::UI::buttons::BaseButton::BaseButton(const BaseButton &button)
	: _ButtonsIDController(button._ButtonsIDController),
	  _type(button._type)
{
#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor(copy)" << std::endl;
#endif
#endif

	SetId(button._id);

#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton constructor: done" << std::endl;
#endif
#endif
}

buttons::BaseButton::~BaseButton()
{
#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton destructor" << std::endl;
#endif
#endif

	Deactivate();

#ifndef NO_LOGS
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "BaseButton destructor: done" << std::endl;
#endif
#endif
}

void BaseButton::BaseButton::Deactivate()
{
	if (IsActive())
	{
		if (_ButtonsIDController != nullptr)
		{
			_ButtonsIDController->FreeButtonID(_id, this);
		}

		_id = buttons::ButtonIDNotSet;
	}
	else
	{
#ifndef NO_LOGS
		logger << microlog::LogLevel::Warning << "BaseButton already not active" << std::endl;
#endif
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
#ifndef NO_LOGS
		logger << microlog::LogLevel::Warning << "BaseButton already active" << std::endl;
#endif
	}
}

bool KolibriLib::UI::buttons::BaseButton::IsActive() const
{
#ifdef DEBUG
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Button is ";
	if (_id == ButtonIDNotSet)
		logger << "not";

	logger << " active" << std::endl;
#endif
#endif

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
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "SetId(ButtonID)" << std::endl;
#endif

	_id = NewID;

	if (_ButtonsIDController != nullptr)
	{
		_ButtonsIDController->TakeUpButtonID(NewID, this);
	}
}

void KolibriLib::UI::buttons::BaseButton::SetId()
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "SetId()" << std::endl;
#endif

	if (_ButtonsIDController != nullptr)
	{
		if (IsActive())
			_ButtonsIDController->FreeButtonID(_id, this);

		_id = _ButtonsIDController->GetFreeButtonID(this);
	}
	else
	{
		_id = ButtonIDNotSet;
	}
}

void BaseButton::Define(const Coord &coord, const Size &size, const Colors::Color &color) const
{
	DefineButton(coord, size, _id, color);
}
