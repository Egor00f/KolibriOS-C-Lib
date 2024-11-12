#include <kolibriLib/UI/UI.hpp>

#include <algorithm>

#include <input.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/graphic/screen.hpp>
#include <kolibriLib/img.hpp>

using namespace KolibriLib;
using namespace UI;

/*
	GuiObject
*/

unsigned KolibriLib::UI::GuiObject::GetMargin() const
{
	return _Margin;
}

void KolibriLib::UI::GuiObject::SetMargin(unsigned NewMargin)
{
	_Margin = NewMargin;
}

/*
	UIElement
*/

KolibriLib::UI::UIElement::UIElement(const UDim &coord, const UDim &size, const Colors::Color &MainColor, const unsigned &Margin)
	: _coord(coord),
	  _size(size),
	  _MainColor(MainColor)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "UIElement constructor" << std::endl;
#endif
	_Margin = Margin;
}

KolibriLib::UI::UIElement::UIElement(const UIElement &cp)
	: _coord(cp._coord),
	  _size(cp._size),
	  _MainColor(cp._MainColor),
	  Parent(cp.Parent),
	  Visible(cp.Visible),
	  _childs(cp._childs),
	  ParentIsWindow(cp.ParentIsWindow)
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "UIElement constructor(copy)" << std::endl;
#endif

	_Margin = cp._Margin;
}

KolibriLib::UI::UIElement::~UIElement()
{
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "UIElement destructor" << std::endl;
#endif

	DeleteFromParentChilds();
}

/*
	UIElement::Functions
*/

void KolibriLib::UI::UIElement::DeleteFromParentChilds()
{
	if (!Parent.expired() && !ParentIsWindow)
	{
#ifdef VERBOSE
		logger << microlog::LogLevel::Debug << "Delete From Parent childs";
#endif
		std::shared_ptr<UIElement> s_ptr;
		std::shared_ptr<UIElement> THIS(this);

		((UIElement *)Parent.lock().get())->DeleteChildren(THIS);

		Parent.reset();
	}
	else if (ParentIsWindow)
	{
#ifdef VERBOSE
		logger << microlog::LogLevel::Debug << "Parent Is window";
#endif
	}
	else
	{
#ifdef VERBOSE
		logger << microlog::LogLevel::Debug << "Parent is expired";
#endif
	}
}

UDim KolibriLib::UI::UIElement::GetSize() const
{
	return _size;
}

void KolibriLib::UI::UIElement::SetSize(const UDim &NewSize)
{
	_size = NewSize;
}

Colors::Color KolibriLib::UI::UIElement::GetColor() const
{
	return _MainColor;
}

void KolibriLib::UI::UIElement::SetColor(const Colors::Color &NewColor)
{
	_MainColor = NewColor;
}

void KolibriLib::UI::UIElement::SetCoord(const UDim &NewCoord)
{
	_coord = NewCoord;
}

Size KolibriLib::UI::UIElement::GetAbsoluteSize() const
{
#ifndef NO_LOG
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Get Absolute Size" << std::endl;
#endif
#endif

	Size ret;

	if (std::shared_ptr<GuiObject> s_ptr = Parent.lock())
		ret = _size.GetAbsolute(s_ptr.get()->GetAbsoluteSize());
	else
		ret = _size.GetAbsolute(window::GetWindowSize());

	ret -= static_cast<int>(_Margin);

	return ret;
}

Coord KolibriLib::UI::UIElement::GetAbsoluteCoord() const
{
#ifndef NO_LOG
#ifdef VERBOSE
	logger << microlog::LogLevel::Debug << "Get Absolute Coord" << std::endl;
#endif
#endif

	Coord ret;

	if (std::shared_ptr<GuiObject> s_ptr = Parent.lock())
	{
		if (ParentIsWindow)
			ret = _coord.GetAbsolute(s_ptr->GetAbsoluteSize());
		else
			ret = (_coord.GetAbsolute(s_ptr->GetAbsoluteSize()) + s_ptr->GetAbsoluteCoord());
	}
	else
	{
		ret = _coord.GetAbsolute(window::GetWindowSize());
	}

	ret += static_cast<int>(_Margin);

	return ret;
}

UDim KolibriLib::UI::UIElement::GetCoord() const
{
	return _coord;
}

bool KolibriLib::UI::UIElement::Hover() const
{
	const Coord Mouse = mouse::GetMousePositionInWindow();
	const point coord = GetAbsoluteSize();
	const Size size = GetAbsoluteSize();

	return ((coord < Mouse) &&
			(Mouse.x < (coord.x + size.x)) &&
			(Mouse.y < (coord.x + size.y)) &&
			(ScreenPointAffiliation(Mouse) == Thread::GetThreadSlot()));
}

int KolibriLib::UI::UIElement::Handler(OS::Event)
{
	//_ksys_debug_puts("call KolibriLib::UIElement, this func do nothing...\n");
	return 0;
}

