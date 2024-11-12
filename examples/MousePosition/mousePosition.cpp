#include <KolibriLib.hpp>

using namespace KolibriLib;

std::shared_ptr<TextLabel> MousePosAbs(new TextLabel(
	UDim(0.2f, 0.2f),
	UDim(0.6f, 0.2f),
	"idk"));
std::shared_ptr<TextLabel> MousePosRelative(new TextLabel(
	UDim(0.2f, 0.4f),
	UDim(0.6f, 0.2f),
	"idk"));

void update()
{
	logger << microlog::LogLevel::Info << "Update";

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
	// Включаем ивенты мыши
	SetEventMask(OS::Mask::DefaultEventMask | OS::Mask::MouseEvent);

	std::shared_ptr<Window> wnd(new Window("Mouse Pos"));

	MousePosAbs->SetAlign(TextLabel::Align::Left);

	wnd->AddElement(MousePosAbs);
	wnd->AddElement(MousePosRelative);

	update();

	wnd->RenderAllElements();

	bool exit = false;
	while (!exit)
	{
		switch (wnd->Handler())
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

			wnd->RenderAllElements();

			break;
		default:
			break;
		}
	}

	return 0;
}
