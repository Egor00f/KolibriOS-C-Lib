#include <kolibriLib/UI/checkbox.hpp>

using namespace KolibriLib;
using namespace UI;

CheckBox::CheckBox(const UDim &coord, const UDim &size, const style &Style, const Colors::Color &CheckBoxBorderColor, const Colors::Color &BackgroundColor, const unsigned &Margin)
	:	Button(coord, size, Margin, BackgroundColor), 
		_BorderColor(CheckBoxBorderColor),
		_style(Style)
{
	PrintDebug("CheckBox Constructor\n");
}

void KolibriLib::UI::CheckBox::DrawBorder() const
{
	const Coord absCoord = GetAbsoluteCoord();
	const Coord absSize = GetAbsoluteSize();

	switch (_style)
	{
	case Default:
		graphic::DrawRectangleLines(absCoord, absCoord + absSize, _MainColor);
		break;
	case Circle:
		graphic::DrawCircle(absCoord + point(absSize.x / 2, absSize.y / 2), static_cast<unsigned int>(absSize.x) / 2, _BorderColor);
	default:
		break;
	}
}

bool KolibriLib::UI::CheckBox::GetChecked() const
{
	return checked;
}

void KolibriLib::UI::CheckBox::OnButtonEvent(buttons::ButtonID PressedButtonID)
{
	Button::OnButtonEvent(PressedButtonID);
	if(_status)
	{
		checked = !checked;
	}
}

void CheckBox::Render() const
{
	if(Visible)
	{
		PrintDebug("Render Checkbox\n");

		DrawBorder();

		const Coord absoluteCoord = GetAbsoluteCoord();
		const Size absoluteSize = GetAbsoluteSize();

		if (checked)
		{
			switch (_style)
			{
			case CheckBox::style::Default:
				graphic::DrawRectangleFill(absoluteCoord,
										   absoluteSize,
										   _BorderColor);
				break;
			case CheckBox::style::Circle:
				graphic::DrawCircle(absoluteCoord + point(absoluteSize.x / 2, absoluteSize.y / 2),
									(static_cast<unsigned int>(absoluteSize.x) / 2U) - GetMargin(),
									_BorderColor);
				break;
			case CheckBox::style::Smoth:
				PrintDebug("KolibriLib::UI::Checkbox smath style now not support :(\n");
				break;
			default:
				break;
			}
		}

		Button::Render();
	}
}
