#pragma once

#include "CustomWindows.h"
#include "Exception.h"
#include <d3d11.h>
#include <vector>
#include "DebugInfoManager.h"
#include <wrl.h>

class Graphics
{
public:
	Graphics(HWND windowHandle);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(Graphics&) = delete;
	~Graphics() = default;

	void Present();
	void ClearBuffer(float red, float green, float blue) noexcept;

	void DrawTestTriangle(float angle, float x, float y);

private:
	// &p on these is equal to p.ReleaseAndGetAddressOf()
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;

	// Only have this field if the app is built in debug mode
#ifndef NDEBUG
	DebugInfoManager infoManager;
#endif

public:
	class GraphicsException : public Exception
	{
		using Exception::Exception;
	public:
		GraphicsException(int line, const char* file, HRESULT hResult, std::vector<std::string> debugInfoMessages = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetDebugInfo() const noexcept;

	private:
		HRESULT hResult;
		std::string debugInfo;
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> messages = {});
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetDebugInfo() const noexcept;
	private:
		std::string debugInfo;
	};

	class DeviceRemovedException : public GraphicsException
	{
		using GraphicsException::GraphicsException;
	public:
		const char* GetType() const noexcept override;

	private:
		std::string reason;
	};
};
