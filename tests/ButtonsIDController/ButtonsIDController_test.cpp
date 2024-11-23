#include <kolibriLib/UI/buttons/ButtonsIDController.hpp>

using namespace KolibriLib::UI::buttons;

ButtonsIDController controller;

int main()
{
	std::vector<ButtonID> IDs;
	IDs.reserve(ButtonIDEnd.value - 2);

	for (std::size_t i = 2; i < ButtonIDEnd; i++) // Все проверять слишком долго
	{
		auto id = controller.GetFreeButtonID(nullptr);
		IDs.push_back(id);
		KolibriLib::logger << microlog::LogLevel::Debug << "Get ID: " << id << std::endl
						   << "List size: " << controller.GetButtonsIDList().size() << std::endl;
	}

	assert(controller.GetButtonsIDList().size() > 0);

	for (std::size_t i = 2; i < ButtonIDEnd; i++)
	{
		try
		{
			controller.FreeButtonID(IDs[i - 2], nullptr);
		}
		catch(...)
		{
			KolibriLib::logger << microlog::LogLevel::Error << "exception in controller.FreeButtonID(IDs[i - 2], nullptr); i=" << i << std::endl;
		}
		
	}

	// удаляем сохранённые ID. Он больше ненужны
	IDs.clear();

	controller.TakeUpButtonID(59, nullptr);
	controller.FreeButtonID(59, nullptr);

	KolibriLib::logger << microlog::LogLevel::Info << "test ButtonsIDController_test is OK" << std::endl;

	return 0;
}