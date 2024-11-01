#include <kolibriLib/UI/buttons/button.hpp>
#include <kolibriLib/globals.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

BaseButton::BaseButton()
{
	#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "BaseButton Constructor" << std::endl;
	#endif

	if (KolibriLib::Globals::DefaultButtonsIDController != nullptr)
	{
		_ButtonsIDController = KolibriLib::Globals::DefaultButtonsIDController;

		std::shared_ptr<BaseButton> s_ptr(this);

		_id = KolibriLib::Globals::DefaultButtonsIDController->GetFreeButtonID(s_ptr);
	}
}

BaseButton::BaseButton(ButtonID id)
{
	#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "BaseButton Constructor" << std::endl;
	#endif

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
	#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "BaseButton Constructor" << std::endl;
	#endif

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
		std::shared_ptr<BaseButton> s_ptr(this);
		_ButtonsIDController->TakeUpButtonID(NewID, s_ptr);
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
	DefineButton(coord, size, _id, color);
}
