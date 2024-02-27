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
	window.GetGraphics().ClearBuffer(0, 0, 0);
	window.GetGraphics().DrawTestTriangle(
		timer.Peek(),
		window.mouse.GetPositionX() / 800.0f * 2.0f - 1.0f,
		-window.mouse.GetPositionY() / 600.0f * 2.0f + 1.0f
	);
	window.GetGraphics().Present();
}