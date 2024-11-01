#include <kolibriLib/UI/buttons/ButtonsIDController.hpp>
#include <algorithm>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;

UI::buttons::ButtonsIDController *Globals::DefaultButtonsIDController = nullptr;

ButtonID buttons::ButtonsIDController::GetFreeButtonID(std::weak_ptr<BaseButton> ptr)
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
			for (auto j : _ButtonsIdList)
			{
				if (j.id != i)
				{
					_ButtonsIdList.push_back(node(i, ptr));

					#ifndef NO_LOGS
					#ifdef VERBOSE
					logger << ": OK";
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

void buttons::ButtonsIDController::FreeButtonID(const ButtonID &id)
{
	#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "ButtonsController::FreeButtonID";
	#endif

	for (std::size_t i = 0; i < _ButtonsIdList.size(); i++)
	{
		if (_ButtonsIdList[i].id == id)
		{
			_ButtonsIdList.erase(std::next(_ButtonsIdList.begin(), i));

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

std::vector<std::weak_ptr<BaseButton>> KolibriLib::UI::buttons::ButtonsIDController::GetPointerToButton(const ButtonID &ID) const
{
	std::vector<std::weak_ptr<BaseButton>> ret;

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

	for (ButtonsIDController::node i : list)
	{
		ret.push_back(i.id);
	}

	return ret;
}

void KolibriLib::UI::buttons::ButtonsIDController::TakeUpButtonID(const ButtonID &id, std::weak_ptr<BaseButton> ptr)
{
	#ifndef NO_LOGS
	logger << microlog::LogLevel::Debug << "TakeUpButtonID" << std::endl;
	#endif

	for (auto i : _ButtonsIdList)
	{
		if (i.id == id)
		{
			i.pointers.push_back(ptr);
			return;
		}
	}

	_ButtonsIdList.push_back(node(id, ptr));
}

void KolibriLib::UI::buttons::ButtonsIDController::Sort()
{
	std::sort(_ButtonsIdList.begin(), _ButtonsIdList.end(),
			  [](node a, node b)
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