#pragma once

#include "Window.h"

class Application
{
public:
	Application();
	int Start();

private:
	void DoFrame();

	Window window;
};