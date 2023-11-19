#pragma comment(lib, "d3d11.lib")

#include <sstream>
#include "Graphics.h"


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
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = windowHandle;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Flags = 0;

	HRESULT hResult = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);

	if (hResult != S_OK)
	{
		throw GRAPHICS_EXCEPTION(hResult);
	}

	ID3D11Resource* pBackBuffer = nullptr;
	hResult = pSwapChain->GetBuffer(
		0,
		_uuidof(ID3D11Resource),
		reinterpret_cast<void**>(&pBackBuffer)
	);

	if (hResult != S_OK)
	{
		throw GRAPHICS_EXCEPTION(hResult);
	}

	hResult = pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget
	);

	if (hResult != S_OK)
	{
		throw GRAPHICS_EXCEPTION(hResult);
	}

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
	HRESULT hResult = pSwapChain->Present(1, 0);

	if (hResult == DXGI_ERROR_DEVICE_REMOVED)
	{
		throw DEVICE_REMOVED_EXCEPTION(hResult);
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

Graphics::GraphicsException::GraphicsException(int line, const char* file, HRESULT hResult) noexcept
	:
	Exception(line, file),
	hResult(hResult)
{
}

const char* Graphics::GraphicsException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
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

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception: Device removed (DXGI_ERROR_DEVICE_REMOVED)";
}