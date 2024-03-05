#include "Application.h"
#include <memory>
#include <random>
#include "Box.h"


Application::Application()
	:
	window(800, 600, "Tower")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> alist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> dlist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> olist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rlist(2.0f, 15.0f);

	for (int i = 0; i < 10; i++) {
		boxes.push_back(std::make_unique<Box>(window.GetGraphics(), rng, alist, dlist, olist, rlist));
	}

	window.GetGraphics().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1, 0.75f, 0.5f, 40));
}

Application::~Application() 
{}

int Application::Start()
{
	while (true)
	{
		if (const auto errorCode = Window::ProcessMessages())
		{
			return *errorCode;
		}
		DoFrame();
	}
}

void Application::DoFrame()
{
	float delta = timer.Mark();

	window.GetGraphics().ClearBuffer(0, 0, 0);
	for (auto& box : boxes) {
		box->Update(delta);
		box->Draw(window.GetGraphics());
	}
	window.GetGraphics().Present();
}