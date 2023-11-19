#pragma once

#include "CustomWindows.h"
#include "Exception.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND windowHandle);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(Graphics&) = delete;
	~Graphics();

	void Present();
	void ClearBuffer(float red, float grree, float blue) noexcept;

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

public:
	class GraphicsException : public Exception
	{
		using Exception::Exception;
	public:
		GraphicsException(int line, const char* file, HRESULT hResult) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;

	private:
		HRESULT hResult;
	};

	class DeviceRemovedException : public GraphicsException
	{
		using GraphicsException::GraphicsException;
	public:
		const char* GetType() const noexcept override;
	};
};

#define GRAPHICS_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, hResult)
#define DEVICE_REMOVED_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, hResult)