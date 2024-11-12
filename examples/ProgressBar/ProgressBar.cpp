#include <KolibriLib.hpp>

// АХТУНГ!!!!
using namespace KolibriLib;

int main()
{
	std::shared_ptr<Window> wndw(new Window(
		"Example Window" // Заголовок окна
		));

	// Добавление текстовой метки
	std::shared_ptr<ProgressBar> bar(new ProgressBar(
		UDim(0.2f, 0.2f),
		UDim(0.2f, 0.6f)));

	// Добавление кнопки
	std::shared_ptr<TextButton> button(new TextButton(
		UDim(0.6f, 0, 0.4f, 0),
		UDim(0.2f, 0, 0.2f, 0),
		"Press Me"));

	wndw->AddElement(bar);
	wndw->AddElement(button);

	// Отрисовка всех элементов, чтоб они были видны
	wndw->RenderAllElements();

	bool exit = false;
	while (!exit)
	{
		// Вызов обработчика окна
		Event event = wndw->Handler();

		switch (event)
		{
		case Event::Exit: // Если был нажата кнопка закрытия окна

			exit = true;
			break;

		case Event::Button:

			if (static_cast<BaseButton *>(wndw->GetPressedButton().get()) == static_cast<BaseButton *>(button.get()))
			{
				bar->AddFill(1);
			}

			break;
		default:
			break;
		}
	}

	return 0;
}
