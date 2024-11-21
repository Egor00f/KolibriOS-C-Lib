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
		проходимся по всем возможным ID кнопок с _top до конца. Если находим соответствие, то возвращаем id
		иначе повторяем всё ещё раз, но начиная с начала списка.
		можно оптимизировать чтобы полностью по всем элементам _ButtonsIdList не проходиться
	*/

	auto find = [](ButtonID &top, List &ButtonsIdList, BaseButton *Pointer)
	{
		for (ButtonID i = top; i < ButtonIDEnd; i.value++)
		{
			for (const node &j : ButtonsIdList)
			{
				if (j.id != i)
				{
					ButtonsIdList.push_back(node(i, Pointer));

					top = i + 1;

#ifndef NO_LOGS
#ifdef VERBOSE
					logger << ": return ID: " << i;
#endif
					logger << std::endl;
#endif

					return i;
				}
			}
		}

		return ButtonIDNotSet;
	};

	ButtonID ret = find(_top, _ButtonsIdList, ptr);

	if (ret == ButtonIDNotSet) // Пытаемся получить ID ещё раз
	{
		_top = _StartTop; // Но уже с самого начала
		ret = find(_top, _ButtonsIdList, ptr);
	}

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
		if (_ButtonsIdList[i].id == id) // ID кнопки найден
		{
			auto iter = std::find(_ButtonsIdList[i].pointers.begin(), _ButtonsIdList[i].pointers.end(), ptr);

			if (iter != _ButtonsIdList[i].pointers.end())
				_ButtonsIdList[i].pointers.erase(iter);
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

std::vector<BaseButton *> KolibriLib::UI::buttons::ButtonsIDController::GetPointerToButton(const ButtonID &ID) const
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::GetPointerToButton, ID: " << ID << std::endl;
#endif

	return std::find_if(_ButtonsIdList.begin(), _ButtonsIdList.end(),
						[&ID](const ButtonsIDController::node &n)
						{
							return n.id == ID;
						})
		->pointers;
}

ButtonIDList KolibriLib::UI::buttons::ButtonsIDController::ListToButtonIDList(const List &list)
{
	ButtonIDList ret;
	ret.reserve(list.size());

	for (const ButtonsIDController::node &i : list)
		ret.push_back(i.id);

	return ret;
}

void KolibriLib::UI::buttons::ButtonsIDController::TakeUpButtonID(const ButtonID &id, BaseButton *ptr)
{
#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsIDController::TakeUpButtonID, ID: " << id << std::endl;
#endif

	for (node &i : _ButtonsIdList)
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
