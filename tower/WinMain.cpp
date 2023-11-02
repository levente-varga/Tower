#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) 
{
	try
	{
		Window window(640, 480, "Tower");

		MSG message;
		BOOL gResult;
		while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&message); // Creates WM_CHAR messages if the event is a key press
			DispatchMessage(&message);
		}

		if (gResult == -1)
		{
			return -1;
		}

		return message.wParam;
	}
	catch (const Exception& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception e)
	{
		MessageBox(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}