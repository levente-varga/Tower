#pragma once

#include "Window.h"
#include "Timer.h"
#include "Box.h"



class Application
{
public:
	Application();
	~Application();
	int Start();

private:
	void DoFrame();

	Window window;
	Timer timer;

	std::vector<std::unique_ptr<Box>> boxes;
};