#include "frame.hpp"



using namespace KolibriLib;
using namespace UI;

Frame::Frame(const Coord &coord, const Size &size, const Colors::Color &Color, const unsigned &Margin) : UIElement(coord, size, Color, Margin)
{
#if DEBUG == true
    _ksys_debug_puts("Frame constructor\n");
#endif
}

void UI::Frame::Render()
{
    graphic::DrawRectangleFill(_coord, _size, _MainColor);

    for (unsigned i = 0; i < _Elements.size(); i++)
    {
        if (_Elements[i].use)
        {
            switch (_Elements[i]._type)
            {
            case Element::Type::TextLabel:
                _Elements[i].txt.Render();
                break;
            case Element::Type::Button:
                _Elements[i].btn.Render();
                break;
            case Element::Type::Image:
                _Elements[i].img.Render();
                break;
            case Element::Type::Form:
                _Elements[i].frm.Render();
                break;
            case Element::Type::CheckBox:
                _Elements[i].ChckBx.Render();
            default:
                break;
            }
        }
    }
}

void UI::Frame::Handler()
{
    if (Hover())
    {
    }
}

template <class T>
unsigned UI::Frame::AddElement(const T &element)
{
    Element a;

    switch (sizeof(T))
    {
    case sizeof(UI::text::TextLabel):
        a.txt = element;
        a._type = Element::Type::TextLabel;
        break;
    case sizeof(UI::buttons::Button):
        a.btn = element;
        a._type = Element::Type::Button;
        break;
    case sizeof(UI::Images::Image):
        a.img = element;
        a._type = Element::Type::Image;
        break;
    case sizeof(UI::Form):
        a.frm = element;
        a._type = Element::Type::Form;
        break;
    case sizeof(UI::CheckBox):
        a.ChckBx = element;
        a._type = Element::Type::CheckBox;
        break;
    default:
        break;
    }
    a.use = true;

    for (unsigned i = 0; i < _Elements.size(); i++) // Ищем свободный элемент
    {
        if (!_Elements[i].use)
        {
            _Elements[i] = a;
            return i;
        }
    }

    _Elements.push_back(a);

    return _Elements.size() - 1;
}

template <class T>
void UI::Frame::SetElement(unsigned i, const T &element)
{
    if (i >= _Elements.size())
    {
        _ksys_debug_puts("KolibriLib::window::Window::SetElement: i >= _Elements.size(), return\n");
        return;
    }
    switch (sizeof(T))
    {
    case sizeof(UI::text::TextLabel):
        _Elements[i].txt = element;
        _Elements[i]._type = Element::Type::TextLabel;
        break;
    case sizeof(UI::buttons::Button):
        _Elements[i].btn = element;
        _Elements[i]._type = Element::Type::Button;
        break;
    case sizeof(UI::Images::Image):
        _Elements[i].img = element;
        _Elements[i]._type = Element::Type::Image;
        break;
    case sizeof(UI::Form):
        _Elements[i].frm = element;
        _Elements[i]._type = Element::Type::Form;
        break;
    case sizeof(UI::CheckBox):
        _Elements[i].ChckBx = element;
        _Elements[i]._type = Element::Type::CheckBox;
        break;
        break;
    default:
        _ksys_debug_puts("KolibriLib::window::Window::AddElement: unknown type, break\n");
        break;
    }
}

template <class T>
T UI::Frame::GetElement(unsigned i) const
{
    if (i >= _Elements.size())
    {
        _ksys_debug_puts("KolibriLib::window::Window::SetElement: i >= _Elements.size(), return\n");
        return 0;
    }

    switch (_Elements[i]._type)
    {
    case Element::Type::TextLabel:
        return _Elements[i].txt;
    case Element::Type::Button:
        return _Elements[i].btn;
    case Element::Type::Image:
        return _Elements[i].img;
    case Element::Type::Form:
        return _Elements[i].frm;
    case Element::Type::CheckBox:
        return _Elements[i].ChckBx;
    case Element::Type::Frame:
        return _Elements[i].frame;
    default:
        _ksys_debug_puts("KolibriLib::window::Window::GetElement: unknown type, break\n");
        break;
    }
    return 0;
}