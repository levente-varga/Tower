#pragma once

#include "CustomWindows.h"
#include "Exception.h"

class Window
{
private:
	class WindowClass
	{
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
	class WindowException : public Exception
	{
	public:
		WindowException(int line, const char* file, HRESULT hResult) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hResult;
	};

	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
};

// 
#define WINDOW_EXCEPTION(hResult) Window::WindowException(__LINE__, __FILE__, hResult)