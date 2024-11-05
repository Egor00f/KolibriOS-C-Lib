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

	ButtonID ret = ButtonIDNotSet;

	/*
		В общем че происходит:
		проходимся по всем возможным ID кнопок с _top до конца. Если находим соответствие, то возвращаем id
		иначе повторяем всё ещё раз, но начиная с начала списка.
		можно оптимизировать чтобы полностью по всем элементам _ButtonsIdList не проходиться
	*/

	for (std::uint8_t a = 0; a < 2; a++)
	{
		for (ButtonID i = _top; i < ButtonIDEnd; i.value++)
		{
			for (const node& j : _ButtonsIdList)
			{
				if (j.id != i)
				{
					_ButtonsIdList.push_back(node(i, ptr));

#ifndef NO_LOGS
#ifdef VERBOSE
					logger << ": id is " << i;
#endif
					logger << std::endl;
#endif

					return i;
				}
			}
		}

		_top = _StartTop;
	}

	logger << microlog::LogLevel::Warning << "Free ID not found in ButtonsIDList" << std::endl;

	return ret;
}

void buttons::ButtonsIDController::FreeButtonID(const ButtonID &id, BaseButton *ptr)
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::FreeButtonID, ID: " << id;
#endif

	if (id.CheckIsValid())
	{
		for (std::size_t i = 0; i < _ButtonsIdList.size(); i++)
		{
			if (_ButtonsIdList[i].id == id)
			{
				if (_ButtonsIdList[i].pointers.size() > 1)
				{
					auto iter = std::find(_ButtonsIdList[i].pointers.begin(), _ButtonsIdList[i].pointers.end(), ptr);

					if (iter != _ButtonsIdList[i].pointers.end())
					{
						_ButtonsIdList[i].pointers.erase(iter);
					}
				}
				else // Если в ID заняла только 1 кнопка(или никто не занял), то можно удалить ноду с эти ID
				{
					_ButtonsIdList.erase(_ButtonsIdList.begin() + i);
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
	else
	{
		logger << microlog::LogLevel::Warning << "ID is not valid" << std::endl;
	}
}

ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList()
{
	return _ButtonsIdList;
}

const ButtonsIDController::List &buttons::ButtonsIDController::GetButtonsIDList() const
{
	return _ButtonsIdList;
}

std::vector<BaseButton *> KolibriLib::UI::buttons::ButtonsIDController::GetPointerToButton(const ButtonID &ID) const
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::GetPointerToButton, ID: " << ID << std::endl;
#endif

	std::vector<BaseButton *> ret;

	std::find_if(_ButtonsIdList.begin(), _ButtonsIdList.end(),
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

ButtonIDList KolibriLib::UI::buttons::ButtonsIDController::ListToButtonIDList(const List &list)
{
	ButtonIDList ret;
	ret.reserve(list.size());

	for (const ButtonsIDController::node& i : list)
	{
		ret.push_back(i.id);
	}

	return ret;
}

void KolibriLib::UI::buttons::ButtonsIDController::TakeUpButtonID(const ButtonID &id, BaseButton *ptr)
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsIDController::TakeUpButtonID, ID: " << id << std::endl;
#endif

	for (node& i : _ButtonsIdList)
	{
		if (i.id == id)
		{
#ifndef NO_LOGS
			logger << microlog::LogLevel::Debug << "Add to exist node" << std::endl;
#endif
			i.pointers.push_back(ptr);
			return;
		}
	}

#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "Create new node" << std::endl;
#endif

	_ButtonsIdList.push_back(node(id, ptr));
}

void KolibriLib::UI::buttons::ButtonsIDController::Sort()
{
	std::sort(_ButtonsIdList.begin(), _ButtonsIdList.end(),
			  [](const node &a, const node &b)
			  {
				  return a.id > b.id;
			  });
}

/*
	ButtonsIDController::node
*/

KolibriLib::UI::buttons::ButtonsIDController::node::node(ButtonID Id)
	: id(Id)
{
}

KolibriLib::UI::buttons::ButtonsIDController::node::node(ButtonID Id, BaseButton *p)
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
