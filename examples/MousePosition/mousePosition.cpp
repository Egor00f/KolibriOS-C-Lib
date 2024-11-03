#include <KolibriLib.hpp>

using namespace KolibriLib;

TextLabel *MousePosAbs;
TextLabel *MousePosRelative;

void update()
{
	char buff[64];

	Coord mouseCoord = GetMousePositionOnScreen();
	std::sprintf(buff, "Absolute mouse Pos: y: %i, x: %i", mouseCoord.y, mouseCoord.x);
	MousePosAbs->SetText(buff);

	mouseCoord = GetMousePositionInWindow();
	std::sprintf(buff, "Relative mouse Pos: y: %i, x: %i", mouseCoord.y, mouseCoord.x);
	MousePosRelative->SetText(buff);
}

int main()
{
	SetEventMask(OS::Mask::DefaultEventMask | OS::Mask::MouseEvent);

	Window wnd("Mouse Pos");

	MousePosAbs = wnd.AddElement(TextLabel(
		UDim(0.2f, 0.2f),
		UDim(0.6f, 0.2f),
		"idk"));
	MousePosAbs->SetAlign(TextLabel::Align::Left);
	

	MousePosRelative = wnd.AddElement(TextLabel(
		UDim(0.2f, 0.4f),
		UDim(0.6f, 0.2f),
		"idk"));

	wnd.RenderAllElements();

	update();

	bool exit = false;
	while (!exit)
	{
		switch (wnd.Handler())
		{
		case Event::Exit:

			exit = true;
			break;

		case Event::Mouse:

			update();

			DrawRectangleFill(
				MousePosAbs->GetAbsoluteCoord(),
				MousePosAbs->GetAbsoluteSize(),
				MousePosAbs->GetColor());

			DrawRectangleFill(
				MousePosRelative->GetAbsoluteCoord(),
				MousePosRelative->GetAbsoluteSize(),
				MousePosRelative->GetColor());

			wnd.RenderAllElements();

			break;
		default:
			break;
		}
	}

	return 0;
}