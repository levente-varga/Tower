#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& graphics, const std::wstring& path);
	void Bind(Graphics& graphics) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};