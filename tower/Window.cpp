#include "Window.h"
#include <sstream>
#include "resource.h"

/*
 *	Implementation of Window
 */

Window::Window(int width, int height, const std::string title)
	:
	width(width),
	height(height)
{
	RECT windowRect;
	windowRect.left = 100;
	windowRect.right = width + windowRect.left;
	windowRect.top = 100;
	windowRect.bottom = height + windowRect.top;

	if (AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WINDOW_EXCEPTION(GetLastError());
	}

	windowHandle = CreateWindow(
		WindowClass::GetName(),
		title.c_str(),											// title
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

	if (windowHandle == nullptr)
	{
		throw WINDOW_EXCEPTION(GetLastError());
	}

	ShowWindow(windowHandle, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(windowHandle);
}

void Window::SetTitle(const std::string title)
{
	if (SetWindowText(windowHandle, title.c_str()) == 0)
	{
		throw WINDOW_EXCEPTION(GetLastError());
	}
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept
{
	if (messageType == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
		return pWindow->HandleMessage(windowHandle, messageType, wParam, lParam);
	}
	return DefWindowProc(windowHandle, messageType, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
	return pWindow->HandleMessage(windowHandle, messageType, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND windowHandle, UINT messageType, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (messageType)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;

		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (!(lParam & 0x40000000) || keyboard.AutoRepeatIsEnabled())
			{
				keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_CHAR:
		case WM_SYSCHAR:
		{
			keyboard.OnChar(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_KILLFOCUS:
		{
			keyboard.ClearState();
			break;
		}
		case WM_MOUSEMOVE:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnMouseMove(point.x, point.y);
			if (0 <= point.x && point.x < width && 0 <= point.y && point.y < height)
			{
				mouse.OnMouseMove(point.x, point.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(windowHandle);
					mouse.OnMouseEnterWindow();
				}
			}
			else
			{
				if (mouse.LeftIsPressed() || mouse.RightIsPressed() || mouse.MiddleIsPressed())
				{
					mouse.OnMouseMove(point.x, point.y);
				}
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeaveWindow();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(point.x, point.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnRightPressed(point.x, point.y);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnMiddlePressed(point.x, point.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(point.x, point.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnRightReleased(point.x, point.y);
			break;
		}
		case WM_MBUTTONUP:
		{
			const POINTS point = MAKEPOINTS(lParam);
			mouse.OnMiddleReleased(point.x, point.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS point = MAKEPOINTS(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.OnWheelUp(point.x, point.y);
			}
			else
			{
				mouse.OnWheelDown(point.x, point.y);
			}
			break;
		}
	}

	return DefWindowProc(windowHandle, messageType, wParam, lParam);
}



/*
 *	Implementation of Window::WindowClass
 */

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(windowClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}



/*
 *	Implementation of Window::WindowException
 */

Window::WindowException::WindowException(int line, const char* file, HRESULT hResult) noexcept
	: 
	Exception(line, file), 
	hResult(hResult) 
{}

const char* Window::WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::WindowException::GetType() const noexcept
{
	return "Window Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT hResult) noexcept
{
	char* pMessageBuffer = nullptr;
	DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer),
		0,
		nullptr
	);

	if (nMessageLength == 0)
	{
		return "Unidentified error code";
	}

	std::string errorString = pMessageBuffer;
	LocalFree(pMessageBuffer);
	return errorString;
}

HRESULT Window::WindowException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string Window::WindowException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hResult);
}