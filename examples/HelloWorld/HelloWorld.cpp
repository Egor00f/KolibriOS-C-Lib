#include <KolibriLib.hpp>

// АХТУНГ!!!!
using namespace KolibriLib;

int main()
{
	// окно
	std::shared_ptr<Window> window(new Window ( 
		"Example Window",           // Заголовок окна
		window::DefaultWindowSize,  // Размер окна
		window::DefaultWindowCoord, // Координаты окна
		Globals::SystemColors       // Таблица цветов для окна
	));

	// Добавление текстовой метки
	std::shared_ptr<TextLabel> label(new TextLabel(
		                            UDim(0.0f, 0, 0.0f, 0), 	// Координаты текстовой метки (самый левый верхний угол окна)
									UDim(0.6f, 0, 1.0f, 0), 	// Размер текстовой метки (3/5 ширины окна и в полную высоту окна)
									"Hello World",
									{32, 36}           	// Размер символов 32x36
								));

	// Добавление кнопки
	std::shared_ptr<TextButton> button(new TextButton(UDim(0.6f, 0, 0.4f, 0), UDim(0.2f, 0, 0.2f, 0)));

	window->AddElement(button); // при добавлении кнопки падает добавлении кнопки падает
	window->AddElement(label); 

	// здесь

	logger << microlog::LogLevel::Info << "Button ID: " << button->GetId() << std::endl;

	// Отрисовка всех элементов, чтоб они были видны
	window->RenderAllElements();

	bool exit = false;
	while (!exit)
	{
		switch (window->Handler())
		{
		case Event::Exit: // Если был нажата кнопка закрытия окна

			exit = true; 
			break;

		case Event::Button: // Была нажата какая-то кнопка

			if(window->GetPressedButton().get() == static_cast<BaseButton*>(button.get()))
			{
				logger << "You Press Button" << std::endl;
				OS::Notify("You Press Buttons");

				label->SetTextColor(Color(std::rand()));
			}
			
			break;
		default:
			break;
		}
	}

	return 0;
}
