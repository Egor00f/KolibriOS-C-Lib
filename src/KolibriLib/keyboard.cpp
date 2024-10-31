#include <kolibriLib/input/keyboard.hpp>
#include <algorithm>
#include <kolibriLib/debug.hpp>

using namespace KolibriLib;
using namespace keyboard;

Scancode keyboard::GetScancodeByASCII(char ascii, KeyboardLayoutMode mode)
{
	KeyboardLayout layout = GetKeyboardLayout(mode);

	for (uint8_t i = 0; i < sizeof(KeyboardLayout); i++)
	{
		if (layout[i] == ascii)
		{
			return Scancode(i);
		}
	}

	return Scancodes::ANY;
}

Scancode keyboard::GetScancodeByASCII(char ascii)
{
	Scancode a = GetScancodeByASCII(ascii, KeyboardLayoutMode::Normal);

	if (a == Scancode(Scancodes::ANY))
	{
		a = GetScancodeByASCII(ascii, KeyboardLayoutMode::Shift);

		if (a == Scancode(Scancodes::ANY)) // Вероятность того что до этого дойдёт довольно низка
		{
			a = GetScancodeByASCII(ascii, KeyboardLayoutMode::Alt);
		}
	}

	return (Scancode){a};
}

Scancode::Scancode(char c)
{
	logger << microlog::LogLevel::Debug << "Scancode constructor (from char)" << std::endl;
	val = GetScancodeByASCII(c);
}

KolibriLib::keyboard::Scancode::Scancode(Scancodes v)
	: val(v)
{
}

Scancode::operator char() const
{
	const auto ret = keyboard::GetKeyboardLayout();
	return ret[static_cast<std::uint8_t>(val)];
}

Scancode::operator std::uint8_t() const
{
	return static_cast<std::uint8_t>(val);
}

KolibriLib::keyboard::Scancode::operator Scancodes() const
{
	return val;
}

bool KolibriLib::keyboard::Scancode::operator==(const Scancode &val)
{
	return val.val == val;
}

bool KolibriLib::keyboard::Scancode::operator!=(const Scancode &val)
{
	return val.val != val;
}

char &KeyboardLayout::operator[](uint8_t i)
{
	return table[i];
}

char KeyboardLayout::operator[](uint8_t i) const
{
	return table[i];
}
