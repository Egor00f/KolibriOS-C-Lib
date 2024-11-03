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
	_Margin = Margin;
}

KolibriLib::UI::UIElement::UIElement(const UIElement &cp)
	: _coord(cp._coord),
	  _size(cp._size),
	  _MainColor(cp._MainColor),
	  Parent(cp.Parent)
{
	_Margin = cp._Margin;
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

void KolibriLib::UI::UIElement::Rotate(unsigned NewAngle)
{
	_rotation = NewAngle;
}

unsigned KolibriLib::UI::UIElement::GetRotate() const
{
	return _rotation;
}

bool KolibriLib::UI::UIElement::Hover() const
{
	if (Parent.lock())
	{
		const Coord Mouse = mouse::GetMousePositionInWindow();
		const point coord = GetAbsoluteSize();
		const Size size = GetAbsoluteSize();

		return ((coord < Mouse) &&
				(Mouse.x < (coord.x + size.x)) &&
				(Mouse.y < (coord.x + size.y)) &&
				(ScreenPointAffiliation(Mouse) == Thread::GetThreadSlot()));
	}

	return false;
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

void KolibriLib::UI::UIElement::SetParent(const UIElement *NewParent) const
{
	logger << microlog::LogLevel::Debug << "SetParent" << std::endl;

	std::shared_ptr<GuiObject> s_ptr = Parent.lock();

	if (!ParentIsWindow && s_ptr)
	{
		((UIElement *)s_ptr.get())->DeleteChildren(this);
	}

	std::shared_ptr<GuiObject> ptr(const_cast<GuiObject *>(static_cast<const GuiObject *>(NewParent)));

	s_ptr.swap(ptr);

	((UIElement *)s_ptr.get())->AddChildren(this);

	ParentIsWindow = false;
}

void KolibriLib::UI::UIElement::SetParent(std::weak_ptr<UIElement> ptr) const
{
	SetParent(ptr.lock().get());
}

void KolibriLib::UI::UIElement::WindowAsParent(const GuiObject *window) const
{
	std::shared_ptr<GuiObject> s_ptr = Parent.lock();

	if (!ParentIsWindow && s_ptr)
	{
		((UIElement *)s_ptr.get())->DeleteChildren(this);
	}

	s_ptr = std::shared_ptr<GuiObject>(const_cast<GuiObject *>(window));

	ParentIsWindow = true;
}

const GuiObject *KolibriLib::UI::UIElement::GetParent() const
{
	return Parent.lock().get();
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

void KolibriLib::UI::UIElement::AddChildren(const UIElement *child) const
{
	logger << microlog::LogLevel::Debug << "Add Children" << std::endl;

	std::shared_ptr<UIElement> s_ptr(const_cast<UIElement *>(child));

	_childs.push_back(s_ptr);
}

void KolibriLib::UI::UIElement::DeleteChildren(const UIElement *child) const
{
	logger << microlog::LogLevel::Debug << "Delete Children" << std::endl;

	std::shared_ptr<UIElement> ptr(const_cast<UIElement *>(child));
	std::weak_ptr<UIElement> v = ptr;

	auto n = std::find_if(
		_childs.begin(), _childs.end(),
		[&v](const std::weak_ptr<UIElement> &our_obj)
		{
			return v.lock() == our_obj.lock();
		});

	if (n != _childs.end())
	{
		_childs.erase(n);
	}
}

buttons::ButtonsIDController *KolibriLib::UI::UIElement::GetButtonIDController() const
{
	auto s_ptr = Parent.lock();

	if (s_ptr)
		return s_ptr.get()->GetButtonIDController();
	else
		return nullptr;
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