#include <Windows.h>

LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT messageId, WPARAM wParam, LPARAM lParam) 
{
	switch (messageId)
	{
	case WM_CLOSE:
		PostQuitMessage(1);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F') {
			SetWindowText(windowHandle, "Down");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(windowHandle, "Up");
		}
		break;
	case WM_CHAR:

		break;
	}
	return DefWindowProc(windowHandle, messageId, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) 
{
	const auto pClassName = "Window";

	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIcon = nullptr;

	RegisterClassEx(&wc);

	HWND windowHandle = CreateWindowEx(
		0,
		pClassName,
		"Window test",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		10, 10,
		640, 480,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(windowHandle, SW_SHOW);

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
	else {
		return message.wParam;
	}
}