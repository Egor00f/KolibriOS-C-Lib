#include <KolibriLib.hpp>

// АХТУНГ!!!!
using namespace KolibriLib;

int main()
{
	// окно
	Window* wndw = new Window ( 
		"Example Window",           // Заголовок окна
		window::DefaultWindowSize,  // Размер окна
		window::DefaultWindowCoord, // Координаты окна
		Globals::SystemColors       // Таблица цветов для окна
	);

	// Добавление текстовой метки
	TextLabel* label = wndw->AddElement(TextLabel(
		                            UDim(0.0f, 0, 0.0f, 0), 	// Координаты текстовой метки (самый левый верхний угол окна)
									UDim(0.6f, 0, 1.0f, 0), 	// Рамер текстовой метки (3/5 ширины окна и в полную высоту окна)
									"Hello World",
									{32, 36}           	// Размер символов 32x36
								)
					);

	// Добавление кнопки
	TextButton* button = wndw->AddElement(TextButton(UDim(0.6f, 0, 0.4f, 0), UDim(0.2f, 0, 0.2f, 0)));

	// Отрисовка всех элементов, чтоб они были видны
	wndw->RenderAllElements();

	bool exit = false;
	while (!exit)
	{
		switch (wndw->Handler())
		{
		case Event::Exit: // Если был нажата кнопка закрытия окна

			exit = true; 
			break;

		case Event::Button: // Была нажата какая-то кнопка

			if(static_cast<BaseButton*>(wndw->GetPressedButton().get()) == static_cast<BaseButton*>(button))
			{
				logger << "You Press Button";
				OS::Notify("You Press Buttons");

				label->SetTextColor(Color(rand()));
			}
			
			break;
		default:
			break;
		}
	}

	delete wndw;

	return 0;
}
