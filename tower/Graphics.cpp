#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <sstream>
#include "Graphics.h"
#include <DirectXMath.h>
#include "GraphicsMacros.h"


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

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	GRAPHICS_THROW_INFO(pSwapChain->GetBuffer(
		0,
		_uuidof(ID3D11Resource),
		&pBackBuffer
	));

	GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView
	));

	// Create and bind depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	GRAPHICS_THROW_INFO(pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState));
	pContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	// Create depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 800u;
	textureDesc.Height = 600u;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GRAPHICS_THROW_INFO(pDevice->CreateTexture2D(&textureDesc, nullptr, &pDepthStencilTexture));

	// Create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;
	GRAPHICS_THROW_INFO(pDevice->CreateDepthStencilView(
		pDepthStencilTexture.Get(), &depthStencilViewDesc, &pDepthStencilView
	));

	// Bind render target and depth stencil
	pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

	// Configure viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pContext->RSSetViewports(1, &viewport);
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
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
	GRAPHICS_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0, 0));
}

void Graphics::SetProjectionMatrix(DirectX::XMMATRIX projection) noexcept
{
	projectionMatrix = projection;
}

DirectX::XMMATRIX Graphics::GetProjectionMatrix() const noexcept
{
	return projectionMatrix;
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




/*
 *	Implementation of Graphics::InfoException
 */

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> messages)
	:
	Exception(line, file)
{
	for (int i = 0; i < messages.size(); i++)
	{
		debugInfo += messages[i];
		if (i < messages.size() - 1) debugInfo += '\n';
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\n"
		<< "\n[Debug Info]\n" << GetDebugInfo() << "\n\n"
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetDebugInfo() const noexcept
{
	return debugInfo;
}