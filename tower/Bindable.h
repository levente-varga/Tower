#pragma once

#include "Graphics.h"
#include "DebugInfoManager.h"

class Bindable
{
public:
	virtual void Bind(Graphics& graphics) noexcept = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(Graphics& graphics) noexcept;
	static ID3D11Device* GetDevice(Graphics& graphics) noexcept;
	static DebugInfoManager& GetInfoManager(Graphics& graphics) noexcept(!IS_DEBUG);
};