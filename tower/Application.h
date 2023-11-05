#pragma once

#include "Window.h"
#include "Timer.h"

class Application
{
public:
	Application();
	int Start();

private:
	void DoFrame();

	Window window;
	Timer timer;
};