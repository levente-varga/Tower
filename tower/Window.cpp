#include "Window.h"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIcon = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(windowClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
	return windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return windowClass.hInstance;
}

Window::Window(int width, int height, const char* name) noexcept {
	RECT windowRect;
	windowRect.left = 100;
	windowRect.right = width + windowRect.left;
	windowRect.top = 100;
	windowRect.bottom = height + windowRect.top;

	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	windowHandle = CreateWindow(
		WindowClass::GetName(),
		name,											// title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,									// default starting position X
		CW_USEDEFAULT,									// default starting position Y
		windowRect.right - windowRect.left,				// width
		windowRect.bottom - windowRect.top,				// height
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this
	);

	ShowWindow(windowHandle, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(windowHandle);
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept {
	if (messageType == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
		return pWindow->HandleMessage(windowHandle, messageType, wParam, lParam);
	}
	return DefWindowProc(windowHandle, messageType, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
	return pWindow->HandleMessage(windowHandle, messageType, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept {
	switch (messageType) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, messageType, wParam, lParam);
}