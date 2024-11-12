#include <KolibriLib.hpp>

using namespace KolibriLib;

int main()
{
	std::shared_ptr<Window> window(new Window("Checkbox example"));

	std::shared_ptr<Frame> frame(new Frame(
		UDim(0.2f, 0.2f), 
		UDim(0.6f, 0.4f)));
	frame->SetColor(Globals::SystemColors.work_area);

	std::shared_ptr<CheckBox> checkbox(new CheckBox(
		UDim(0.1f, 0.25f),		
		UDim(0.2f, 0.5f)
	));
	checkbox->SetParent(frame);
	
	std::shared_ptr<TextLabel> textLabel(new TextLabel(
		UDim(0.3f, 0.0f),
		UDim(0.7f, 1.0f),
		"<- checkbox"
	));
	textLabel->SetParent(frame);

	window->AddElement(frame);
	window->AddElement(checkbox);
	window->AddElement(textLabel);

	window->RenderAllElements();

	bool exit = false;
	while (!exit)
	{
		switch (window->Handler())
		{
		case Event::Exit:
			exit = true;
			break;
		case Event::Button:

			static bool lastState = false;

			if(checkbox->GetChecked() != lastState)
			{
				lastState = checkbox->GetChecked();

				OS::Notify("You change checkbox state");
			}

			break;
		default:
			break;
		}
	}
	
	return 0;
}
