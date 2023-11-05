#pragma once

#include "CustomWindows.h"
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
};