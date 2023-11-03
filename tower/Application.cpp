#include "Application.h"

Application::Application()
	:
	window(800, 600, "Tower")
{
}

int Application::Start()
{
	while (true)
	{
		if (const auto eCode = Window::ProcessMessages())
		{
			return *eCode;
		}
		DoFrame();
	}
}

void Application::DoFrame()
{

}