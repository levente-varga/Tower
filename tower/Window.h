#pragma once

#include <optional>
#include <memory>

#include "CustomWindows.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

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
	std::unique_ptr<Graphics> pGraphics;

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
		std::string GetErrorDescription() const noexcept;

	private:
		HRESULT hResult;
	};

	class NoGraphicsException : Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

	Keyboard keyboard;
	Mouse mouse;

	Window(int width, int height, const std::string title);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::string title);
	static std::optional<int> ProcessMessages();
	Graphics& GetGraphics();
};

#define WINDOW_EXCEPTION(hResult) Window::WindowException(__LINE__, __FILE__, hResult)
#define NO_GRAPHICS_EXCEPTION() Window::NoGraphicsException(__LINE__, __FILE__)