#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <sstream>
#include "Graphics.h"
#include <d3dcompiler.h>
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

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	GRAPHICS_THROW_INFO(pSwapChain->GetBuffer(
		0,
		_uuidof(ID3D11Resource),
		&pBackBuffer
	));

	GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTarget
	));
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
	pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
	GRAPHICS_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0, 0));
}

void Graphics::DrawTestTriangle(float angle, float x, float y)
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		}
		position;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}
		color;
	};

	const Vertex vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f, 0,   0,   0,   255},
		{  1.0f, -1.0f, -1.0f, 255, 0,   0,   255},
		{ -1.0f,  1.0f, -1.0f, 0,   255, 0,   255},
		{  1.0f,  1.0f, -1.0f, 255, 255, 0,   255},
		{ -1.0f, -1.0f,  1.0f, 0,   0,   255, 255},
		{  1.0f, -1.0f,  1.0f, 255, 0,   255, 255},
		{ -1.0f,  1.0f,  1.0f, 0,   255, 255, 255},
		{  1.0f,  1.0f,  1.0f, 255, 255, 255, 255},
	};

	HRESULT hResult;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &pVertexBuffer));


	// Bind vertex buffer to render pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// Create index buffer
	const unsigned short indices[] =
	{
		1, 0, 2,   3, 1, 2,
		0, 1, 4,   1, 5, 4,
		2, 0, 4,   6, 2, 4,
		4, 5, 6,   5, 7, 6,
		3, 2, 6,   7, 3, 6,
		1, 3, 5,   3, 7, 5,
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &pIndexBuffer));


	// Bind index buffer to render pipeline
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);


	// Create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer constantBuffer =
	{
		DirectX::XMMatrixTranspose( // The vertex shader expects a column-major matrix
			DirectX::XMMatrixRotationZ(angle * 2.0f) *
			DirectX::XMMatrixRotationX(angle * 2.0f) *
			DirectX::XMMatrixRotationY(angle / 2.0f) *
			DirectX::XMMatrixTranslation(x, y, 8.0f) *
			DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		)
	};

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.ByteWidth = sizeof(constantBuffer);
	constantBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
	constantSubresourceData.pSysMem = &constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&constantBufferDesc, &constantSubresourceData, &pConstantBuffer));

	// Bind index buffer to render pipeline
	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());


	// Create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	));


	// Bind pixel shader to render pipeline
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);


	// Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));


	// Bind vertex shader to render pipeline
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);


	// Create Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{
			"Position",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
		{
			"Color",
			0,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			0,
			12, // 12 bytes offset
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
	};
	GRAPHICS_THROW_INFO(pDevice->CreateInputLayout(
		inputElementDesc,
		(UINT)std::size(inputElementDesc),
		pBlob->GetBufferPointer(),	// blob still contains vertex shader
		pBlob->GetBufferSize(),
		&pInputLayout
	));


	// Bind input layout
	pContext->IASetInputLayout(pInputLayout.Get());


	// Bind render target
	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), nullptr);


	// Set primitive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Configure viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pContext->RSSetViewports(1, &viewport);


	GRAPHICS_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0, 0));
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