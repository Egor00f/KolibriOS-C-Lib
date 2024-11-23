#include <kolibriLib/UI/buttons/ButtonsIDController.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

UI::buttons::ButtonsIDController *Globals::DefaultButtonsIDController = nullptr;

ButtonID buttons::ButtonsIDController::GetFreeButtonID(BaseButton *ptr)
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::GetFreeButtonID";
#endif

	/*
		В общем че происходит:
		проходимся по всем возможным ID кнопок с _top до конца. Если находим несоответствие, то возвращаем id
		иначе повторяем всё ещё раз, но начиная с начала списка.
		можно оптимизировать чтобы полностью по всем элементам _ButtonsIdList не проходиться
	*/

	auto find = [&ptr](ButtonID &top, List &ButtonsIdList)
	{
		for (ButtonID i = top; i < ButtonIDEnd; i.value++)
		{
			if (std::find_if(
					ButtonsIdList.begin(), ButtonsIdList.end(),
					[&i](const node &n)
					{
						return n.first == i;
					}) == ButtonsIdList.end())
			{
				ButtonsIdList.push_back({i, {ptr}});

				top = i + 1U;

				return i;
			}
		}

		return ButtonIDNotSet;
	};

	ButtonID ret = find(_top, _ButtonsIdList);

	if (ret == ButtonIDNotSet) // Пытаемся получить ID ещё раз
	{
		_top = _StartTop; // Но уже с самого начала
		ret = find(_top, _ButtonsIdList);
	}

#ifndef NO_LOGS
#ifdef VERBOSE
	logger << ": return ID: " << ret;
#endif
	logger << std::endl;
#endif

	if (ret == ButtonIDNotSet)
		logger << microlog::LogLevel::Warning << "Free ID not found in ButtonsIDList" << std::endl;

	return ret;
}

void buttons::ButtonsIDController::FreeButtonID(const ButtonID &id, BaseButton *ptr)
{
	assert(id.CheckIsValid());

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::FreeButtonID, ID: " << id;
#endif

	for (std::size_t i = 0; i < _ButtonsIdList.size(); i++)
	{
		if (_ButtonsIdList[i].first == id) // ID кнопки найден
		{
			auto iter = std::find(_ButtonsIdList[i].second.begin(), _ButtonsIdList[i].second.end(), ptr);

			if (iter != _ButtonsIdList[i].second.end()) // указатель на кнопку найден
				_ButtonsIdList[i].second.erase(iter);
			else
			{
				logger << microlog::LogLevel::Warning << "pointer Not found!";
				throw;
			}

#ifndef NO_LOGS
#ifdef VERBOSE
			logger << ": OK";
#endif

			logger << std::endl;
#endif

			return;
		}
	}

	logger << microlog::LogLevel::Warning << "ID not found in ButtonsIDList" << std::endl;
}

ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList()
{
	return _ButtonsIdList;
}

const ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList() const
{
	return _ButtonsIdList;
}

const std::vector<BaseButton *>& KolibriLib::UI::buttons::ButtonsIDController::GetPointerToButton(const ButtonID &ID) const
{
	assert(ID.CheckIsValid());

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::GetPointerToButton, ID: " << ID << std::endl;
#endif

	return std::find_if(_ButtonsIdList.begin(), _ButtonsIdList.end(),
						[&ID](const ButtonsIDController::node &n)
						{
							return n.first == ID;
						})
		->second;
}

ButtonIDList KolibriLib::UI::buttons::ButtonsIDController::ListToButtonIDList(const List &list)
{
	ButtonIDList ret;
	ret.reserve(list.size());

	for (const ButtonsIDController::node &i : list)
		ret.push_back(i.first);

	return ret;
}

void KolibriLib::UI::buttons::ButtonsIDController::TakeUpButtonID(const ButtonID &id, BaseButton *ptr)
{
	assert(id.CheckIsValid());

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsIDController::TakeUpButtonID, ID: " << id << std::endl;
#endif

	for (node &i : _ButtonsIdList)
	{
		if (i.first == id)
		{
#ifndef NO_LOGS
			logger << microlog::LogLevel::Debug << "Add to exist node" << std::endl;
#endif
			i.second.push_back(ptr);
			return;
		}
	}

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "Create new node" << std::endl;
#endif

	_ButtonsIdList.push_back({id, {ptr}});
}

void KolibriLib::UI::buttons::ButtonsIDController::Sort()
{
	std::sort(_ButtonsIdList.begin(), _ButtonsIdList.end(),
			  [](const node &a, const node &b)
			  {
				  return a.first > b.first;
			  });
}