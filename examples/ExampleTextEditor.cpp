#include <KolibriLib.hpp>

using namespace KolibriLib;

int main()
{
	init();

	Window wnd("TextEdiatorExample");

	auto editor = window.AddElement(BaseEditor(UDim(0.2f, 0.2f), UDim(0.2f, 0.6f)));

	bool exit = false;
	while(!exit)
	{
		switch(wnd.Handler())
		{
			case Event::Exit:
				exit = true;
				break;
		};
	}

	return 0;
}