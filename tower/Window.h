#pragma once

#include "CustomWindows.h"

class Window {
private:
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const char* windowClassName = "Unnamed Window";
		static WindowClass windowClass;
		HINSTANCE hInstance;
	};

	int width;
	int height;
	HWND windowHandle;

	static LRESULT CALLBACK HandleMessageSetup(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMessageThunk(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMessage(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Window(int width, int height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
};