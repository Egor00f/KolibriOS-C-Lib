#ifndef __KOLIBRILIB_HPP__
#define __KOLIBRILIB_HPP__

#include <kolibriLib/window/window.hpp>
#include <kolibriLib/graphic/screen.hpp>
#include <kolibriLib/filesystem/filesystem.hpp>
#include <kolibriLib/childWindow.hpp>
#include <kolibriLib/graphic/graphic.hpp>
#include <kolibriLib/graphic/background.hpp>
#include <kolibriLib/network.hpp>
#include <kolibriLib/input/clipboard.hpp>
#include <input.hpp>
#include <KolibriLibUI.hpp>
#include <kolibriLib/system/notify.hpp>

/**
 * @brief Пространство имён библиотеки KolibriLib
 * @author Egor00f
 */
namespace KolibriLib
{

	// Тут всё из этой библиотеки используется как KolibriLib::

	using KolibriLib::Colors::Color;
	using KolibriLib::Colors::ColorsTable;

	using KolibriLib::window::Window;

	using KolibriLib::OS::AllMem;
	using KolibriLib::OS::Event;
	using KolibriLib::OS::Exec;
	using KolibriLib::OS::GetEventMask;
	using KolibriLib::OS::GetFreeMem;
	using KolibriLib::OS::GetSystemColors;
	using KolibriLib::OS::SetEventMask;

	using graphic::DrawCircle;
	using graphic::DrawCircleFill;
	using graphic::DrawLine;
	using graphic::DrawPixel;
	using graphic::DrawRectangleFill;
	using graphic::DrawRectangleGradient;
	using graphic::DrawRectangleLines;
	using graphic::DrawTriangle;
	using graphic::ReadArea;
	using graphic::ReadPoint;

	using UI::buttons::autoDefineButton;
	using UI::buttons::DefineButton;
	using UI::buttons::DeleteButton;

	using mouse::CenterCursor;
	using mouse::GetMouseButtons;
	using mouse::GetMouseEvents;
	using mouse::GetMousePositionInWindow;
	using mouse::GetMousePositionOnSreen;

	using keyboard::CheckKeyboard;
	using keyboard::KeyboardLayout;
	using keyboard::Scancode;
	using keyboard::SetKeyboardLayout;

} // namespace KolibriLib

#endif // __KOLIBRILIB_HPP__
