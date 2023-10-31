#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) 
{
	Window window(640, 480, "Tower");

	MSG message;
	BOOL gResult;
	while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&message); // Creates WM_CHAR messages if the event is a key press
		DispatchMessage(&message);
	}

	if (gResult == -1) {
		return -1;
	}
	
	return message.wParam;
}