bool KolibriLib::UI::UIElement::OnButtonEvent(buttons::ButtonID)
{
	return false;
}

bool KolibriLib::UI::UIElement::OnKeyEvent()
{
	return false;
}

bool KolibriLib::UI::UIElement::OnMouseEvent()
{
	return false;
}

void KolibriLib::UI::UIElement::WindowAsParent(std::weak_ptr<GuiObject> window)
{
	DeleteFromParentChilds();

	Parent = window;

	ParentIsWindow = true;

	if(window.expired())
		logger << microlog::LogLevel::Warning << "child is expired" << std::endl;

	logger << microlog::LogLevel::Debug << "done WindowAsParent" << std::endl;
}

void KolibriLib::UI::UIElement::SetParent(std::weak_ptr<UIElement> ptr)
{
	DeleteFromParentChilds();

	Parent = ptr;

	ParentIsWindow = false;

	if(ptr.expired())
		logger << microlog::LogLevel::Warning << "child is expired" << std::endl;

	logger << microlog::LogLevel::Debug << "done SetParent" << std::endl;
}

const std::weak_ptr<GuiObject> KolibriLib::UI::UIElement::GetParent() const
{
	return Parent;
}

std::weak_ptr<GuiObject> KolibriLib::UI::UIElement::GetParent()
{
	return Parent;
}

UIElement &KolibriLib::UI::UIElement::operator=(const UIElement &Element)
{
	_coord = Element._coord;
	_size = Element._size;
	_MainColor = Element._MainColor;
	Parent = Element.Parent;
	Visible = Element.Visible;
	_Margin = Element._Margin;

	return *this;
}

bool KolibriLib::UI::UIElement::operator==(const UIElement &Element) const
{
	return (_coord == Element._coord) &&
		   (_size == Element._size) &&
		   (_MainColor == Element._MainColor) &&
		   (Visible == Element.Visible) &&
		   (_Margin == Element._Margin);
}

bool KolibriLib::UI::UIElement::operator!=(const UIElement &Element) const
{
	return (_coord != Element._coord) ||
		   (_size != Element._size) ||
		   (_MainColor != Element._MainColor) ||
		   (Visible != Element.Visible) ||
		   (_Margin != Element._Margin);
}

void KolibriLib::UI::UIElement::Render() const
{
	logger << microlog::LogLevel::Debug << "Render UIElement" << std::endl;

	if (_MainColor._a != 0xFF)
	{
		if (_MainColor._a != 0)
		{
			Images::img img(_MainColor, GetAbsoluteSize());

			img.Draw(GetAbsoluteCoord());
		}
	}
	else
	{
		graphic::DrawRectangleFill(GetAbsoluteCoord(), GetAbsoluteSize(), _MainColor);
	}
}

void KolibriLib::UI::UIElement::SetSize(const Size &NewSize)
{
	_size = UDim(NewSize);
}

void KolibriLib::UI::UIElement::SetCoord(const Coord &NewCoord)
{
	_coord = UDim(NewCoord);
}

std::vector<std::weak_ptr<UIElement>> KolibriLib::UI::UIElement::GetChildren() const
{
	return _childs;
}

std::vector<std::weak_ptr<UIElement>> &KolibriLib::UI::UIElement::GetChildren()
{
	return _childs;
}

void KolibriLib::UI::UIElement::AddChildren(std::weak_ptr<UIElement> child) const
{
	if (!child.expired())
	{
		logger << microlog::LogLevel::Debug << "Add Children" << std::endl;

		_childs.push_back(child);
	}
	else
	{
		logger << microlog::LogLevel::Error << "child is expired" << std::endl;
	}
}

void KolibriLib::UI::UIElement::DeleteChildren(const std::weak_ptr<UIElement> &child) const
{
	if (!child.expired())
	{
		logger << microlog::LogLevel::Debug << "Delete Children" << std::endl;

		auto n = std::find_if(_childs.begin(), _childs.end(),
							  [&child](std::weak_ptr<UIElement> &obj)
							  {
								  return child.lock() == obj.lock();
							  });

		if (n != _childs.end())
			_childs.erase(n);
		else
			logger << microlog::LogLevel::Warning << "child not found" << std::endl;
	}
	else
	{
		logger << microlog::LogLevel::Error << "child is expired" << std::endl;
	}
}

buttons::ButtonsIDController *KolibriLib::UI::UIElement::GetButtonIDController() const
{
	if (auto s_ptr = Parent.lock())
	{
		return s_ptr.get()->GetButtonIDController();
	}
	else
	{
		return nullptr;
	}
}

void KolibriLib::UI::UIElement::SetButtonIDController(const buttons::ButtonsIDController *)
{
}

void KolibriLib::UI::UIElement::swap(UIElement &e)
{
	UIElement buff(*this);

	*this = e;
	e = buff;
}