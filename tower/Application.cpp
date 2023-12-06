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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	window.GetGraphics().ClearBuffer(c, c, 0);
	window.GetGraphics().DrawTestTriangle(timer.Peek());
	window.GetGraphics().Present();
}