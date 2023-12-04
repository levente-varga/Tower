#pragma comment(lib, "d3d11.lib")

#include <sstream>
#include "Graphics.h"


#define GRAPHICS_EXCEPTION_NOINFO(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hr))
#define GRAPHICS_THROW_NOINFO(hResultCall) if (FAILED(hResult = (hResultCall))) throw Graphics::GraphicsException()

#ifndef NDEBUG

#define GRAPHICS_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hResult), infoManager.GetMessages())
#define GRAPHICS_THROW_INFO(hResultCall) infoManager.Set(); if (FAILED(hResult = (hResultCall))) throw GRAPHICS_EXCEPTION(hResult)
#define GRAPHICS_DEVICE_REMOVED_EXCEPTION(hResult) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hResult), infoManager.GetMessages())

#else

#define GRAPHICS_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hResult))
#define GRAPHICS_THROW_INFO(hResultCall) GFX_THROW_NOINFO(hResultCall)
#define GRAPHICS_DEVICE_REMOVED_EXCEPTION(hResult) Graphics::DeviceRemovedException(__LINE__, __FILE__)

#endif


Graphics::Graphics(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
	swapChainDescription.BufferDesc.Width = 0;
	swapChainDescription.BufferDesc.Height = 0;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 2;
	swapChainDescription.OutputWindow = windowHandle;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDescription.Flags = 0;

	UINT swapCreateFlags = 0;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Used in the macros to get the result of function calls
	HRESULT hResult;

	GRAPHICS_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	ID3D11Resource* pBackBuffer = nullptr;

	GRAPHICS_THROW_INFO(pSwapChain->GetBuffer(
		0,
		_uuidof(ID3D11Resource),
		reinterpret_cast<void**>(&pBackBuffer)
	));

	GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget
	));

	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pTarget != nullptr)
	{
		pTarget->Release();
	}

	if (pContext != nullptr)
	{
		pContext->Release();
	}

	if (pSwapChain != nullptr)
	{
		pSwapChain->Release();
	}

	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
}

void Graphics::Present()
{
	HRESULT hResult;

#ifndef NDEBUG
	infoManager.Set();
#endif

	if (FAILED(hResult = pSwapChain->Present(1, 0)))
	{
		if (hResult == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GRAPHICS_DEVICE_REMOVED_EXCEPTION(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GRAPHICS_EXCEPTION(hResult);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 0 };
	pContext->ClearRenderTargetView(pTarget, color);
}



/*
 *	Implementation of Graphics::GraphicsException
 */

Graphics::GraphicsException::GraphicsException(int line, const char* file, HRESULT hResult, std::vector<std::string> debugInfoMessages) noexcept
	:
	Exception(line, file),
	hResult(hResult)
{
	for (int i = 0; i < debugInfoMessages.size(); i++)
	{
		debugInfo += debugInfoMessages[i];
		if (i < debugInfoMessages.size() - 1) debugInfo += '\n';
	}
}

const char* Graphics::GraphicsException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!debugInfo.empty())
	{
		oss << "\n[Debug Info]\n" << GetDebugInfo() << "\n\n";
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::GraphicsException::GetType() const noexcept
{
	return "Graphics Exception";
}

std::string Graphics::GraphicsException::TranslateErrorCode(HRESULT hResult) noexcept
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

HRESULT Graphics::GraphicsException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string Graphics::GraphicsException::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hResult);
}

std::string Graphics::GraphicsException::GetDebugInfo() const noexcept
{
	return debugInfo;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception: Device removed (DXGI_ERROR_DEVICE_REMOVED)";
}